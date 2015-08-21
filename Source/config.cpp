/**********************************************************************************************************************************************/
#include "config.h"
#include "globals.h"
#include "version.h"
/**********************************************************************************************************************************************/
/*
This holds all the functions related to the config file, its loading, making, and holding the values pulled from the config.
*/
/**********************************************************************************************************************************************/
clsConfig::clsConfig() {
	//Set the values as some default value.
	values.blnLogging = true;
	values.blnShowMap = false;
	values.blnAppendTime = true;
	values.blnHardMode = false;
	values.uintFirstGen = 100;
	values.uintGenIncrease = 100;
	values.uintGensPastGrowth = 10;
	values.uintMutationChance = 15;
	values.uintSeed = 12345;
	if (Global::blnDebugMode) {printf("Config Constructor called.\n");}
}
/**********************************************************************************************************************************************/
clsConfig::~clsConfig() {
	if(Global::blnDebugMode) {printf("Config Destructor called.\n");}
}
/**********************************************************************************************************************************************/
bool clsConfig::exists(void) {
	//Returns true or false if config file exists

    /* TODO (GamerMan7799#1#): Replace this with a method that doesn't need the fstream library. ...
    So we can reduce the libraries that we need.*/

	std::ifstream infile(FileName);
	return infile.good();
}
/**********************************************************************************************************************************************/
void clsConfig::make(void) {
	//Makes the config file
	configFile = fopen(FileName,"w");
	printf("Config File will now be created!\n");

	fprintf(configFile,"Config File for the program.\n");
	fprintf(configFile,"%s\n",DEFINED_VER_FULLVERSION_STRING);

	fprintf(configFile,"First Generation Steps: 100\n");
	fprintf(configFile,"Generation Increase: 100\n");
	fprintf(configFile,"Gens Past Growth: 10\n");
	fprintf(configFile,"Percent Mutation Chance: 15\n");
	fprintf(configFile,"Log to File: 1\n");
	fprintf(configFile,"Hard mode: 0\n");
	fprintf(configFile,"Show map on update: 0\n");
	fprintf(configFile,"Random Seed: [12345]\n");
	fprintf(configFile,"Append Time: 1\n");
	fclose(configFile);

	//These are just the default values I use when testing the program.
	values.blnLogging = true;
	values.blnShowMap = false;
	values.blnAppendTime = true;
	values.blnHardMode = false;
	values.uintFirstGen = 100;
	values.uintGenIncrease = 100;
	values.uintGensPastGrowth = 10;
	values.uintMutationChance = 15;
	values.uintSeed = 12345;
}
/**********************************************************************************************************************************************/
char clsConfig::verisonCheck(const char *ConfigVerison) {
	//This checks the version number written at the top of the config file
	//against the internal version number of the program.
	//If it finds a Major revision change the config HAS to be replaced.
	//A Minor revision will result in a prompt to the user about if it should be replaced.
	//And if only a patch change is found then it will just use the old config
	//Lastly if no change is found then use the config of course
	
	// TODO (GamerMan7799#6#) : Allow the check to also check program status (beta / alpha / rc)
	uint C_MajorNum, C_MinorNum, C_PatchNum;
	sscanf(ConfigVerison,"%u.%u.%u",&C_MajorNum,&C_MinorNum,&C_PatchNum);
	if (Global::blnDebugMode) {printf("Config: v %u %u %u \n",C_MajorNum,C_MinorNum,C_PatchNum);}
	if (DEFINED_VER_MAJOR != C_MajorNum) {return NEWCONFIG;}
	else if (DEFINED_VER_MINOR != C_MinorNum) {return PROMPTUSER;}
	else {return USECONFIG;}
}
/**********************************************************************************************************************************************/
void clsConfig::load(void) {
	//Loads all of the config values

	char chrTempString[50];
	int intTempBool, intValuesScanned;
	//Get the First Generation Input from config
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %d",&values.uintFirstGen);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintFirstGen = 100;}
	if(Global::blnDebugMode) {printf("First Gen Steps \t \t %2d\n",values.uintFirstGen);}

	//Get Amount the inputs increase by each generation
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %d",&values.uintGenIncrease);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintGenIncrease = 100;}
	if(Global::blnDebugMode) {printf("Generation Increase \t \t %2d\n",values.uintGenIncrease);}

	//Get the number of generations past growth
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&values.uintGensPastGrowth);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintGensPastGrowth = 10;}
	if(Global::blnDebugMode) {printf("Gens Past Growth \t \t %u\n",values.uintGensPastGrowth);}

	//Get the percent chance of mutation
	//Should I make this a float?
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %u", &values.uintMutationChance);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintMutationChance = 15;}
	if(Global::blnDebugMode) {printf("Percent Mutation Chance \t %u\n",values.uintMutationChance);}

	//Get if there is logging
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d", &intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
	if(Global::blnDebugMode) {printf("Log to file \t \t \t %d\n",intTempBool);}
	if(intTempBool == 1) {values.blnLogging = true;}
	else {values.blnLogging = false;}

	//Check if hard mode is enabled.
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
	if(Global::blnDebugMode) {printf("Hard mode \t \t \t %u\n",intTempBool);}
	if(intTempBool == 1) {values.blnHardMode = true;}
	else {values.blnHardMode = false;}

	//Check if show map is enabled
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
	if(Global::blnDebugMode) {printf("Show Map Update \t \t %d\n",intTempBool);}
	if(intTempBool == 1) {values.blnShowMap = true;}
	else {values.blnShowMap = false;}

	//Get the seed used in the config
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s [%u]", &values.uintSeed);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintSeed = 0;}
	if(Global::blnDebugMode) {printf("Random Seed \t \t \t %d\n",values.uintSeed);}

	//Check if append time
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
	if(Global::blnDebugMode) {printf("Append Time \t \t \t %u\n",intTempBool);}
	if(intTempBool == 1) {values.blnAppendTime = true;}
	else {values.blnAppendTime = false;}

	fclose(configFile);
	printf("\n\n");
}
/**********************************************************************************************************************************************/
void clsConfig::Check(void) {
	char chrTempString[50], chrConfigVerison;

	if (exists() != true) {
		printf("Config file was not found; creating now one\n");
		make();
	} else {
		configFile = fopen(DEFINED_CONFIG_FILE_NAME,"r");
		printf("Config file found; loading values\n");
		fgets(chrTempString,50,configFile);
		fgets(chrTempString,50,configFile);
		chrConfigVerison = verisonCheck(chrTempString);

		if (chrConfigVerison == NEWCONFIG) {
			printf("Current config file out of date. Making new one.\n");
			fclose(configFile);
			make();
		}
		else if (chrConfigVerison == PROMPTUSER) {
			printf("\nThe config file you are using has a different Minor Version than the program.\n");
			printf("The config file should in theory still work with this version but I can't say for sure.\n");
			printf("Would you like to replace the config file with a new one?\n");
			do {
				printf("Y or N\n> ");
				scanf("%c",&chrConfigVerison);
				switch (chrConfigVerison)
				{
					case 'Y' :
					case 'y' :
						//Replace the config file
						fclose(configFile);
						make();
						break;
					case 'n' :
					case 'N' :
						//Load the config file
						load();
						break;
					default :
						printf("\nUnknown answer; try again.\n");
						break;
				}; //end switch
			} while (chrConfigVerison != 'n' || chrConfigVerison != 'N' || chrConfigVerison != 'Y' || chrConfigVerison != 'y');
		} else { load();}
	} //end if exists
}
/**********************************************************************************************************************************************/
Configures clsConfig::getvalues(void) {
	return values;
}
/**********************************************************************************************************************************************/
uint clsConfig::getvalues(uchar Spot) {
	//Returns just one value from the config.
	//Useful when I don't need ALL the values.
	switch (Spot) {
		case cnfgLogging :
			if (values.blnLogging) {return 1;}
			else {return 0;}
			break;
		case cnfgShowMap :
			if (values.blnShowMap) {return 1;}
			else {return 0;}
			break;
		case cnfgAppendTime :
			if (values.blnAppendTime) {return 1;}
			else {return 0;}
			break;
		case cnfgHardMode :
			if (values.blnHardMode) {return 1;}
			else {return 0;}
			break;
		case cnfgFirstGen :
			return values.uintFirstGen;
			break;
		case cnfgGenIncrease :
			return values.uintGenIncrease;
			break;
		case cnfgGensPastGrowth :
			return values.uintGensPastGrowth;
			break;
		case cnfgMutationChance :
			return values.uintMutationChance;
			break;
		case cnfgSeed :
			return values.uintSeed;
			break;
		default :
			return 0;
			break;
	}; //end switch

	return 9999;
}
/**********************************************************************************************************************************************/
