/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
Authors: 
Claas Nendel <claas.nendel@zalf.de>
Xenia Specka <xenia.specka@zalf.de>
Michael Berg <michael.berg@zalf.de>

Maintainers: 
Currently maintained by the authors.

This file is part of the MONICA model. 
Copyright (C) Leibniz Centre for Agricultural Landscape Research (ZALF)
*/

#ifndef __SOIL_ORGANIC_H
#define __SOIL_ORGANIC_H

/**
 * @file soilorganic.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <list>

#include "monica-parameters.h"

namespace Monica
{
  // forward declaration
  class SoilColumn;
  class CropGrowth;

  /**
 * @brief Soil carbon and nitrogen part of model
 *
 * <img src="../images/soil_organic_schema.png">
 *
 * <img src="../images/nitrification_denitrification.png">
 *
 * @author Michael Berg, Claas Nendel, Xenia Specka
 */
  class SoilOrganic
  {
  public:
    SoilOrganic(SoilColumn& soilColumn,
                const SiteParameters& sps,
                const UserSoilOrganicParameters& userParams);

    void step(double vw_Precipitation, double vw_MeanAirTemperature, double vw_WindSpeed);

    void addOrganicMatter(OrganicMatterParametersPtr addedOrganicMatter,
													std::map<int, double> layer2amount,
													double nConcentration = 0);

		void addOrganicMatter(OrganicMatterParametersPtr addedOrganicMatter,
													double amount,
													double nConcentration = 0,
													int intoLayerIndex = 0)
		{
			addOrganicMatter(addedOrganicMatter, {{intoLayerIndex, amount}}, nConcentration);
		}


    void addIrrigationWater(double amount);

    /**
     * @brief TRUE, if organic fertilizer is added with a following corporation.
     *
     * Because such a corporation only affects the first layer, no tillage
     * is done for incorporation.
     *
     * @param incorporation TRUE/FALSE
     */
    void setIncorporation(bool incorp) { this->incorporation = incorp; }
    void put_Crop(CropGrowth* crop);
    void remove_Crop();

    double get_SoilOrganicC(int i_Layer) const;
    double get_AOM_FastSum(int i_Layer) const;
    double get_AOM_SlowSum(int i_Layer) const;
    double get_SMB_Fast(int i_Layer) const;
    double get_SMB_Slow(int i_Layer) const;
    double get_SOM_Fast(int i_Layer) const;
    double get_SOM_Slow(int i_Layer) const;
		double get_CBalance(int i_Layer) const;
    double get_SMB_CO2EvolutionRate(int i_layer) const;
    double get_ActDenitrificationRate(int i_Layer) const;
    double get_NetNMineralisationRate(int i_Layer) const;
    double get_NH3_Volatilised() const;
    double get_SumNH3_Volatilised() const;
    double get_N2O_Produced() const;
    double get_SumN2O_Produced() const;
    double get_NetNMineralisation() const;
    double get_SumNetNMineralisation() const;
    double get_SumDenitrification() const;
    double get_Denitrification() const;
    double get_DecomposerRespiration() const;
    double get_NetEcosystemProduction() const;
    double get_NetEcosystemExchange() const;

		double get_Organic_N(int i_Layer) const;

    double actAmmoniaOxidationRate(int i) const {
      return vo_ActAmmoniaOxidationRate.at(i);
    }

    double actNitrificationRate(int i) const {
      return vo_ActNitrificationRate.at(i);
    }
    double actDenitrificationRate(int i) const {
      return vo_ActDenitrificationRate.at(i);
    }

  private:
    //void fo_OM_Input(bool vo_AOM_Addition);
    void fo_Urea(double vo_RainIrrigation);
    void fo_MIT();
    void fo_Volatilisation(bool vo_AOM_Addition, double vw_MeanAirTemperature, double vw_WindSpeed);
    
    // MONICA nitrification code
    void fo_Nitrification();
    
    // use STICS nitrification code
    void fo_stics_Nitrification();

    // MONICA dentrification code
    void fo_Denitrification();

    // use STICS denitrification code
    void fo_stics_Denitrification();

    // MONICA N2O production code
    double fo_N2OProduction();

    // use STICS N2O production code
    double fo_stics_N2OProduction();

    void fo_PoolUpdate();
    double fo_NetEcosystemProduction(double vc_NetPrimaryProduction, double vo_DecomposerRespiration);
    double fo_NetEcosystemExchange(double vc_NetPrimaryProduction, double vo_DecomposerRespiration);
    double fo_ClayOnDecompostion(double d_SoilClayContent, double d_LimitClayEffect);
    double fo_TempOnDecompostion(double d_SoilTemperature);
    double fo_MoistOnDecompostion(double d_SoilMoisture_pF);
    double fo_MoistOnHydrolysis(double d_SoilMoisture_pF);
    double fo_TempOnNitrification(double d_SoilTemperature);
    double fo_MoistOnNitrification(double d_SoilMoisture_pF);
    double fo_MoistOnDenitrification(double d_SoilMoisture_m3, double d_Saturation);
    double fo_NH3onNitriteOxidation (double d_SoilNH4, double d_SoilpH);
		void fo_distributeDeadRootBiomass();
    SoilColumn& soilColumn;
    const SiteParameters& siteParams;
    const UserSoilOrganicParameters& organicPs;

    std::size_t vs_NumberOfLayers{0};
    std::size_t vs_NumberOfOrganicLayers{0};
    bool addedOrganicMatter{false};
    double irrigationAmount{0.0};
    std::vector<double> vo_ActAmmoniaOxidationRate; //!< [kg N m-3 d-1]
    std::vector<double> vo_ActNitrificationRate; //!< [kg N m-3 d-1]
    std::vector<double> vo_ActDenitrificationRate; //!< [kg N m-3 d-1]
    std::vector<double> vo_AOM_FastDeltaSum;
		double vo_AOM_FastInput{0.0}; //!< AOMfast pool change by direct input [kg C m-3]
    std::vector<double> vo_AOM_FastSum;
    std::vector<double> vo_AOM_SlowDeltaSum;
    double vo_AOM_SlowInput{0.0}; //!< AOMslow pool change by direct input [kg C m-3]
    std::vector<double> vo_AOM_SlowSum;
    std::vector<double> vo_CBalance;
    double vo_DecomposerRespiration{0.0};
    std::string vo_ErrorMessage;
    std::vector<double> vo_InertSoilOrganicC;
    double vo_N2O_Produced{0.0}; // [kg-N2O-N/ha]
    double vo_NetEcosystemExchange{0.0};
    double vo_NetEcosystemProduction{0.0};
    double vo_NetNMineralisation{0.0};
    std::vector<double> vo_NetNMineralisationRate;
    double vo_Total_NH3_Volatilised{0.0};
    double vo_NH3_Volatilised{0.0};
    std::vector<double> vo_SMB_CO2EvolutionRate;
    std::vector<double> vo_SMB_FastDelta;
    std::vector<double> vo_SMB_SlowDelta;
    std::vector<double> vs_SoilMineralNContent;
    std::vector<double> vo_SoilOrganicC;
    std::vector<double> vo_SOM_FastDelta;
    double vo_SOM_FastInput{0.0}; //!< SOMfast pool change by direct input [kg C m-3]
    std::vector<double> vo_SOM_SlowDelta;
    double vo_SumDenitrification{0.0}; // kg-N/m2
    double vo_SumNetNMineralisation{0.0};
    double vo_SumN2O_Produced{0.0};
    double vo_SumNH3_Volatilised{0.0};
    double vo_TotalDenitrification{0.0};

    /*
    struct AddedOMParams {
      double vo_AddedOrganicCarbonAmount;
      double vo_AddedOrganicMatterAmount;
      double vo_AOM_DryMatterContent;
      double vo_AOM_NH4Content;
      double vo_AOM_NO3Content;
      double vo_PartAOM_to_AOM_Slow;
      double vo_PartAOM_to_AOM_Fast;
    };

    std::vector<AddedOMParams> newOM;

     */

    //! True, if organic fertilizer has been added with a following incorporation.
    //! Parameter is automatically set to false, if carbamid amount is falling below 0.001.
    bool incorporation{false};
    CropGrowth* crop{nullptr};
  };

} // namespace Monica

#endif

