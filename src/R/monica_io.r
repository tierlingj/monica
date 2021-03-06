# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/. */

# Authors:
# Michael Berg-Mohnicke <michael.berg@zalf.de>
#
# Maintainers:
# Currently maintained by the authors.
#
# This file is part of the MONICA model created at the Institute of
# Landscape Systems Analysis at the ZALF.
# Copyright (C: Leibniz Centre for Agricultural Landscape Research (ZALF)


#import soil_io
#import monica_python

library(stringr)
library(sets)
library(readr)
library(jsonlite)
library(rlist)
library(rzmq)

print("local monica_io.r")

OP_AVG = 0
OP_MEDIAN = 1
OP_SUM = 2
OP_MIN = 3
OP_MAX = 4
OP_FIRST = 5
OP_LAST = 6
OP_NONE = 7
OP_UNDEFINED_OP_ = 8

ORGAN_ROOT = 0
ORGAN_LEAF = 1
ORGAN_SHOOT = 2
ORGAN_FRUIT = 3
ORGAN_STRUCT = 4
ORGAN_SUGAR = 5
ORGAN_UNDEFINED_ORGAN_ = 6


oid_is_organ <- function(oid)
{
  oid$organ != ORGAN_UNDEFINED_ORGAN_
}

oid_is_range <- function(oid)
{
  oid$fromLayer >= 0 && oid$toLayer >= 0 #\
    #&& oid$fromLayer < oid$toLayer
}

op_to_string <- function(op)
{
  op_name = switch(op + 1,
    "AVG", #OP_AVG
    "MEDIAN", #OP_MEDIAN
    "SUM", #OP_SUM
    "MIN", #OP_MIN
    "MAX", #OP_MAX
    "FIRST", #OP_FIRST
    "LAST", #OP_LAST
    "NONE", #OP_NONE
    "undef" #OP_UNDEFINED_OP_
  )
  op_name
}
  

organ_to_string <- function(organ)
{
  o_name = switch(organ + 1, 
    "Root", #ORGAN_ROOT
    "Leaf", #ORGAN_LEAF
    "Shoot", #ORGAN_SHOOT
    "Fruit", #ORGAN_FRUIT
    "Struct", #ORGAN_STRUCT
    "Sugar", #ORGAN_SUGAR
    "undef" #ORGAN_UNDEFINED_ORGAN_
  )
  o_name
}


oid_to_string <- function(oid, include_time_agg)
{
  paste0(
    "[", 
    oid$name,
    if(oid_is_organ(oid)) 
      paste0(", ", organ_to_string(oid$organ))
    else {
      if(oid_is_range(oid))
        paste0(", [", 
              oid$fromLayer + 1,
              ", ", 
              oid$toLayer + 1,
              if(oid$layerAggOp != OP_NONE)
                paste0(", ", op_to_string(oid$layerAggOp))
              else
                "",
              "]")
      else {
        if(oid$fromLayer >= 0)
          paste0(", ", oid$fromLayer + 1)
        else
          ""
      }
    },
    if(include_time_agg)
      paste0(", ", op_to_string(oid$timeAggOp))
    else
     "",
    "]"
  )
}

create_R_from_JSON_string <- function(json_string)
{
  jsonlite::fromJSON(json_string, 
                     simplifyVector = FALSE, 
                     simplifyDataFrame = FALSE,
                     simplifyMatrix = FALSE)
}

create_R_from_JSON_file <- function(path_to_json_file)
{
  create_R_from_JSON_string(read_file(path_to_json_file))
}

create_JSON_string_from_R <- function(R_data)
{
  rjson::toJSON(R_data)
}

send_JSON_string <- function(socket, json_string)
{
  #send.socket(socket, rj_in_str)
  send.raw.string(socket, json_string)
}

receive_JSON_string <- function(socket)
{
  receive.string(socket)
}


create_output_header_rows <- function(output_ids,
                                     include_header_row=TRUE,
                                     include_units_row=TRUE,
                                     include_time_agg=FALSE)
{
  row1 = list()
  row2 = list()
  row3 = list()
  row4 = list()
  for(oid in output_ids)
  {
    from_layer <- oid$fromLayer
    to_layer <- oid$toLayer
    is_organ <- oid_is_organ(oid)
    is_range <- oid_is_range(oid) && oid$layerAggOp == OP_NONE
    if(is_organ)
        # organ is being represented just by the value of fromLayer currently
        to_layer <- from_layer <- oid$organ
    else
    { 
      if(is_range)
      {
        from_layer <- from_layer + 1
        to_layer <- to_layer + 1     # display 1-indexed layer numbers to users
      }
      else
        to_layer <- from_layer # for aggregated ranges, which aren't being displayed as range
    }
      
    for(i in from_layer : to_layer)
    {
      str1 <- ""
      if(is_organ)
        str1 <- paste0(str1, 
                      if(nchar(oid$displayName) == 0) 
                        paste0(oid$name, "/", organ_to_string(oid$organ))
                      else 
                        oid$displayName)
      else {
        if(is_range)
          str1 <- paste0(str1, 
                        if(nchar(oid$displayName) == 0)
                          paste0(oid$name, "_", i)
                        else 
                          oid$displayName)
        else
          str1 <- paste0(str1, 
                        if(nchar(oid$displayName) == 0)
                          oid$name 
                        else 
                          oid$displayName)
      }
      row1 <- list.append(row1, str1)
      row4 <- list.append(row4, paste0("j:", gsub("\"", "", oid$jsonInput)))
      row3 <- list.append(row3, paste0("m:",  oid_to_string(oid, include_time_agg)))
      row2 <- list.append(row2, paste0("[", oid$unit, "]"))
    }
  }

  out = list()
  if(include_header_row)
    out <- list.append(out, row1)
  if(include_units_row)
    out <- list.append(out, row4)
  if(include_time_agg)
  {
    out <- list.append(out, row3)
    out <- list.append(out, row2)
  }
  
  out
}


create_output_data_rows <- function(output_ids, values)
{
  #"write actual output lines"
  out <- list()
  if(length(values) > 0)
  {
    for(k in 1 : length(values[[1]]))
    {
      i <- 1
      row <- list()
      for(. in output_ids)
      {
        j__ <- values[[i]][[k]]
        if(typeof(j__) == "list")
        {
          for(jv_ in j__)
            row <- list.append(row, jv_)
        }
        else
          row <- list.append(row, j__)
        i <- i + 1
      }
      out <- list.append(out, row)
    }
  }
  out
}


write_output <- function(result_json, file = "", sep = ",")
{
  sink(file)
  for(d in result_json$data)
  {
    cat("\"", str_replace_all(d$origSpec, "\"", ""), "\"\n", sep = "")
    
    header <- create_output_header_rows(d$outputIds, 
                                       include_header_row = TRUE,
                                       include_units_row = TRUE,
                                       include_time_agg = FALSE)
    for(h in header)
    {
      do.call(cat, c(h, sep = sep))
      cat("\n") 
    }
    
    data <- create_output_data_rows(d$outputIds, d$results)
    for(d in data)
    {
      do.call(cat, c(d, sep = sep))
      cat("\n") 
    }
    
    cat("\n")
  }
  sink()
}


is_absolute_path <- function(p)
{
  #"is absolute path"
  str_sub(p, 1, 1) == "/" || 
    (nchar(p) == 2 && str_sub(p, 2, 2) == ":") ||
    (nchar(p) > 2 && str_sub(p, 2, 2) == ":" &&
      (str_sub(p, 3, 3) == "\\" || 
        str_sub(p, 3, 3) == "/"))
}

fix_system_separator <- function(path)
{
  #"fix system separator"
  str_replace_all(path, "\\\\", "/")
}

replace_env_vars <- function(path)
{
  #"replace ${ENV_VAR} in path"
  start_token <- "\\$\\{"
  end_token <- "\\}"
  start_pos <- str_locate(path, start_token)[[1]]
  while(!is.na(start_pos))
  {
    end_pos = str_locate(str_sub(path, start_pos+1), end_token)[[1]] + start_pos
    if(!is.na(end_pos))
    {
      name_start = start_pos + 2
      env_var_name = str_sub(path, name_start, end_pos - 1)
      env_var_content <- Sys.getenv(env_var_name)
      if(nchar(env_var_content) > 0)
      {
        var = paste0(start_token, env_var_name, end_token)
        path = str_replace(path, var, fix_system_separator(env_var_content))
        start_pos = str_locate(path, start_token)[[1]]
      }
      else
        start_pos = str_locate(str_sub(path, end_pos + 1), start_token)
    }
    else
      break
  }

  path
}

default_value <- function(dic, key, default)
{
  "return default value if key not there"
  if(is.null(dic[[key]])) default else dic[[key]]
}

read_and_parse_json_file <- function(path)
{
  tryCatch(
    return(list("result" = fromJSON(read_file(path), simplifyVector = FALSE), "errors" = list(), "success" = TRUE)),
    error = function(e) 
    {
      return(list("result" = NA,
                  "errors" = list(paste0("Error opening file with path : '", path, "'!")),
                  "success" = FALSE))
    }
  )
}

parse_json_string <- function(jsonString)
{
  list("result" = fromJSON(jsonString), "errors" = list(), "success" = TRUE)
}

is_string_type <- function(j)
{
  typeof(j) == "character"
}

#python list/js array like thing
is_array <- function(j)
{
  is.list(j) && is.null(names(j))
}

#python dict/js object/c++ map like thing
is_dict <- function(j)
{
  is.list(j) && !is.null(names(j)) && length(j) == length(names(j))
}


find_and_replace_references <- function(root, j)
{
  sp <- supported_patterns()

  success <- TRUE
  errors <- list()

  if(is_array(j) && length(j) > 0)
  {
      arr <- list()
      array_is_reference_function <- FALSE

      j1 <- j[[1]]
      if(is_string_type(j1))
      {
        if(!is.null(sp[[j1]]))
        {
          f <- sp[[j1]]
          array_is_reference_function <- TRUE

          #check for nested function invocations in the arguments
          funcArr <- list()
          for(i in j)
          {
            res <- find_and_replace_references(root, i)
            success <- success && res$success
            if(! res$success)
              for(err in res$errors)
                errors <- list.append(errors, err)
            funcArr <- list.append(funcArr, res$result)
          }
            
          #invoke function
          jaes <- f(root, funcArr)

          success <- success && jaes$success
          if(!jaes$success)
            for(err in jaes$errors)
              errors <- list.append(errors, err)

          #if successful try to recurse into result for functions in result
          if(jaes$success)
          {
            res <- find_and_replace_references(root, jaes$result)
            success <- success && res$success
            if(!res$success)
              for(err in res$errors)
                errors <- list.append(errors, err)
            return(list("result" = res$result, "errors" = errors, "success" = length(errors) == 0))
          }
          else
              return(list("result" = {}, "errors" = errors, "success" = length(errors) == 0))
        }
      }

      if(!array_is_reference_function)
      {
        for(jv in j)
        {
          res <- find_and_replace_references(root, jv)
          success <- success && res$success
          if(!res$success)
            for(err in res$errors)
              errors <- list.append(errors, err)
          arr <- list.append(arr, res$result)
        }
      }

      return(list("result" = arr, "errors" = errors, "success" = length(errors) == 0))
  }
  else 
  {
    if(is_dict(j))
    {
      obj = list()

      for(k in names(j))
      {
        v = j[[k]]
        r <- find_and_replace_references(root, v)
        success <- success && r$success
        if(!r$success)
          for(e in r$errors)
            errors <- list.append(errors, e)
        obj[[k]] <- r$result
      }
        
      return(list("result" = obj, "errors" = errors, "success" = length(errors) == 0))
    }
  }

  list("result" = j, "errors" = errors, "success" = length(errors) == 0)
}

supported_patterns <- function(. = NA)
{
  ref <- function(root, j)
  {
    if(is.null(attr(ref, "cache")))
      attr(ref, "cache") <<- list()

    if(length(j) == 3 && 
      is_string_type(j[[2]]) &&
      is_string_type(j[[3]]))
    {
      key1 <- j[[2]]
      key2 <- j[[3]]

      for(v in attr(ref, "cache"))
      {
        if(identical(v[1:2], c(key1, key2)))
          return(v[[3]])
      }

      res <- find_and_replace_references(root, root[[key1]][[key2]])
      attr(ref, "cache") <<- list(key1, key2, res)
      return(res)
    }
      
    list("result" = j,
         "errors" = list(paste0("Couldn't resolve reference: ", toJSON(j), "!")),
         "success" = FALSE)
  }
    

  from_file <- function(root, j__)
  {
    "include from file"
    if(length(j__) == 2 && is_string_type(j__[[2]]))
    {
      base_path <- default_value(root, "include-file-base-path", ".")
      path_to_file <- j__[[2]]
      if(!is_absolute_path(path_to_file))
        path_to_file <- paste0(base_path, "/", path_to_file)
      path_to_file <- replace_env_vars(path_to_file)
      path_to_file <- fix_system_separator(path_to_file)
      jo_ <- read_and_parse_json_file(path_to_file)
      if(jo_$success)
        return(list("result" = jo_$result, "errors" = list(), "success" = TRUE))

      return(list("result" = j__,
                  "errors" = list(paste0("Couldn't include file with path: '", path_to_file, "'!")),
                  "success" = FALSE))
    }
        
    list("result" = j__,
         "errors" = list(paste0("Couldn't include file with function: ", toJSON(j__), "!")),
         "success" = FALSE)
  }
    
    
  #humus_to_corg <- function(., j__)
  #{
  #  "convert humus level to corg"
  #  if(length(j__) == 2 && typeof(j__[2]) == "double")
  #    return(list("result" = soil_io.humus_class_to_corg(j__[2]), "errors" = list(), "success" = TRUE))
  #  
  #  list("result" = j__,
  #       "errors" = list(paste0("Couldn't convert humus level to corg: ", toJSON(j__), "!")),
  #       "success" = FALSE)
  #}
    
  #def ld_to_trd(_, j__):
  #    if len(j__) == 3 \
  #        and isinstance(j__[1], types.IntType) \
  #        and isinstance(j__[2], types.FloatType):
  #        return {"result": soil_io.bulk_density_class_to_raw_density(j__[1], j__[2]), "errors": [], "success": True}
  #    return {"result": j__,
  #            "errors": ["Couldn't convert bulk density class to raw density using function: " + json.dumps(j__) + "!"],
  #            "success": False}

  #def ka5_to_clay(_, j__):
  #    if len(j__) == 2 and is_string_type(j__[1]):
  #        return {"result": soil_io.ka5_texture_to_clay(j__[1]), "errors": [], "success": True}
  #    return {"result": j__,
  #            "errors": ["Couldn't get soil clay content from KA5 soil class: " + json.dumps(j__) + "!"],
  #            "success": False}

  #def ka5_to_sand(_, j__):
  #    if len(j__) == 2 and is_string_type(j__[1]):
  #        return {"result": soil_io.ka5_texture_to_sand(j__[1]), "errors": [], "success": True}
  #    return {"result": j__,
  #            "errors": ["Couldn't get soil sand content from KA5 soil class: " + json.dumps(j__) + "!"],
  #            "success": False}

  #def sand_clay_to_lambda(_, j__):
  #    if len(j__) == 3 \
  #        and isinstance(j__[1], types.FloatType) \
  #        and isinstance(j__[2], types.FloatType):
  #        return {"result": soil_io.sand_and_clay_to_lambda(j__[1], j__[2]), "errors": [], "success": True}
  #    return {"result": j__,
  #            "errors": ["Couldn't get lambda value from soil sand and clay content: " + json.dumps(j__) + "!"],
  #            "success": False}

  percent <- function(., j__)
  {
    if(length(j__) == 2 && typeof(j__[[2]]) == "double")
      return(list("result" = j__[[2]] / 100.0, "errors" = list(), "success": TRUE))
    
    list("result" = j__,
         "errors" = list(parse("Couldn't convert percent to decimal percent value: ", toJSON(j__), "!", sep="")),
         "success" = FALSE)
  }
      
  if(is.null(attr(supported_patterns, "m")))
    attr(supported_patterns, "m") <<- list(
      #"include-from-db" = fromDb
      "include-from-file" = from_file,
      "ref" = ref,
      #"humus_st2corg" = humus_to_corg,
      #"humus-class->corg" = humus_to_corg,
      #"ld_eff2trd" = ld_to_trd,
      #"bulk-density-class->raw-density" = ld_to_trd,
      #"KA5TextureClass2clay" = ka5_to_clay,
      #"KA5-texture-class->clay" = ka5_to_clay,
      #"KA5TextureClass2sand" = ka5_to_sand,
      #"KA5-texture-class->sand" = ka5_to_sand,
      #"sandAndClay2lambda" = sand_clay_to_lambda,
      #"sand-and-clay->lambda" = sand_clay_to_lambda,
      "%" = percent
    )
  
  m <- attr(supported_patterns, "m")
  attr(supported_patterns, "m")
}

print_possible_errors <- function(errs, include_warnings=FALSE)
{
  if(!errs$success)
    for(err in errs$errors)
      print(err)

  if(include_warnings && "warnings" %in% errs)
    for(war in errs$warnings)
      print(war)

  errs$success
}


create_env_json_from_json_config <- function(crop_site_sim)
{
  #"create the json version of the env from the json config files"
  for(k in names(crop_site_sim))
  {
    j <- crop_site_sim[[k]]
    if(is.null(j))
      return(NA)
  }
    
  path_to_parameters <- crop_site_sim$sim$"include-file-base-path"
  
  add_base_path <- function(j, base_path)
  {
    #"add include file base path if not there"
    if(is.null(j$"include-file-base-path"))
      j$"include-file-base-path" <- base_path
    j
  }
          
  crop_site_sim2 = list()
  #collect all errors in all files and don't stop as early as possible
  errors = list()
  for(k in names(crop_site_sim))
  {
    j <- crop_site_sim[[k]]
    if(k == "climate")
      next

    j = add_base_path(j, path_to_parameters)
    res <- find_and_replace_references(j, j)
    if(res$success)
      crop_site_sim2[[k]] <- res$result
    else
      errors <- list.append(errors, res$errors)
  }
     
  if(length(errors) > 0)
  {
    for(err in as.set(errors))
      print(err)
    return(NA)
  }
  
  cropj <- crop_site_sim2$crop
  sitej <- crop_site_sim2$site
  simj <- crop_site_sim2$sim
  
  env <- list(
    type = "Env",
    
    #store debug mode in env, take from sim.json, but prefer params map 
    debugMode = simj$"debug?"
  )

  cpp <- list(
      "type" = "CentralParameterProvider",
      "userCropParameters" = cropj$CropParameters,
      "userEnvironmentParameters" = sitej$EnvironmentParameters,
      "userSoilMoistureParameters" = sitej$SoilMoistureParameters,
      "userSoilTemperatureParameters" = sitej$SoilTemperatureParameters,
      "userSoilTransportParameters" = sitej$SoilTransportParameters,
      "userSoilOrganicParameters" = sitej$SoilOrganicParameters,
      "simulationParameters" = simj,
      "siteParameters" = sitej$SiteParameters
  )
  
  env[["params"]] <- cpp
  env[["cropRotation"]] <- cropj$cropRotation
  env[["events"]] <- simj$output$events
  
  env[["pathToClimateCSV"]] <- simj$climate.csv
  env[["csvViaHeaderOptions"]] <- simj$"climate.csv-options"
 
  climate_csv_string = crop_site_sim$climate
  if(!is.null(climate_csv_string) && nchar(climate_csv_string) > 0)
    add_climate_data_to_env(env, simj, climate_csv_string)
  
  return(env)
}

add_climate_data_to_env <- function(env, simj, climate_csv_string=NA)
{
  #"add climate data separately to env"

  if(is.NA(climate_csv_string))
    climate_csv_string <- read_file(simj$climate.csv)

  if(climate_csv_string)
    env$climateData = json.loads(monica_python.readClimateDataFromCSVStringViaHeadersToJsonString(climate_csv_string, toJSON(simj$"climate.csv-options")))

  env
}
