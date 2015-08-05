/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
/**********************************************************************************************************************************************/
#include "platformer.h"
/**********************************************************************************************************************************************/
DEBUG_MODE
/**********************************************************************************************************************************************/
const char* version = PROGRAM_VERISON; //PROGRAM_VERISON is define in mario.h
/**********************************************************************************************************************************************/
bool blnLogging = true;
bool blnShowMap = false;
bool blnAppendTimeToSeed = false;
bool blnError = false;
bool blnHardMode = false;
uint InputFirstGen;
uint InputGenIncrease;
uint intGensPastGrowth;
uint intPercentMutationChance;
uint intSeed;
/**********************************************************************************************************************************************/
FILE* ConfigFile;
/**********************************************************************************************************************************************/
inline bool fileexists(const char *fileName) {std::ifstream infile(fileName); return infile.good();}
/**********************************************************************************************************************************************/
char CheckVerison(const char *ConfigVerison) {
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
	char chrTempString[50];
	char chrConfigVerison;
	int intTempBool;
	int intValuesScanned;
	bool blnFileExists = fileexists(ConfigFileName);
	if (blnFileExists){
		printf("Config File Found, loading values\n");
		ConfigFile = fopen(ConfigFileName,"r");
		fgets(chrTempString,50,ConfigFile);
		fgets(chrTempString,50,ConfigFile);
		chrConfigVerison = CheckVerison(chrTempString);
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
				printf("\nUnknown answer; try again.\n");
				break;
		};
	}

	if (chrConfigVerison == USECONFIG) {
		//Use the current config file.
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %d",&InputFirstGen);
		if (intValuesScanned < 1) {printf("ERROR!"); InputFirstGen = 50;}
		printf("First Gen Steps \t \t %2d\n",InputFirstGen);
				
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %d",&InputGenIncrease);
		if (intValuesScanned < 1) {printf("ERROR!"); InputGenIncrease = 50;}
		printf("Generation Increase \t \t %2d\n",InputGenIncrease);
			
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d", &intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
		printf("Log to file \t \t \t %d\n",intTempBool);
		if(intTempBool == 1) {blnLogging = true;}
		else {blnLogging = false;}
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
		printf("Show Map Update \t \t %d\n",intTempBool);
		if(intTempBool == 1) {blnShowMap = true;}
		else {blnShowMap = false;}		
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s [%u]", &intSeed);
		if (intValuesScanned < 1) {printf("ERROR!"); intSeed = 0;}
		printf("Random Seed \t \t \t %d\n",intSeed);
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
		printf("Append Time \t \t \t %u\n",intTempBool);
		if(intTempBool == 1) {blnAppendTimeToSeed = true;}
		else {blnAppendTimeToSeed = false;}
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&intGensPastGrowth);
		if (intValuesScanned < 1) {printf("ERROR!"); intGensPastGrowth = 10;}
		printf("Gens Past Growth \t \t %u\n",intGensPastGrowth);
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %u", &intPercentMutationChance);
		if (intValuesScanned < 1) {printf("ERROR!"); intPercentMutationChance = 30;}
		printf("Percent Mutation Chance \t %u\n",intPercentMutationChance);		
		
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
