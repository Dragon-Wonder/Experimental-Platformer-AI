/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
/**********************************************************************************************************************************************/
#include "platformer.h"
/**********************************************************************************************************************************************/
//Debug mode is defined in platformer.h, it just creates a boolean called blnDebugMode
//I chose to have it defined in the header so that it is unviersal if I change it one place
DEBUG_MODE
/**********************************************************************************************************************************************/
const char* version = PROGRAM_VERISON; //PROGRAM_VERISON is define in platformer.h
/**********************************************************************************************************************************************/
//These are values which will be pulled from the config file. They will be used under platformer.cpp
bool blnLogging = true; //If player direction will be logged to a file
bool blnShowMap = false; //if the console shows all the steps the players take
bool blnAppendTimeToSeed = false; //If time is added to the seed 
bool blnError = false; //If there was an error found
bool blnHardMode = false; //if hard mode is on. Hard mode basically makes the players die a lot more.
uint InputFirstGen; //How many inputs the first generation has
uint InputGenIncrease; //How many inputs that each generation increases by until the limit of Max_Player_Steps
uint intGensPastGrowth; //How many generations take place with Max_Player_Steps
uint intPercentMutationChance; //Percent chance that when using directions from a previous player, the direction will be replace with a random one.
uint intSeed; //The seed that is used with srand()
/**********************************************************************************************************************************************/
FILE* ConfigFile;
/**********************************************************************************************************************************************/
inline bool fileexists(const char *fileName) {std::ifstream infile(fileName); return infile.good();} //Return true or false if file exists
/**********************************************************************************************************************************************/
char CheckVerison(const char *ConfigVerison) {
	//This checks the verison number written at the top of the config file
	//against the internal verison number of the program.
	//If it finds a Major revision change the config HAS to be replaced.
	//A Minor revision will result in a propmt to the user about if it should be replaced.
	//And if only a patch change is found then it will just use the old config
	//Lastly if no change is found then use the config of course
	uint P_MajorNum, P_MinorNum, P_PatchNum;
	uint C_MajorNum, C_MinorNum, C_PatchNum;
	sscanf(version,"v%u.%u.%u",&P_MajorNum,&P_MinorNum,&P_PatchNum);
	sscanf(ConfigVerison,"v%u.%u.%u",&C_MajorNum,&C_MinorNum,&C_PatchNum);
	if (blnDebugMode) {printf("\nProgram: v %u %u %u \n",P_MajorNum,P_MinorNum,P_PatchNum);}
	if (blnDebugMode) {printf("Config: v %u %u %u \n",C_MajorNum,C_MinorNum,C_PatchNum);}
	if (P_MajorNum != C_MajorNum) {return NEWCONFIG;}
	else if (P_MinorNum != C_MinorNum) {return PROMPTUSER;}
	else {return USECONFIG;}
}

void CheckConfigFile(){
	char chrTempString[50], chrConfigVerison;
	int intTempBool, intValuesScanned;
	bool blnFileExists = fileexists(ConfigFileName); //Check if config file exists
	if (blnFileExists){
		printf("Config File Found, loading values\n");
		ConfigFile = fopen(ConfigFileName,"r");
		fgets(chrTempString,50,ConfigFile);
		fgets(chrTempString,50,ConfigFile);
		chrConfigVerison = CheckVerison(chrTempString); //Check the verison number of the config file.
	} else {chrConfigVerison = NEWCONFIG;}

	if (chrConfigVerison == PROMPTUSER) {
		//Prompt user about whether to use old config or make new one.
		printf("\nThe config file you are using has a different Minor Version than the program.\n");
		printf("The config file should in theory still work with this version but I can't say for sure.\n");
		printf("Would you like to replace the config file with a new one?\n");
		printf("Y or N\n> ");
		scanf("%c",&chrConfigVerison);
		switch (chrConfigVerison)
		{
			case 'Y' :
			case 'y' :
				chrConfigVerison = NEWCONFIG;
				break;
			case 'n' :
			case 'N' :
				chrConfigVerison = USECONFIG;
				break;
			default :
				//At some point I'll need to add a go back and propmt again
				//But you know, bigger and badder things await
				printf("\nUnknown answer; try again.\n");
				chrConfigVerison = USECONFIG;
				break;
		};
	}

	if (chrConfigVerison == USECONFIG) {
		//Use the current config file.
		
		//Get the First Generation Input from config
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %d",&InputFirstGen);
		if (intValuesScanned < 1) {printf("ERROR!"); InputFirstGen = 50;}
		printf("First Gen Steps \t \t %2d\n",InputFirstGen);
		
		//Get Amount the inputs increase by each generation
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %d",&InputGenIncrease);
		if (intValuesScanned < 1) {printf("ERROR!"); InputGenIncrease = 50;}
		printf("Generation Increase \t \t %2d\n",InputGenIncrease);
		
		//Get if there is logging
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d", &intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
		printf("Log to file \t \t \t %d\n",intTempBool);
		if(intTempBool == 1) {blnLogging = true;}
		else {blnLogging = false;}
		
		//Check if show map is enabled
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
		printf("Show Map Update \t \t %d\n",intTempBool);
		if(intTempBool == 1) {blnShowMap = true;}
		else {blnShowMap = false;}		
		
		//Get the seed used in the config
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s [%u]", &intSeed);
		if (intValuesScanned < 1) {printf("ERROR!"); intSeed = 0;}
		printf("Random Seed \t \t \t %d\n",intSeed);
		
		//Check if append time
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
		printf("Append Time \t \t \t %u\n",intTempBool);
		if(intTempBool == 1) {blnAppendTimeToSeed = true;}
		else {blnAppendTimeToSeed = false;}
		
		//Get the number of generations past growth
		//NOTE TO SELF: move this up by the other generation number
		//during next major revison.
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&intGensPastGrowth);
		if (intValuesScanned < 1) {printf("ERROR!"); intGensPastGrowth = 10;}
		printf("Gens Past Growth \t \t %u\n",intGensPastGrowth);
		
		//Get the percent chance of mutation
		//Should I make this a float?
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %u", &intPercentMutationChance);
		if (intValuesScanned < 1) {printf("ERROR!"); intPercentMutationChance = 30;}
		printf("Percent Mutation Chance \t %u\n",intPercentMutationChance);		
		
		//Check if hard mode is enabled.
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
		printf("Hard mode \t \t \t %u\n",intTempBool);
		if(intTempBool == 1) {blnHardMode = true;}
		else {blnHardMode = false;}
		
		fclose(ConfigFile);
		printf("\n\n");
	}
	
	
	if (chrConfigVerison == NEWCONFIG) {
		//New config will be made.
		printf("Config File not found it will be created!\n");
		ConfigFile = fopen(ConfigFileName,"w");
		fprintf(ConfigFile,"Config File for the program.\n");
		fprintf(ConfigFile,"%s\n",version);
		fprintf(ConfigFile,"First Generation Steps: 50\n");
		InputFirstGen = 50;
		fprintf(ConfigFile,"Generation Increase: 50\n");
		InputGenIncrease = 50;
		fprintf(ConfigFile,"Log to File: 1\n");
		blnLogging = true;
		fprintf(ConfigFile,"Show map on update: 0\n");
		blnShowMap = false;
		fprintf(ConfigFile,"Random Seed: [00000]\n");
		intSeed = 0;
		fprintf(ConfigFile,"Append Time: 1\n");
		blnAppendTimeToSeed = true;
		fprintf(ConfigFile,"Gens Past Growth: 10\n");
		intGensPastGrowth = 10;
		fprintf(ConfigFile,"Percent Mutation Chance: 30\n");
		intPercentMutationChance = 30;
		fprintf(ConfigFile,"Hard mode: 0\n");
		blnHardMode = false;
		fclose(ConfigFile);
	}
}
/**********************************************************************************************************************************************/