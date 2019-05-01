#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/*****************************************************************************/
#include <cstdio>
/*****************************************************************************/
#include "../globals.h"
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
  bool blnHumanBuild;         /**< If a human player is using the game instead of
                                    an ai. */
	uint uintFirstGen;          /**< How many inputs the first generation has. */
	uint uintGenIncrease;       /**< How many inputs that each generation increases by
                                     until the limit of defined::kMaxPlayerSteps. */
	uint uintGensPastGrowth;    /**< How many generations take place with
                                     defined::kMaxPlayerSteps. */
	uint uintMutationChance;    /**< Percent chance that when using directions from
                                     a previous player, the direction will be replace
                                     with a random one. */
	uint uintSeed;              /**< The seed that is used with srand(). */
	uint uintScreenWidth;       /**< Screen width, only used if specifically called. */
	uint uintScreenHeight;      /**< Screen height, only used if specifically called. */
	uint uintMaxPlayerSteps;    /**< Max steps player can take */
	uint uintPlayersPerGen;     /**< Number of players in each generation */
	uint uintNumberOfBreed;     /**< Number of players for each generation to breed in the next. */
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
	cnfgScreenHeight,   /**< 10: Spot for screen height value. */
	cnfgMaxSteps,       /**< 11: Spot for max steps. */
	cnfgPlayerGen,      /**< 12: Spot for players per generation. */
	cnfgPlayerBreed,    /**< 13: Spot for players to breed. */
	cnfgHumanBuild      /**< 14: Spot for if human build instead of ai. */
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
    clsConfig();

		static constexpr char* FileName = defined::kConfigFileName; /**< The config file name. */
		FILE* configFile; /**< Pointer to the config file open in memory. */
		Configures values; /**< All the config values together. */

    Configures getvalues(void); //Get all values
		uint getvalues(uchar); //Get just one value (use enum above to define which one).
		void Check(void);

  private:
		char verisonCheck(const char *ConfigVerison);
		bool exists(void);
		void make(void);
		void load(void);
};
/*****************************************************************************/
#endif
