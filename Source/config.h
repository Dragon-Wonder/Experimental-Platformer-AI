#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/*****************************************************************************/
#include <cstdio>
/*****************************************************************************/
#include "main.h"
/*****************************************************************************/
/** @struct ConfigValues
    This Structure holds all of the different config values together. */
struct ConfigValues {
	bool blnLogging;            /**< If player direction will be logged to a file. */
	bool blnShowMap;            /**< If the console shows all the steps the
                                     players take. */
	bool blnAppendTime;         /**< If time is added to the seed. */
	bool blnHardMode;           /**< If hard mode is on. Hard mode basically
                                     makes the players die a lot more. */
	uint uintFirstGen;          /**< How many inputs the first generation has. */
	uint uintGenIncrease;       /**< How many inputs that each generation increases by
                                     until the limit of DEFINED_MAX_PLAYER_STEPS. */
	uint uintGensPastGrowth;    /**< How many generations take place with
                                     DEFINED_MAX_PLAYER_STEPS. */
	uint uintMutationChance;    /**< Percent chance that when using directions from
                                     a previous player, the direction will be replace
                                     with a random one. */
	uint uintSeed;              /**< The seed that is used with srand(). */
	uint uintScreenWidth;       /**< Screen width, only used if specifically called. */
	uint uintScreenHeight;      /**< Screen height, only used if specifically called. */
};

/** @enum configValueSpot
  * Holds all of the spots for the config values that can be used to return just one config value.
  * See clsConfig::getvalues
  */
enum configValueSpot {
	cnfgLogging = 0,    /**< 0: Spot for logging value. */
	cnfgShowMap,        /**< 1: Spot for show map value. */
	cnfgAppendTime,     /**< 2: Spot for append time value. */
	cnfgHardMode,       /**< 3: Spot for hard mode value. */
	cnfgFirstGen,       /**< 4: Spot for first gen value. */
	cnfgGenIncrease,    /**< 5: Spot for gen increase value. */
	cnfgGensPastGrowth, /**< 6: Spot for gens past growth value. */
	cnfgMutationChance, /**< 7: Spot for mutation chance value. */
	cnfgSeed,           /**< 8: Spot for seed value. */
	cnfgScreenWidth,    /**< 9: Spot for screen width value. */
	cnfgScreenHeight    /**< 10: Spot for screen height value. */
};

typedef struct ConfigValues Configures;
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsConfig config.h "source/config.h"
/// @brief This class will hold all of the values and functions related to the
///        config file.
/////////////////////////////////////////////////

class clsConfig {
	public:
	//private:
		//Members
		/** The config file name. */
		const char* FileName = DEFINED_CONFIG_FILE_NAME;

		/** Pointer to the config file open in memory. */
		FILE* configFile;

		/** All the config values together. */
		Configures values;

		//Functions
		char verisonCheck(const char *ConfigVerison);
		bool exists(void);
		void make(void);
		void load(void);

	//public:
		//Default Constructor
		clsConfig();
		//Functions
		void Check(void);
		Configures getvalues(void); //Get all values
		uint getvalues(uchar); //Get just one value (use enum above to define which one).
};
/*****************************************************************************/
#endif
