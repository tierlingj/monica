/**
Authors: 
Dr. Claas Nendel <claas.nendel@zalf.de>
Xenia Specka <xenia.specka@zalf.de>
Michael Berg <michael.berg@zalf.de>

Maintainers: 
Currently maintained by the authors.

This file is part of the MONICA model. 
Copyright (C) 2007-2013, Leibniz Centre for Agricultural Landscape Research (ZALF)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MONICA_PARAMETERS_H_
#define MONICA_PARAMETERS_H_

/**
 * @file monica-parameters.h
 */

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "json11/json11.hpp"

#include "climate/climate-common.h"
#include "tools/date.h"
#include "soil/soil.h"
#include "monica-typedefs.h"

namespace Monica
{
#undef min
#undef max

	inline std::string pathSeparator()
	{
		return
#ifdef __unix__
			"/";
#else
			"\\";
#endif
	}

	class CentralParameterProvider;

	enum Eva2_Nutzung
	{
		NUTZUNG_UNDEFINED=0,
		NUTZUNG_GANZPFLANZE=1,
		NUTZUNG_KORN=2,
		NUTZUNG_GRUENDUENGUNG=7,
		NUTZUNG_CCM=8
	};

	const double UNDEFINED = -9999.9;
	const int UNDEFINED_INT = -9999;

	enum Region
	{
		weisseritz, uecker
	};

	enum { MONTH=12 };

	//----------------------------------------------------------------------------

	/**
	* @brief Enumeration for defining automatic harvesting times
	* Definition of different harvest time definition for the automatic
	* yield trigger
	*/
	enum AutomaticHarvestTime
	{
		maturity,	/**< crop is harvested when maturity is reached */
		unknown		/**< default error value */
	};


	//----------------------------------------------------------------------------

	enum ResultId
	{
		//! primary yield for the crop (e.g. the actual fruit)
		primaryYield,

		//! secondary yield for the crop (e.g. leafs and other stuff useable)
		secondaryYield,


		//! above ground biomass of the crop
		aboveGroundBiomass,

		//! Julian day of anthesis of the crop
		anthesisDay,

		//! Julian day of maturity of the crop
		maturityDay,
		
		//! Julian day of harvest
		harvestDay,

		//! sum of applied fertilizer for that crop during growth period
		sumFertiliser,

		//! sum of used irrigation water for the crop during growth period
		sumIrrigation,

		//! sum of N turnover
		sumMineralisation,

		//! the monthly average of the average corg content in the first 10 cm soil
		avg10cmMonthlyAvgCorg,

		//! the monthly average of the average corg content in the first 30 cm soil
		avg30cmMonthlyAvgCorg,

		//! the monthly average of the summed up water content in the first 90 cm soil
		mean90cmMonthlyAvgWaterContent,

		//! at some day in the year the sum of the N content in the first 90cm soil
		sum90cmYearlyNatDay,

		//! the monthly summed up amount of ground water recharge
		monthlySumGroundWaterRecharge,

		//! the monthly sum of N leaching
		monthlySumNLeaching,

		//! height of crop at harvesting date
		cropHeight,

		//! sum of NO3 content in the first 90cm soil at a special, hardcoded date
		sum90cmYearlyNO3AtDay,

		//! sum of NH4 content in the first 90cm soil at a special, hardcoded date
		sum90cmYearlyNH4AtDay,

		//! value of maximal snow depth during simulation duration
		maxSnowDepth,

		//! sum of snow depth for every day
		sumSnowDepth,

		//! sum of frost depth
		sumFrostDepth,

		//! Average soil temperature in the first 30cm soil at special, hardcoded date
		avg30cmSoilTemperature,

		//! Sum of soil temperature in the first 30cm soil at special, hardcoded date
		sum30cmSoilTemperature,

		//! Average soilmoisture content in first 30cm soil at special, hardcoded date
		avg0_30cmSoilMoisture,

		//! Average soilmoisture content in 30-60cm soil at special, hardcoded date
		avg30_60cmSoilMoisture,

		//! Average soilmoisture content in 60-90cm soil at special, hardcoded date
		avg60_90cmSoilMoisture,

		//! Average soilmoisture content in 0-90cm soil at special, hardcoded date
		avg0_90cmSoilMoisture,

		//! water flux at bottom layer of soil at special, hardcoded date
		waterFluxAtLowerBoundary,

		//! capillary rise in first 30 cm soil at special date
		avg0_30cmCapillaryRise,

		//! capillary rise in 30-60 cm soil at special date
		avg30_60cmCapillaryRise,

		//! capillary rise in 60-90cm cm soil at special date
		avg60_90cmCapillaryRise,

		//! percolation ratein first 30 cm soil at special date
		avg0_30cmPercolationRate,

		//! percolation ratein 30-60 cm soil at special date
		avg30_60cmPercolationRate,

		//! percolation rate in 60-90cm cm soil at special date
		avg60_90cmPercolationRate,

		//! sum of surface run off volumes during hole simulation duration
		sumSurfaceRunOff,

		//! Evapotranspiration amount at a special date
		evapotranspiration,

		//! Transpiration amount at a special date
		transpiration,

		//! Evaporation amount at a special date
		evaporation,

		//! N content in biomass after harvest
		biomassNContent,

		//! N content in above ground biomass after harvest
		aboveBiomassNContent,

		//! sum of total N uptake of plant
		sumTotalNUptake,

		//! sum of CO2 evolution rate in first 30 cm soil at special date
		sum30cmSMB_CO2EvolutionRate,

		//! volatilised NH3 at a special date
		NH3Volatilised,

		//! sum of all volatilised NH3
		sumNH3Volatilised,

		//! sum of denitrification rate in first 30cm at a special date
		sum30cmActDenitrificationRate,

		//! leaching N at boundary at special date
		leachingNAtBoundary,

		//! leaching N accumulated for a year
		yearlySumNLeaching,

		//! Groundwater recharge accumulated for a year
		yearlySumGroundWaterRecharge,

		//! Evapotranspiration in time of crop cultivation
		sumETaPerCrop,
		sumTraPerCrop,
		cropname,
		primaryYieldTM,
		secondaryYieldTM,
		monthlySurfaceRunoff,
		monthlyPrecip,
		monthlyETa,
		monthlySoilMoistureL0,
		monthlySoilMoistureL1,
		monthlySoilMoistureL2,
		monthlySoilMoistureL3,
		monthlySoilMoistureL4,
		monthlySoilMoistureL5,
		monthlySoilMoistureL6,
		monthlySoilMoistureL7,
		monthlySoilMoistureL8,
		monthlySoilMoistureL9,
		monthlySoilMoistureL10,
		monthlySoilMoistureL11,
		monthlySoilMoistureL12,
		monthlySoilMoistureL13,
		monthlySoilMoistureL14,
		monthlySoilMoistureL15,
		monthlySoilMoistureL16,
		monthlySoilMoistureL17,
		monthlySoilMoistureL18,
		daysWithCrop,
		NStress,
		WaterStress,
		HeatStress,
		OxygenStress,
		dev_stage
	};

	/*!
	 * @return list of results from a single crop
	 */
	const std::vector<ResultId>& cropResultIds();

  std::pair<std::string, std::string>
  nameAndUnitForResultId(ResultId rid);

	const std::vector<int>& eva2CropResultIds();
	const std::vector<int>& eva2MonthlyResultIds();

	/*!
	 * @return list of the montly results
	 */
	const std::vector<ResultId>& monthlyResultIds();

	/**
	 * @return list if ids used for sensitivity analysis
	 */
	const std::vector<int>& CCGermanyResultIds();

	struct ResultIdInfo {
		ResultIdInfo(const std::string& n, const std::string& u,
								 const std::string& sn = std::string())
			: name(n), unit(u), shortName(sn) {}
		std::string name;
		std::string unit;
		std::string shortName;
	};

	ResultIdInfo resultIdInfo(ResultId rid);

	/**
	 * @brief structure holding the results for a particular crop (in one year usually)
	 */
	struct PVResult
	{
		/*!
		 * @brief default constructor initializing to sensible values
		 * especially for use as value type
		 */
		PVResult() : id(-1), customId(-1) { }

		//! id of crop
		CropId id;

		//custom id to enable mapping of monica results to user defined other entities
		//e.g. a crop activity id from Carbiocial
		int customId;
    Tools::Date date;

		//! different results for a particular crop
		std::map<ResultId, double> pvResults;
	};

  typedef std::shared_ptr<PVResult> PVResultPtr;

	//----------------------------------------------------------------------------

	/**
	 * @brief
	 */
	struct YieldComponent
	{
		/**
		 * @brief Constructor
		 * @param oid organ ID
		 * @param yp Yield percentage
		 */
		YieldComponent(int oid, double yp, double ydm) : organId(oid), yieldPercentage(yp), yieldDryMatter(ydm) { }

		int organId; /**<  */
		double yieldPercentage; /**<  */
		double yieldDryMatter;
	};

	//----------------------------------------------------------------------------

	/**
	 * @brief Parameter for crops
	 */
	struct CropParameters
	{
		CropParameters();

		void resizeStageOrganVectors();
		std::string toString() const;

		//		pc_DevelopmentAccelerationByNitrogenStress(0)

		// members
		std::string pc_CropName; /**< Name */
		bool pc_Perennial;
		int pc_NumberOfDevelopmentalStages; 
		int pc_NumberOfOrgans; 
		int pc_CarboxylationPathway; 
		double pc_DefaultRadiationUseEfficiency;
		double pc_PartBiologicalNFixation;
		double pc_InitialKcFactor; 
		double pc_LuxuryNCoeff; 
		double pc_MaxAssimilationRate;
		double pc_MaxCropDiameter;
		double pc_MaxCropHeight;
		double pc_CropHeightP1; 
		double pc_CropHeightP2; 
		double pc_StageAtMaxHeight;
		double pc_StageAtMaxDiameter; 
		double pc_MinimumNConcentration; 
		double pc_MinimumTemperatureForAssimilation; 
		double pc_NConcentrationAbovegroundBiomass; 
		double pc_NConcentrationB0; 
		double pc_NConcentrationPN; 
		double pc_NConcentrationRoot; 
		double pc_ResidueNRatio; 
		int pc_DevelopmentAccelerationByNitrogenStress; 
		double pc_FieldConditionModifier;
		double pc_AssimilateReallocation;
		double pc_LT50cultivar;
		double pc_FrostHardening;
		double pc_FrostDehardening;
		double pc_LowTemperatureExposure;
		double pc_RespiratoryStress;
		int pc_LatestHarvestDoy;

		std::vector<std::vector<double> > pc_AssimilatePartitioningCoeff; /**<  */
		std::vector<std::vector<double> > pc_OrganSenescenceRate; /**<  */

		std::vector<double> pc_BaseDaylength; /**<  */
		std::vector<double> pc_BaseTemperature; /**<  */
		std::vector<double> pc_OptimumTemperature; /**<  */
		std::vector<double> pc_DaylengthRequirement; /**<  */
		std::vector<double> pc_DroughtStressThreshold; /**<  */
		std::vector<double> pc_OrganMaintenanceRespiration; /**<  */
		std::vector<double> pc_OrganGrowthRespiration; /**<  */
		std::vector<double> pc_SpecificLeafArea; /**<  */
		std::vector<double> pc_StageMaxRootNConcentration; /**<  */
		std::vector<double> pc_StageKcFactor; /**<  */
		std::vector<double> pc_StageTemperatureSum; /**<  */
		std::vector<double> pc_VernalisationRequirement; /**<  */
		std::vector<double> pc_InitialOrganBiomass; /**<  */
		std::vector<double> pc_CriticalOxygenContent; /**<  */

		double pc_CropSpecificMaxRootingDepth; /**<  */
		std::vector<int> pc_AbovegroundOrgan; /**<  */
		std::vector<int> pc_StorageOrgan; /**<  */

		double pc_SamplingDepth;
		double pc_TargetNSamplingDepth;
		double pc_TargetN30;
		double pc_HeatSumIrrigationStart;
		double pc_HeatSumIrrigationEnd;
		double pc_MaxNUptakeParam;
		double pc_RootDistributionParam;
		double pc_PlantDensity;
		double pc_RootGrowthLag;
		double pc_MinimumTemperatureRootGrowth;
		double pc_InitialRootingDepth;
		double pc_RootPenetrationRate;
		double pc_RootFormFactor;
		double pc_SpecificRootLength;
		int pc_StageAfterCut;
		double pc_CriticalTemperatureHeatStress;
		double pc_LimitingTemperatureHeatStress;
		double pc_BeginSensitivePhaseHeatStress;
		double pc_EndSensitivePhaseHeatStress;
		int pc_CuttingDelayDays;
		double pc_DroughtImpactOnFertilityFactor;

		std::vector<YieldComponent> pc_OrganIdsForPrimaryYield;
		std::vector<YieldComponent> pc_OrganIdsForSecondaryYield;
		std::vector<YieldComponent> pc_OrganIdsForCutting;
	};

	//----------------------------------------------------------------------------

	/**
	 * @brief Returns crop parameters for a special crop, specified by cropId.
	 * @param cropID
	 * @param @return CropParameters
	 */
	const CropParameters* getCropParametersFromMonicaDB(int cropId);

	//----------------------------------------------------------------------------

	struct GeneralParameters
	{
		GeneralParameters(double ps_LayerThickness = 0.1,
											double ps_ProfileDepth = 2.0, 
											double ps_MaximumMineralisationDepth = 0.4,
											bool pc_NitrogenResponseOn = true,
											bool pc_WaterDeficitResponseOn = true,
											bool pc_EmergenceFloodingControlOn = true,
											bool pc_EmergenceMoistureControlOn = true);

		/**
		 * @brief Returns number of layers.
		 * @return Number of layers.
		 */
    size_t ps_NumberOfLayers() const { return ps_LayerThickness.size(); }

		std::vector<double> ps_LayerThickness;

		double ps_ProfileDepth;
		double ps_MaxMineralisationDepth;
		bool pc_NitrogenResponseOn;
		bool pc_WaterDeficitResponseOn;
		bool pc_EmergenceFloodingControlOn;
		bool pc_EmergenceMoistureControlOn;
	};

	//----------------------------------------------------------------------------

	/**
	 * @author Claas Nendel, Michael Berg
	 */
	struct SiteParameters
	{
		SiteParameters();

		double vs_Latitude;
		double vs_Slope;                  //!< Neigung [m m-1]
		double vs_HeightNN;               //!< [m]
		double vs_GroundwaterDepth;  //!< Tiefe des Grundwasserspiegels [m]
		double vs_Soil_CN_Ratio;
		double vs_DrainageCoeff;
		double vq_NDeposition;
		double vs_MaxEffectiveRootingDepth;

		std::string toString() const;
	};

	//----------------------------------------------------------------------------

	/**
	* @brief Data structure that containts all relevant parameters for the automatic yield trigger.
	*/
	class AutomaticHarvestParameters
	{
		public:
		/** @brief Constructor */
		AutomaticHarvestParameters() : 
			_harvestTime(AutomaticHarvestTime::unknown),
			_latestHarvestDOY(-1) {}
	
		/** @brief Constructor */
		AutomaticHarvestParameters(AutomaticHarvestTime yt) : 
			_harvestTime(yt),
			_latestHarvestDOY(-1) {}

		/**
		 * @brief Setter for automatic harvest time
		 */
		void setHarvestTime(AutomaticHarvestTime time) { _harvestTime = time;  }

		/**
		 * @brief Getter for automatic harvest time
		 */
		AutomaticHarvestTime getHarvestTime() const { return _harvestTime;  }

		/**
		 * @brief Setter for fallback automatic harvest day
		 */
		void setLatestHarvestDOY(int doy) { _latestHarvestDOY = doy;  }

		/**
		* @brief Getter for fallback automatic harvest day
		*/
		int getLatestHarvestDOY() const { return _latestHarvestDOY; }
		
		std::string toString() const;

		private:

		AutomaticHarvestTime _harvestTime;		/**< Harvest time parameter */
		int _latestHarvestDOY;					/**< Fallback day for latest harvest of the crop */

		
	};

	//----------------------------------------------------------------------------

	class OrganicMatterParameters;
	class AutomaticHarvestParameters;
	
	class Crop;
  typedef std::shared_ptr<Crop> CropPtr;
	
	class Crop
	{
	public:
		//! default constructor for value object use

		Crop(const std::string& name = "fallow") :
			_id(-1), _name(name), _cropParams(NULL), _perennialCropParams(NULL), _residueParams(NULL),
			_primaryYield(0), _secondaryYield(0),_primaryYieldTM(0), _secondaryYieldTM(0),
			_appliedAmountIrrigation(0),_primaryYieldN(0), _secondaryYieldN(0),
			_sumTotalNUptake(0), _crossCropAdaptionFactor(1),
			_cropHeight(0.0), _accumulatedETa(0.0), _accumulatedTranspiration(0.0), eva2_typeUsage(Monica::NUTZUNG_UNDEFINED),
			_anthesisDay(-1), _maturityDay(-1), _automaticHarvest(false){ }

		Crop(CropId id, const std::string& name, const CropParameters* cps = NULL,
				 const OrganicMatterParameters* rps = NULL,
				 double crossCropAdaptionFactor = 1) :
				 _id(id), _name(name), _cropParams(cps), _perennialCropParams(NULL), _residueParams(rps),
			_primaryYield(0), _secondaryYield(0),  _primaryYieldTM(0), _secondaryYieldTM(0),_appliedAmountIrrigation(0), _primaryYieldN(0), _secondaryYieldN(0),
			_sumTotalNUptake(0), _crossCropAdaptionFactor(crossCropAdaptionFactor),
			_cropHeight(0.0), _accumulatedETa(0.0), _accumulatedTranspiration(0.0), eva2_typeUsage(NUTZUNG_UNDEFINED),
			_anthesisDay(-1), _maturityDay(-1), _automaticHarvest(false) { }

		Crop(CropId id, const std::string& name,
				 const Tools::Date& seedDate, const Tools::Date& harvestDate,
				 const CropParameters* cps = NULL, const OrganicMatterParameters* rps = NULL,
				 double crossCropAdaptionFactor = 1) :
			_id(id), _name(name), _seedDate(seedDate), _harvestDate(harvestDate),
			_cropParams(cps), _perennialCropParams(NULL), _residueParams(rps),
			_primaryYield(0), _secondaryYield(0),
			_primaryYieldTM(0), _secondaryYieldTM(0), _primaryYieldN(0), _secondaryYieldN(0),
			_sumTotalNUptake(0),
			_crossCropAdaptionFactor(crossCropAdaptionFactor),
			_cropHeight(0.0), _accumulatedETa(0.0), _accumulatedTranspiration(0.0), eva2_typeUsage(NUTZUNG_UNDEFINED),
			_anthesisDay(-1), _maturityDay(-1), _automaticHarvest(false){ }

		Crop(const Crop& new_crop)
		{
			_id = new_crop._id;
			_name  = new_crop._name;
			_seedDate = new_crop._seedDate;
			_harvestDate = new_crop._harvestDate;
			_cropParams = new_crop._cropParams;
			_perennialCropParams = new_crop._perennialCropParams;
			_residueParams = new_crop._residueParams;
			_primaryYield = new_crop._primaryYield;
			_secondaryYield = new_crop._secondaryYield;
			_primaryYieldTM = new_crop._primaryYieldTM;
			_secondaryYieldTM = new_crop._secondaryYieldTM;
			_primaryYieldN = new_crop._primaryYieldN;
			_secondaryYieldN = new_crop._secondaryYieldN;
			_sumTotalNUptake = new_crop._sumTotalNUptake;
			_appliedAmountIrrigation = new_crop._appliedAmountIrrigation;
			_crossCropAdaptionFactor = new_crop._crossCropAdaptionFactor;
			_cropHeight = new_crop._cropHeight;
			eva2_typeUsage = new_crop.eva2_typeUsage;
			_anthesisDay = new_crop._anthesisDay;
			_maturityDay = new_crop._maturityDay;
			_automaticHarvest = new_crop._automaticHarvest;
			_automaticHarvestParams = new_crop._automaticHarvestParams;

		}

    CropId id() const { return _id; }

    std::string name() const { return _name; }

    bool isValid() const { return _id > -1; }

    const CropParameters* cropParameters() const { return _cropParams; }

    const CropParameters* perennialCropParameters() const { return _perennialCropParams; }

    void setCropParameters(const CropParameters* cps) { _cropParams = cps; }

    void setPerennialCropParameters(const CropParameters* cps) { _perennialCropParams = cps; }

		const OrganicMatterParameters* residueParameters() const
		{
			return _residueParams;
		}

		void setResidueParameters(const OrganicMatterParameters* rps)
		{
			_residueParams = rps;
		}

		Tools::Date seedDate() const { return _seedDate; }

		Tools::Date harvestDate() const { return _harvestDate; }

		std::vector<Tools::Date> getCuttingDates() const { return _cuttingDates; }

		void setSeedAndHarvestDate(const Tools::Date& sd, const Tools::Date& hd)
		{
			_seedDate = sd;
			_harvestDate = hd;
		}

		void addCuttingDate(const Tools::Date cd) { _cuttingDates.push_back(cd); }

		//void applyCutting();

		std::string toString(bool detailed = false) const;

		void setHarvestYields(double primaryYield, double secondaryYield)
		{
			_primaryYield += primaryYield;
			_secondaryYield += secondaryYield;
		}

		void setHarvestYieldsTM(double primaryYieldTM, double secondaryYieldTM)
		{
			_primaryYieldTM += primaryYieldTM;
			_secondaryYieldTM += secondaryYieldTM;
		}


		void setYieldNContent(double primaryYieldN, double secondaryYieldN)
		{
			_primaryYieldN += primaryYieldN;
			_secondaryYieldN += secondaryYieldN;
		}


		void addAppliedIrrigationWater(double amount) { _appliedAmountIrrigation += amount; }
		void setSumTotalNUptake(double sum) { _sumTotalNUptake = sum; }
		void setCropHeight(double height) {_cropHeight = height; }
		void setAccumulatedETa(double eta) { _accumulatedETa = eta; }
		void setAccumulatedTranspiration(double transp) { _accumulatedTranspiration = transp;  }

		double appliedIrrigationWater() const { return _appliedAmountIrrigation; }
		double sumTotalNUptake() const {return _sumTotalNUptake; }
		double primaryYield() const { return _primaryYield * _crossCropAdaptionFactor; }
		double aboveGroundBiomass() const { return _primaryYield * _crossCropAdaptionFactor + _secondaryYield * _crossCropAdaptionFactor; }
		double secondaryYield() const { return _secondaryYield * _crossCropAdaptionFactor; }
		double primaryYieldTM() const {  return _primaryYieldTM * _crossCropAdaptionFactor; }
		double secondaryYieldTM() const { return _secondaryYieldTM * _crossCropAdaptionFactor; }
		double primaryYieldN() const { return _primaryYieldN; }
		double aboveGroundBiomasseN() const { return _primaryYieldN + _secondaryYieldN; }
		double secondaryYieldN() const { return _secondaryYieldN; }
		double cropHeight() const { return _cropHeight; }

		void reset()
		{
			_primaryYield = _secondaryYield = _appliedAmountIrrigation = 0;
			_primaryYieldN = _secondaryYieldN = _accumulatedETa = _accumulatedTranspiration =  0.0;
			_primaryYieldTM = _secondaryYield = 0.0;
			_anthesisDay = _maturityDay = -1;

		}

		void setEva2TypeUsage(int type) { eva2_typeUsage = type; }
		int getEva2TypeUsage() const { return eva2_typeUsage; }

		double get_AccumulatedETa() const {return _accumulatedETa;}
		double get_AccumulatedTranspiration() const { return _accumulatedTranspiration; }

		void writeCropParameters(std::string path);

		void setAnthesisDay(int day) { _anthesisDay = day; }
		int getAnthesisDay() const { return _anthesisDay; }

		void setMaturityDay(int day) { _maturityDay = day; }
		int getMaturityDay() const { return _maturityDay; }
		
		
		// Automatic yiedl trigger parameters
		bool useAutomaticHarvestTrigger() { return _automaticHarvest; }
		void activateAutomaticHarvestTrigger(AutomaticHarvestParameters params) { _automaticHarvest = true; _automaticHarvestParams = params; }
		AutomaticHarvestParameters getAutomaticHarvestParams() { return _automaticHarvestParams; }
		

	private:
		CropId _id;
		std::string _name;
		Tools::Date _seedDate;
		Tools::Date _harvestDate;
		std::vector<Tools::Date> _cuttingDates;
		const CropParameters* _cropParams;
		const CropParameters* _perennialCropParams;
		const OrganicMatterParameters* _residueParams;
		double _primaryYield;
		double _secondaryYield;
		double _primaryYieldTM;
		double _secondaryYieldTM;
		double _appliedAmountIrrigation;
		double _primaryYieldN;
		double _secondaryYieldN;
		double _sumTotalNUptake;

		double _crossCropAdaptionFactor;
		double _cropHeight;
		double _accumulatedETa;
		double _accumulatedTranspiration;

		int eva2_typeUsage;

		int _anthesisDay;
		int _maturityDay;
		
		bool _automaticHarvest;
		AutomaticHarvestParameters _automaticHarvestParams;
	};

	
	//----------------------------------------------------------------------------

	enum FertiliserType { mineral, organic, undefined };

	/**
	 * @brief Parameters for mineral fertiliser.
	 * Simple data structure that holds information about mineral fertiliser.
	 * @author Xenia Holtmann, Dr. Claas Nendel
	 */
	class MineralFertiliserParameters
	{
	public:
		MineralFertiliserParameters();

    MineralFertiliserParameters(json11::Json);

		MineralFertiliserParameters(const std::string& name,
																double carbamid, double no3, double nh4);

		std::string toString() const;

		// Getter ----------------

		/**
		 * @brief Returns name of fertiliser.
		 * @return Name
		 */
		inline std::string getName() const { return name; }

		/**
		 * @brief Returns carbamid part in percentage of fertiliser.
		 * @return Carbamid in percent
		 */
		inline double getCarbamid() const { return vo_Carbamid; }

		/**
		 * @brief Returns ammonium part of fertliser.
		 * @return Ammonium in percent
		 */
		inline double getNH4() const { return vo_NH4; }

		/**
		 * @brief Returns nitrat part of fertiliser
		 * @return Nitrat in percent
		 */
		inline double getNO3() const { return vo_NO3; }

		// Setter ------------------------------------

		/**
		 * @brief Sets name of fertiliser
		 * @param name
		 */
		inline void setName(const std::string& name) { this->name = name; }

		/**
		 * Sets carbamid part of fertilisers
		 * @param vo_Carbamid percent
		 */
		inline void setCarbamid(double vo_Carbamid)
		{
			this->vo_Carbamid = vo_Carbamid;
		}

		/**
		 * @brief Sets nitrat part of fertiliser.
		 * @param vo_NH4
		 */
		inline void setNH4(double vo_NH4) { this->vo_NH4 = vo_NH4; }

		/**
		 * @brief Sets nitrat part of fertiliser.
		 * @param vo_NO3
		 */
		inline void setNO3(double vo_NO3) { this->vo_NO3 = vo_NO3; }

    json11::Json json() const
    {
      return json11::object {
        {"name", name},
        {"Carbamid", vo_Carbamid},
        {"NH4", vo_NH4},
        {"NO3", vo_NO3}};
    }

	private:
		std::string name;
		double vo_Carbamid;
		double vo_NH4;
		double vo_NO3;
	};

	//----------------------------------------------------------------------------

	class MonicaModel;

  enum WorkStepType { eSeed = 0,
                      eHarvest,
                      eCutting,
                      eMineralFertiliserApplication,
                      eOrganicFertiliserApplication,
                      eTillageApplication,
                      eIrrigationApplication
                    };

	class WorkStep
	{
	public:

		WorkStep(const Tools::Date& d) : _date(d) { }

		Tools::Date date() const { return _date; }
		virtual void setDate(Tools::Date date) {_date = date; }

		//! do whatever the workstep has to do
		virtual void apply(MonicaModel* model) = 0;

		virtual std::string toString() const;

		virtual WorkStep* clone() const = 0;

    virtual WorkstepType workstepType() const = 0;

    virtual json11::Json json() const = 0;

	protected:
		Tools::Date _date;
	};

  typedef std::shared_ptr<WorkStep> WSPtr;

	//----------------------------------------------------------------------------

	class Seed : public WorkStep
	{
	public:

		Seed(const Tools::Date& at, CropPtr crop)
      : WorkStep(at),
        _crop(crop) { }

    Seed(json11::Json j, CropPtr crop)
      : WorkStep(Tools::Date::fromIsoDateString(j["date"].string_value())),
        _crop(crop) {}

		virtual void apply(MonicaModel* model);

		void setDate(Tools::Date date)
		{
			this->_date = date;
			_crop.get()->setSeedAndHarvestDate(date, _crop.get()->harvestDate());
		}

		virtual std::string toString() const;

		virtual Seed* clone() const {return new Seed(*this); }

    virtual WorkstepType workstepType() const { return eSeed; }

    virtual json11::Json json() const
    {
      return json11::object {
        {"type", "Seed"},
        {"date", date().toIsoDateString()}};
    }

	private:
		CropPtr _crop;
	};

	//----------------------------------------------------------------------------

	class Harvest : public WorkStep
	{
	public:

		Harvest(const Tools::Date& at, CropPtr crop, PVResultPtr cropResult, std::string method = "total")
			: WorkStep(at), _crop(crop), _cropResult(cropResult), _method(method) { }

    Harvest(json11::Json j, CropPtr crop)
      : WorkStep(Tools::Date::fromIsoDateString(j["date"].string_value())),
        _crop(crop),
        _method(j["method"].string_value()),
        _percentage(j["percentage"].number_value()),
        _exported(j["exported"].bool_value())
    {}

		virtual void apply(MonicaModel* model);

		void setDate(Tools::Date date)
		{
			this->_date = date;
			_crop.get()->setSeedAndHarvestDate(_crop.get()->seedDate(), date);
		}

		void setPercentage(double percentage)
		{
			_percentage = percentage;
		}

		void setExported(bool exported)
		{
			_exported = exported;
		}

		
		virtual std::string toString() const;

		virtual Harvest* clone() const { return new Harvest(*this); }

    virtual WorkstepType workstepType() const { return eHarvest; }

    virtual json11::Json jsonMessage() const
    {
      return json11::object {
        {"type", "Harvest"},
        {"date", date().toIsoDateString()},
        {"method", _method},
        {"percentage", _percentage},
        {"exported", _exported}};
    }

	private:
		CropPtr _crop;
		PVResultPtr _cropResult;
		std::string _method;
		double _percentage;
		bool _exported;
	};

  //----------------------------------------------------------------------------

	class Cutting : public WorkStep
	{
	public:

		Cutting(const Tools::Date& at, CropPtr crop) : WorkStep(at), _crop(crop) { }

    Cutting(json11::Json j, CropPtr crop)
      : WorkStep(Tools::Date::fromIsoDateString(j["date"].string_value())),
        _crop(crop) {}

		virtual void apply(MonicaModel* model);

		virtual std::string toString() const;

		virtual Cutting* clone() const {return new Cutting(*this); }

    virtual WorkstepType workstepType() const { return eCutting; }

    virtual json11::Json jsonMessage() const
    {
      return json11::object {
        {"type", "Cutting"},
        {"date", date().toIsoDateString()}};
    }

	private:
		CropPtr _crop;
	};

	//----------------------------------------------------------------------------

	struct NMinCropParameters
	{
		NMinCropParameters() : samplingDepth(0), nTarget(0), nTarget30(0) { }

		NMinCropParameters(double samplingDepth, double nTarget, double nTarget30)
			: samplingDepth(samplingDepth), nTarget(nTarget), nTarget30(nTarget30) { }

		std::string toString() const;

		double samplingDepth;
		double nTarget;
		double nTarget30;
	};

	//----------------------------------------------------------------------------

	struct NMinUserParameters
	{
		NMinUserParameters() :
			min(0), max(0), delayInDays(0) {}

		NMinUserParameters(double min, double max, int delayInDays)
			: min(min), max(max), delayInDays(delayInDays) { }

		std::string toString() const;

		double min, max;
		int delayInDays;
	};

	//----------------------------------------------------------------------------

	class MineralFertiliserApplication : public WorkStep
	{
	public:
		MineralFertiliserApplication(const Tools::Date& at,
																 MineralFertiliserParameters partition,
																 double amount)
			: WorkStep(at), _partition(partition), _amount(amount) { }

    MineralFertiliserApplication(json11::Json j)
      : WorkStep(Tools::Date::fromIsoDateString(j["date"].string_value())),
        _partition(j["parameters"]),
        _amount(j["amount"].number_value())
    {}

		virtual void apply(MonicaModel* model);

		MineralFertiliserParameters partition() const
		{
			return _partition;
		}

		double amount() const { return _amount; }

		virtual std::string toString() const;

		virtual MineralFertiliserApplication* clone() const {return new MineralFertiliserApplication(*this); }

    virtual WorkstepType workstepType() const { return eMineralFertiliserApplication; }

    virtual json11::Json jsonMessage() const
    {
      return json11::object {
        {"type", "MineralFertiliserApplication"},
        {"date", date().toIsoDateString()},
        {"amount", _amount},
        {"parameters", _partition.jsonMessage()}};
    }

	private:
		MineralFertiliserParameters _partition;
		double _amount;
	};

	//----------------------------------------------------------------------------

	class OrganicFertiliserApplication : public WorkStep
	{
	public:

		OrganicFertiliserApplication(const Tools::Date& at,
																 const OrganicMatterParameters* params,
																 double amount,
																 bool incorp = true)
			: WorkStep(at), _params(params), _amount(amount), _incorporation(incorp)
		{
		}

    OrganicFertiliserApplication(json11::Json j)
      : WorkStep(Tools::Date::fromIsoDateString(j["date"].string_value())),
        _params(j["parameters"]),
        _amount(j["amount"].number_value()),
        _incorporation(j["incorporation"].bool_value()) {}

		virtual void apply(MonicaModel* model);

		//! Returns parameter for organic fertilizer
		const OrganicMatterParameters* parameters() const { return _params; }

		//! Returns fertilization amount
		double amount() const { return _amount; }

		//! Returns TRUE, if fertilizer is applied with incorporation
		bool incorporation() const { return _incorporation; }

		//! Returns a string that contains all parameters. Used for debug outputs.
		virtual std::string toString() const;

		virtual OrganicFertiliserApplication* clone() const {return new OrganicFertiliserApplication(*this); }

    virtual WorkstepType workstepType() const { return eOrganicFertiliserApplication; }

    virtual json11::Json jsonMessage() const
    {
      return json11::object {
        {"type", "OrganicFertiliserApplication"},
        {"date", date().toIsoDateString()},
        {"amount", _amount},
        {"parameters", _params.jsonMessage()},
        {"incorporation", _incorporation}};
    }

	private:
		const OrganicMatterParameters* _params;
		double _amount;
		bool _incorporation;

	};

	//----------------------------------------------------------------------------

	class TillageApplication : public WorkStep
	{
	public:
		TillageApplication(const Tools::Date& at, double depth)
			: WorkStep(at), _depth(depth) { }

		virtual void apply(MonicaModel* model);

		double depth() const { return _depth; }

		virtual std::string toString() const;

		virtual TillageApplication* clone() const {return new TillageApplication(*this); }

    virtual WorkstepType workstepType() const { return eTillageApplication; }
	private:
		double _depth;
	};

	//----------------------------------------------------------------------------

	struct IrrigationParameters
	{
    IrrigationParameters() {}

    IrrigationParameters(double nitrateConcentration,
                         double sulfateConcentration)
      : nitrateConcentration(nitrateConcentration),
        sulfateConcentration(sulfateConcentration)
    {}

    double nitrateConcentration{0};
    double sulfateConcentration{0};

		std::string toString() const;
	};

	//----------------------------------------------------------------------------

	struct AutomaticIrrigationParameters : public IrrigationParameters
	{
    AutomaticIrrigationParameters() {}

    double amount{17.0};
    double treshold{0.35};

		std::string toString() const;
	};

	//----------------------------------------------------------------------------

	class IrrigationApplication : public WorkStep
	{
	public:
    IrrigationApplication(const Tools::Date& at, double amount,
                          IrrigationParameters params = IrrigationParameters())
      : WorkStep(at), _amount(amount), _params(params) {}

		virtual void apply(MonicaModel* model);

		double amount() const { return _amount; }

		double nitrateConcentration() const { return _params.nitrateConcentration; }

		double sulfateConcentration() const { return _params.sulfateConcentration; }

		virtual std::string toString() const;

		virtual IrrigationApplication* clone() const {return new IrrigationApplication(*this); }

    virtual WorkstepType workstepType() const { return eIrrigationApplication; }

	private:
    double _amount{0};
		IrrigationParameters _params;
	};

	//----------------------------------------------------------------------------

	class MeasuredGroundwaterTableInformation
	{
	public:
	    MeasuredGroundwaterTableInformation() : groundwaterInformationAvailable(false)  {}
	    ~MeasuredGroundwaterTableInformation() {}

	    void readInGroundwaterInformation(std::string path);

	    double getGroundwaterInformation(Tools::Date gwDate);

	    bool isGroundwaterInformationAvailable() {return this->groundwaterInformationAvailable; }

	private:
	    bool groundwaterInformationAvailable;
	    std::map<Tools::Date, double> groundwaterInfo;
	};

	//----------------------------------------------------------------------------

	/**
	 * @class PV
	 * @param params
	 * @param id
	 * @param start Start date
	 * @param end End date
	 */
	class ProductionProcess
	{
	public:
		ProductionProcess() { }

		ProductionProcess(const std::string& name, CropPtr crop = CropPtr());

		//ProductionProcess(const ProductionProcess&);

		ProductionProcess deepCloneAndClearWorksteps() const;

		template<class Application>
		void addApplication(const Application& a)
		{
			_worksteps.insert(std::make_pair(a.date(), WSPtr(new Application(a))));
		};

		void addApplication(WSPtr a)
		{
			_worksteps.insert(std::make_pair((a.get())->date(), a));
		}

		void apply(const Tools::Date& date, MonicaModel* model) const;

		Tools::Date nextDate(const Tools::Date & date) const;

		std::string name() const { return _name; }

		CropPtr crop() const { return _crop; }

		bool isFallow() const { return !_crop->isValid();  }

		//! when does the PV start
		Tools::Date start() const;

		//! when does the whole PV end
		Tools::Date end() const;

		std::multimap<Tools::Date, WSPtr> getWorksteps() {return _worksteps; }

		void clearWorksteps() { _worksteps.clear(); }

		std::string toString() const;

		PVResult cropResult() const { return *(_cropResult.get()); }
		PVResultPtr cropResultPtr() const { return _cropResult; }

		//the custom id is used to keep a potentially usage defined
    //mapping to an entity from another domain,
    //e.g. a Carbiocial CropActivity which the ProductionProcess was based on
		void setCustomId(int cid) { _customId = cid; }
		int customId() const { return _customId; }

	private:
		int _customId;
		std::string _name;
		CropPtr _crop;

		//!ordered list of worksteps to be done for this PV
		std::multimap<Tools::Date, WSPtr> _worksteps;

		//store results of the productionprocess
		PVResultPtr _cropResult;
	};

	MineralFertiliserParameters
	getMineralFertiliserParametersFromMonicaDB(int mineralFertiliserId);

	void attachFertiliserApplicationsToCropRotation
	(std::vector<ProductionProcess>& cropRotation,
	 const std::string& pathToFertiliserFile);

	std::vector<ProductionProcess>
	attachFertiliserSA(std::vector<ProductionProcess> cropRotation,
										 const std::string pathToFertiliserFile);

	void attachIrrigationApplicationsToCropRotation
	(std::vector<ProductionProcess>& cropRotation,
	 const std::string& pathToIrrigationFile);

	/*!
	 * - create cropRotation from hermes file
	 * - the returned production processes contain absolute dates
	 */
	std::vector<ProductionProcess>
		cropRotationFromHermesFile(const std::string& pathToFile, bool useAutomaticHarvestTrigger = false, AutomaticHarvestParameters autoHarvestParameters=AutomaticHarvestParameters());

	Climate::DataAccessor climateDataFromHermesFiles(const std::string& pathToFile,
																									 int fromYear, int toYear,
																									 const CentralParameterProvider& cpp,
																									 bool useLeapYears = true, double latitude = 51.2);

	//----------------------------------------------------------------------------

	/**
	 * @brief Parameters for organic fertiliser
	 */
	class OrganicMatterParameters
	{
	public:
		OrganicMatterParameters();
    OrganicMatterParameters(json11::Json);
		OrganicMatterParameters(const OrganicMatterParameters&);

		~OrganicMatterParameters() { }

		std::string toString() const;

    json11::Json json() const
    {
      return json11::object {
        {"name", name},
        {"AOM_DryMatterContent", vo_AOM_DryMatterContent},
        {"AOM_AOM_NH4Content", vo_AOM_NH4Content},
        {"AOM_AOM_NO3Content", vo_AOM_NO3Content},
        {"AOM_AOM_NO3Content", vo_AOM_NO3Content},
        {"AOM_AOM_SlowDecCoeffStandard", vo_AOM_SlowDecCoeffStandard},
        {"AOM_AOM_FastDecCoeffStandard", vo_AOM_FastDecCoeffStandard},
        {"AOM_PartAOM_to_AOM_Slow", vo_PartAOM_to_AOM_Slow},
        {"AOM_PartAOM_to_AOM_Fast", vo_PartAOM_to_AOM_Fast},
        {"AOM_CN_Ratio_AOM_Slow", vo_CN_Ratio_AOM_Slow},
        {"AOM_CN_Ratio_AOM_Fast", vo_CN_Ratio_AOM_Fast},
        {"AOM_PartAOM_Slow_to_SMB_Slow", vo_PartAOM_Slow_to_SMB_Slow},
        {"AOM_PartAOM_Slow_to_SMB_Fast", vo_PartAOM_Slow_to_SMB_Fast},
        {"AOM_NConcentration", vo_NConcentration}};
    }

		std::string name;

		double vo_AOM_DryMatterContent;   //! Dry matter content of added organic matter [kg DM kg FM-1]
		double vo_AOM_NH4Content;         //! Ammonium content in added organic matter [kg N kg DM-1]
		double vo_AOM_NO3Content;         //! Nitrate content in added organic matter [kg N kg DM-1]
		double vo_AOM_CarbamidContent;    //! Carbamide content in added organic matter [kg N kg DM-1]

		double vo_AOM_SlowDecCoeffStandard; //! Decomposition rate coefficient of slow AOM at standard conditions [d-1]
		double vo_AOM_FastDecCoeffStandard; //! Decomposition rate coefficient of fast AOM at standard conditions [d-1]

		double vo_PartAOM_to_AOM_Slow;    //! Part of AOM that is assigned to the slowly decomposing pool [kg kg-1
		double vo_PartAOM_to_AOM_Fast;    //! Part of AOM that is assigned to the rapidly decomposing pool [kg kg-1]

		double vo_CN_Ratio_AOM_Slow;    //! C to N ratio of the slowly decomposing AOM pool []
		double vo_CN_Ratio_AOM_Fast;    //! C to N ratio of the rapidly decomposing AOM pool []

		double vo_PartAOM_Slow_to_SMB_Slow;   //! Part of AOM slow consumed by slow soil microbial biomass [kg kg-1]
		double vo_PartAOM_Slow_to_SMB_Fast;   //! Part of AOM slow consumed by fast soil microbial biomass [kg kg-1]

		double vo_NConcentration;
	};

	typedef OrganicMatterParameters OMP;
  typedef std::shared_ptr<OMP> OMPPtr;

	OrganicMatterParameters*
	getOrganicFertiliserParametersFromMonicaDB(int organ_fert_id);

	const OrganicMatterParameters* getResidueParametersFromMonicaDB(int crop_id);

	//----------------------------------------------------------------------------

	/**
	 * Class that holds information of crop defined by user.
	 * @author Xenia Specka
	 */
	class UserCropParameters
	{
	public:
		UserCropParameters() {}
		virtual ~UserCropParameters() {}

		double pc_CanopyReflectionCoefficient;
		double pc_ReferenceMaxAssimilationRate;
		double pc_ReferenceLeafAreaIndex;
		double pc_MaintenanceRespirationParameter1;
		double pc_MaintenanceRespirationParameter2;
		double pc_MinimumNConcentrationRoot;
		double pc_MinimumAvailableN;
		double pc_ReferenceAlbedo;
		double pc_StomataConductanceAlpha;
		double pc_SaturationBeta;
		double pc_GrowthRespirationRedux;
		double pc_MaxCropNDemand;
		double pc_GrowthRespirationParameter1;
		double pc_GrowthRespirationParameter2;
		double pc_Tortuosity;
	};

	//----------------------------------------------------------------------------

	/**
	 * Class that holds information about user defined environment parameters.
	 * @author Xenia Specka
	 */
	class UserEnvironmentParameters
	{
	public:
		UserEnvironmentParameters() :
			p_MaxGroundwaterDepth(20),
			p_MinGroundwaterDepth(20)
		{}

		virtual ~UserEnvironmentParameters() {}

		bool p_UseAutomaticIrrigation;
		bool p_UseNMinMineralFertilisingMethod;
		bool p_UseSecondaryYields;
		bool p_UseAutomaticHarvestTrigger;

		double p_LayerThickness;
		double p_Albedo;
		double p_AthmosphericCO2;
		double p_WindSpeedHeight;
		double p_LeachingDepth;
		double p_timeStep;
		double p_MaxGroundwaterDepth;
		double p_MinGroundwaterDepth;

		int p_NumberOfLayers;
		int p_StartPVIndex;
		int p_JulianDayAutomaticFertilising;
		int p_MinGroundwaterDepthMonth;
	};

	class UserInitialValues
	{
	public:
		UserInitialValues() :
			p_initPercentageFC(0.8),
			p_initSoilNitrate(0.0001),
			p_initSoilAmmonium(0.0001)
		{}

		virtual ~UserInitialValues() {}

		double p_initPercentageFC;    // Initial soil moisture content in percent field capacity
		double p_initSoilNitrate;     // Initial soil nitrate content [kg NO3-N m-3]
		double p_initSoilAmmonium;    // Initial soil ammonium content [kg NH4-N m-3]
	};

	//----------------------------------------------------------------------------

	/**
	 * Class that holds information about user defined soil moisture parameters.
	 * @author Xenia Specka
	 */
	class UserSoilMoistureParameters
	{
	public:
		UserSoilMoistureParameters() {}
		virtual ~UserSoilMoistureParameters() {}

		double pm_CriticalMoistureDepth;
		double pm_SaturatedHydraulicConductivity;
		double pm_SurfaceRoughness;
		double pm_GroundwaterDischarge;
		double pm_HydraulicConductivityRedux;
		double pm_SnowAccumulationTresholdTemperature;
		double pm_KcFactor;
		double pm_TemperatureLimitForLiquidWater;
		double pm_CorrectionSnow;
		double pm_CorrectionRain;
		double pm_SnowMaxAdditionalDensity;
		double pm_NewSnowDensityMin;
		double pm_SnowRetentionCapacityMin;
		double pm_RefreezeParameter1;
		double pm_RefreezeParameter2;
		double pm_RefreezeTemperature;
		double pm_SnowMeltTemperature;
		double pm_SnowPacking;
		double pm_SnowRetentionCapacityMax;
		double pm_EvaporationZeta;
		double pm_XSACriticalSoilMoisture;
		double pm_MaximumEvaporationImpactDepth;
		double pm_MaxPercolationRate;
		double pm_MoistureInitValue;
	};

	//----------------------------------------------------------------------------

	/**
	 * Class that holds information about user defined soil temperature parameters.
	 * @author Xenia Specka
	 */
	class UserSoilTemperatureParameters
	{
	public:
		UserSoilTemperatureParameters() {pt_SoilMoisture = 0.25;}
		virtual ~UserSoilTemperatureParameters() {}

		double pt_NTau;
		double pt_InitialSurfaceTemperature;
		double pt_BaseTemperature;
		double pt_QuartzRawDensity;
		double pt_DensityAir;
		double pt_DensityWater;
		double pt_DensityHumus;
		double pt_SpecificHeatCapacityAir;
		double pt_SpecificHeatCapacityQuartz;
		double pt_SpecificHeatCapacityWater;
		double pt_SpecificHeatCapacityHumus;
		double pt_SoilAlbedo;
		double pt_SoilMoisture;
	};

	//----------------------------------------------------------------------------

	/**
	 * Class that holds information about user defined soil transport parameters.
	 * @author Xenia Specka
	 */
	class UserSoilTransportParameters
	{
	public:
		UserSoilTransportParameters() {}
		~UserSoilTransportParameters() {}

		double pq_DispersionLength;
		double pq_AD;
		double pq_DiffusionCoefficientStandard;
		double pq_NDeposition;

	};

	//----------------------------------------------------------------------------

	/**
	 * Class that holds information about user-defined soil organic parameters.
	 * @author Claas Nendel
	 */
	class UserSoilOrganicParameters
	{
	public:
		UserSoilOrganicParameters() {}
		virtual ~UserSoilOrganicParameters() {}


		double po_SOM_SlowDecCoeffStandard; //4.30e-5 [d-1], Bruun et al. 2003 4.3e-5
		double po_SOM_FastDecCoeffStandard; //1.40e-4 [d-1], from DAISY manual 1.4e-4
		double po_SMB_SlowMaintRateStandard; //1.00e-3 [d-1], from DAISY manual original 1.8e-3
		double po_SMB_FastMaintRateStandard; //1.00e-2 [d-1], from DAISY manual
		double po_SMB_SlowDeathRateStandard; //1.00e-3 [d-1], from DAISY manual
		double po_SMB_FastDeathRateStandard; //1.00e-2 [d-1], from DAISY manual
		double po_SMB_UtilizationEfficiency; //0.60 [], from DAISY manual 0.6
		double po_SOM_SlowUtilizationEfficiency; //0.40 [], from DAISY manual 0.4
		double po_SOM_FastUtilizationEfficiency; //0.50 [], from DAISY manual 0.5
		double po_AOM_SlowUtilizationEfficiency; //0.40 [], from DAISY manual original 0.13
		double po_AOM_FastUtilizationEfficiency; //0.10 [], from DAISY manual original 0.69
		double po_AOM_FastMaxC_to_N; // 1000.0
		double po_PartSOM_Fast_to_SOM_Slow; //0.30) [], Bruun et al. 2003
		double po_PartSMB_Slow_to_SOM_Fast; //0.60) [], from DAISY manual
		double po_PartSMB_Fast_to_SOM_Fast; //0.60 [], from DAISY manual
		double po_PartSOM_to_SMB_Slow; //0.0150 [], optimised
		double po_PartSOM_to_SMB_Fast; //0.0002 [], optimised
		double po_CN_Ratio_SMB; //6.70 [], from DAISY manual
		double po_LimitClayEffect; //0.25 [kg kg-1], from DAISY manual
		double po_AmmoniaOxidationRateCoeffStandard; //1.0e-1[d-1], from DAISY manual
		double po_NitriteOxidationRateCoeffStandard; //9.0e-1[d-1], fudged by Florian Stange
		double po_TransportRateCoeff; //0.1 [d-1], from DAISY manual
		double po_SpecAnaerobDenitrification; //0.1 //[g gas-N g CO2-C-1]
		double po_ImmobilisationRateCoeffNO3; //0.5 //[d-1]
		double po_ImmobilisationRateCoeffNH4; //0.5 //[d-1]
		double po_Denit1; //0.2 Denitrification parameter
		double po_Denit2; //0.8 Denitrification parameter
		double po_Denit3; //0.9 Denitrification parameter
		double po_HydrolysisKM; //0.00334 from Tabatabai 1973
		double po_ActivationEnergy; //41000.0 from Gould et al. 1973
		double po_HydrolysisP1; //4.259e-12 from Sadeghi et al. 1988
		double po_HydrolysisP2; //1.408e-12 from Sadeghi et al. 1988
		double po_AtmosphericResistance; //0.0025 [s m-1], from Sadeghi et al. 1988
		double po_N2OProductionRate; //0.5 [d-1]
		double po_Inhibitor_NH3; //1.0 [kg N m-3] NH3-induced inhibitor for nitrite oxidation


	};

	//----------------------------------------------------------------------------

	class SensitivityAnalysisParameters
	{
	public:
		SensitivityAnalysisParameters()
			: p_MeanFieldCapacity(UNDEFINED),
				p_MeanBulkDensity(UNDEFINED),
				p_HeatConductivityFrozen(UNDEFINED),
				p_HeatConductivityUnfrozen(UNDEFINED),
				p_LatentHeatTransfer(UNDEFINED),
				p_ReducedHydraulicConductivity(UNDEFINED),
				vs_FieldCapacity(UNDEFINED),
				vs_Saturation(UNDEFINED),
				vs_PermanentWiltingPoint(UNDEFINED),
				vs_SoilMoisture(UNDEFINED),
				vs_SoilTemperature(UNDEFINED),
				vc_SoilCoverage(UNDEFINED),
				vc_MaxRootingDepth(UNDEFINED),
				vc_RootDiameter(UNDEFINED),
				sa_crop_id(-1)
		{
			crop_parameters.pc_InitialKcFactor = UNDEFINED;
			crop_parameters.pc_StageAtMaxHeight = UNDEFINED;
			crop_parameters.pc_CropHeightP1 = UNDEFINED;
			crop_parameters.pc_CropHeightP2 = UNDEFINED;
			crop_parameters.pc_LuxuryNCoeff = UNDEFINED;
			crop_parameters.pc_ResidueNRatio = UNDEFINED;
			crop_parameters.pc_CropSpecificMaxRootingDepth = UNDEFINED;
			crop_parameters.pc_RootPenetrationRate = UNDEFINED;
			crop_parameters.pc_RootGrowthLag = UNDEFINED;
			crop_parameters.pc_InitialRootingDepth = UNDEFINED;
			crop_parameters.pc_RootFormFactor = UNDEFINED;
			crop_parameters.pc_MaxNUptakeParam = UNDEFINED;
			crop_parameters.pc_CarboxylationPathway = UNDEFINED_INT;
			crop_parameters.pc_MaxAssimilationRate = UNDEFINED;
			crop_parameters.pc_MaxCropDiameter = UNDEFINED;
			crop_parameters.pc_MinimumNConcentration = UNDEFINED;
			crop_parameters.pc_NConcentrationB0 = UNDEFINED;
			crop_parameters.pc_NConcentrationPN =  UNDEFINED;
			crop_parameters.pc_NConcentrationRoot = UNDEFINED;
			crop_parameters.pc_PlantDensity = UNDEFINED;
			crop_parameters.pc_ResidueNRatio = UNDEFINED;

			organic_matter_parameters.vo_AOM_DryMatterContent = UNDEFINED;
			organic_matter_parameters.vo_AOM_NH4Content = UNDEFINED;
			organic_matter_parameters.vo_AOM_NO3Content = UNDEFINED;
			organic_matter_parameters.vo_AOM_CarbamidContent = UNDEFINED;
			organic_matter_parameters.vo_PartAOM_to_AOM_Slow = UNDEFINED;
			organic_matter_parameters.vo_PartAOM_to_AOM_Fast = UNDEFINED;
			organic_matter_parameters.vo_CN_Ratio_AOM_Slow = UNDEFINED;
			organic_matter_parameters.vo_CN_Ratio_AOM_Fast = UNDEFINED;
		}

		~SensitivityAnalysisParameters() {}

		// soilmoisture module parameters
		double p_MeanFieldCapacity;
		double p_MeanBulkDensity;
		double p_HeatConductivityFrozen;
		double p_HeatConductivityUnfrozen;
		double p_LatentHeatTransfer;
		double p_ReducedHydraulicConductivity;
		double vs_FieldCapacity;
		double vs_Saturation;
		double vs_PermanentWiltingPoint;
		double vs_SoilMoisture;
		double vs_SoilTemperature;


		// crop parameters
		double vc_SoilCoverage;
		double vc_MaxRootingDepth;
		double vc_RootDiameter;

		CropParameters crop_parameters;
		OrganicMatterParameters organic_matter_parameters;
		int sa_crop_id;
	};

	//----------------------------------------------------------------------------

	/**
	 * @brief Central data distribution class.
	 *
	 * Class that holds pointers and direct information of user defined parameters.
	 *
	 * @author Xenia Specka
	 */
	class CentralParameterProvider
	{
	public:
		CentralParameterProvider();
		CentralParameterProvider(const CentralParameterProvider&);
		virtual ~CentralParameterProvider() {}

		UserCropParameters userCropParameters;
		UserEnvironmentParameters userEnvironmentParameters;
		UserSoilMoistureParameters userSoilMoistureParameters;
		UserSoilTemperatureParameters userSoilTemperatureParameters;
		UserSoilTransportParameters userSoilTransportParameters;
		UserSoilOrganicParameters userSoilOrganicParameters;
		SensitivityAnalysisParameters sensitivityAnalysisParameters;
		UserInitialValues userInitValues;

    Soil::CapillaryRiseRates capillaryRiseRates;

		double getPrecipCorrectionValue(int month) const;
		void setPrecipCorrectionValue(int month, double value);

		bool writeOutputFiles;

	private:
		double precipCorrectionValues[12];
	};

	//----------------------------------------------------------------------------

	CentralParameterProvider readUserParameterFromDatabase(int type = 0);

	void testClimateData(Climate::DataAccessor &climateData);


	CropPtr hermesCropId2Crop(const std::string& hermesCropId);

  //----------------------------------------------------------------------------

  const std::map<int, std::string>& availableMonicaCrops();
}

#endif
