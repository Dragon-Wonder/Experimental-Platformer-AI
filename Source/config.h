#ifndef __CONFIG__HEADER__
#define __CONFIG__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <cstdio>
#include <fstream> //This is only needed to see if config file exists. I should try to find a different method that doesn't use this.
/**********************************************************************************************************************************************/
struct ConfigValues {
	bool blnLogging; //If player direction will be logged to a file
	bool blnShowMap; //if the console shows all the steps the players take
	bool blnAppendTime; //If time is added to the seed 
	bool blnHardMode; //if hard mode is on. Hard mode basically makes the players die a lot more.
	uint uintFirstGen; //How many inputs the first generation has
	uint uintGenIncrease; //How many inputs that each generation increases by until the limit of DEFINED_MAX_PLAYER_STEPS
	uint uintGensPastGrowth; //How many generations take place with DEFINED_MAX_PLAYER_STEPS
	uint uintMutationChance; //Percent chance that when using directions from a previous player, the direction will be replace with a random one.
	uint uintSeed; //The seed that is used with srand()
};

enum configValueSpot {
	cnfgLogging = 0,
	cnfgShowMap, //1
	cnfgAppendTime, //2
	cnfgHardMode, //3
	cnfgFirstGen, //4
	cnfgGenIncrease, //5
	cnfgGensPastGrowth, //6
	cnfgMutationChance, //7
	cnfgSeed //8
};

typedef struct ConfigValues Configures;
/**********************************************************************************************************************************************/
class clsConfig {
	public:
	//private:
		//Members
		const char* FileName = DEFINED_CONFIG_FILE_NAME;
		FILE* configFile;
		Configures values;
		
		//Functions
		char verisonCheck(const char *ConfigVerison);
		bool exists(void);
		void make(void);
		void load(void);
		
	//public:
		//Default Constructor
		clsConfig();
		~clsConfig();
		
		//Functions
		void Check(void);
		Configures getvalues(void); //Get all values
		uint getvalues(uchar); //Get just one value (use enum above to define which one).
};
/**********************************************************************************************************************************************/
#endif
