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
	values.blnShowMap = true;
	values.blnAppendTime = true;
	values.blnHardMode = false;
	values.uintFirstGen = 100;
	values.uintGenIncrease = 100;
	values.uintGensPastGrowth = 10;
	values.uintMutationChance = 15;
	values.uintSeed = 12345;
	values.uintScreenHeight = 0;
	values.uintScreenWidth = 0;
	if (Global::blnDebugMode) {printf("Config Constructor called.\n");}
}
/**********************************************************************************************************************************************/
clsConfig::~clsConfig() {
	if(Global::blnDebugMode) {printf("Config Destructor called.\n");}
}
/**********************************************************************************************************************************************/
bool clsConfig::exists(void) {
	//Returns true or false if config file exists
	FILE* pTempFile = fopen(FileName, "r");
	if (pTempFile == NULL) {return false;}
	else {return true;}
}
/**********************************************************************************************************************************************/
void clsConfig::make(void) {
	//Makes the config file
	configFile = fopen(FileName,"w");
	printf("Config File will now be created!\n");

	fprintf(configFile,"Config File for the program.\n");
	fprintf(configFile,"%s\n",DEFINED_VER_FULLVERSION_STRING);

    //Write the config with the default values defined when the object was created.
	fprintf(configFile,"First Generation Steps: %u\n",values.uintFirstGen);
	fprintf(configFile,"Generation Increase: %u\n", values.uintGenIncrease);
	fprintf(configFile,"Gens Past Growth: %u\n", values.uintGensPastGrowth);
	fprintf(configFile,"Percent Mutation Chance: %u\n", values.uintMutationChance);
	fprintf(configFile,"Log to File: %u\n", values.blnLogging ? 1 : 0);
	fprintf(configFile,"Hard mode: %u\n", values.blnHardMode ? 1 : 0);
	fprintf(configFile,"Show map on update: %u\n", values.blnShowMap ? 1 : 0);
	fprintf(configFile,"Random Seed: [%u]\n", values.uintSeed);
	fprintf(configFile,"Append Time: %u\n", values.blnAppendTime ? 1 : 0);
	fprintf(configFile, "Only define these if the default screen size doesn't work for you, otherwise leave blank.\n");
	fprintf(configFile, "Screen Height: 0\n");
	fprintf(configFile, "Screen Width: 0\n");
	fclose(configFile);
}
/**********************************************************************************************************************************************/
char clsConfig::verisonCheck(const char *ConfigVerison) {
	//This checks the version number written at the top of the config file
	//against the internal version number of the program.
	//If it finds a difference in Software status or a Major revision change the config HAS to be replaced.
	//A Minor revision will result in a prompt to the user about if it should be replaced.
	//And if only a patch change is found then it will just use the old config
	//Lastly if no change is found then use the config of course

	uint C_MajorNum, C_MinorNum, C_PatchNum;
	char C_SoftwareStatus[15], P_SoftwareStatus[15];
	sscanf(ConfigVerison,"%u.%u.%u-%s",&C_MajorNum,&C_MinorNum,&C_PatchNum,&C_SoftwareStatus);
	sscanf(DEFINED_VER_FULLVERSION_STRING,"%*u.%*u.%*u-%s", &P_SoftwareStatus);
	if (Global::blnDebugMode) {
        printf("Config: v %u %u %u %s\n",C_MajorNum,C_MinorNum,C_PatchNum,C_SoftwareStatus);
        printf("Program: v %u %u %u %s\n",DEFINED_VER_MAJOR, DEFINED_VER_MINOR, DEFINED_VER_PATCH, P_SoftwareStatus);
    }

    if (DEFINED_VER_STATUS == "Release"){
        //Since the release doesn't have any any ending in the string we have to check this differently
        if (C_SoftwareStatus[0] != ')') {
            if (Global::blnDebugMode) {printf("Software Status outdated.\n");}
            return NEWCONFIG;
        }
    } else {
        if ( P_SoftwareStatus[0] != C_SoftwareStatus[0]) {
            if (Global::blnDebugMode) {printf("Software Status outdated.\n");}
            return NEWCONFIG;
        }
    }

    if (DEFINED_VER_MAJOR != C_MajorNum) {
        if (Global::blnDebugMode) {printf("Major number outdated.\n");}
        return NEWCONFIG;
    } else if (DEFINED_VER_MINOR != C_MinorNum) {
        if (Global::blnDebugMode) {printf("Minor number outdated.\n");}
        return PROMPTUSER;
    } else {
        if (Global::blnDebugMode) {printf("Nothing outdated.\n");}
        return USECONFIG;
    }
    return USECONFIG;
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
	values.blnLogging = (intTempBool == 1);

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
	values.blnShowMap = (intTempBool == 1);

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
	values.blnAppendTime = (intTempBool == 1);

	//Get blank line explaining screen sizes
	//Need two fgets because the line is more than 50 characters long.
	fgets(chrTempString,50,configFile);
	fgets(chrTempString,50,configFile);

	//Get Screen Height
	fgets(chrTempString,50,configFile);
    intValuesScanned = sscanf(chrTempString, "%*s %*s %u", &values.uintScreenHeight);
    if (intValuesScanned < 1) {printf("ERROR!"); values.uintScreenHeight = 0;}
    if (Global::blnDebugMode) {printf("Screen Height \t \t \t %u\n", values.uintScreenHeight);}

    //Get Screen Width
    fgets(chrTempString,50,configFile);
    intValuesScanned = sscanf(chrTempString, "%*s %*s %u", &values.uintScreenWidth);
    if (intValuesScanned < 1) {printf("ERROR!"); values.uintScreenWidth = 0;}
    if (Global::blnDebugMode) {printf("Screen Width \t \t \t %u\n", values.uintScreenWidth);}


	fclose(configFile);
	printf("\n\n");
}
/**********************************************************************************************************************************************/
void clsConfig::Check(void) {
	char chrTempString[50], chrConfigVerison;
	bool blnAnswered = false; //this is used for a fix for an issue I was facing.
                          //I'll figure out a better method later.

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
				switch (chrConfigVerison) {
					case 'Y' :
					case 'y' :
						//Replace the config file
						fclose(configFile);
						make();
						blnAnswered = true;
						break;
					case 'n' :
					case 'N' :
						//Load the config file
						load();
						blnAnswered = true;
						break;
					default :
						printf("\nUnknown answer; try again.\n");
						break;
				}; //end switch
			} while (!blnAnswered);
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
			return values.blnLogging ? 1 : 0;
			break;
		case cnfgShowMap :
			return values.blnShowMap ? 1 : 0;
			break;
		case cnfgAppendTime :
			return values.blnAppendTime ? 1 : 0;
			break;
		case cnfgHardMode :
			return values.blnHardMode ? 1 : 0;
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
        case cnfgScreenHeight :
            return values.uintScreenHeight;
            break;
        case cnfgScreenWidth :
            return values.uintScreenWidth;
            break;
		default :
			return 0;
			break;
	}; //end switch

	return 9999;
}
/**********************************************************************************************************************************************/
