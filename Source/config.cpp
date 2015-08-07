/**********************************************************************************************************************************************/
#include "config.h"
/**********************************************************************************************************************************************/
bool Config::exists(void) {
	//Returns true or false if config file exists
	std::ifstream infile(FileName);
	return infile.good();
}
/**********************************************************************************************************************************************/
void Config::make(void) {
	configFile = fopen(FileName,"w");
	printf("Config File will now be created!\n");
	
	fprintf(configFile,"Config File for the program.\n");
	fprintf(configFile,"%s\n",PROGRAM_VERISON);
	
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
char Config::verisonCheck(const char *ConfigVerison) {
	//This checks the verison number written at the top of the config file
	//against the internal verison number of the program.
	//If it finds a Major revision change the config HAS to be replaced.
	//A Minor revision will result in a propmt to the user about if it should be replaced.
	//And if only a patch change is found then it will just use the old config
	//Lastly if no change is found then use the config of course
	uint P_MajorNum, P_MinorNum, P_PatchNum;
	uint C_MajorNum, C_MinorNum, C_PatchNum;
	sscanf(PROGRAM_VERISON,"v%u.%u.%u",&P_MajorNum,&P_MinorNum,&P_PatchNum);
	sscanf(ConfigVerison,"v%u.%u.%u",&C_MajorNum,&C_MinorNum,&C_PatchNum);
	if (Global::blnDebugMode) {printf("\nProgram: v %u %u %u \n",P_MajorNum,P_MinorNum,P_PatchNum);}
	if (Global::blnDebugMode) {printf("Config: v %u %u %u \n",C_MajorNum,C_MinorNum,C_PatchNum);}
	if (P_MajorNum != C_MajorNum) {return NEWCONFIG;}
	else if (P_MinorNum != C_MinorNum) {return PROMPTUSER;}
	else {return USECONFIG;}
}
/**********************************************************************************************************************************************/
void Config::load(void) {
	char chrTempString[50];
	int intTempBool, intValuesScanned;
	//Get the First Generation Input from config
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %d",&values.uintFirstGen);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintFirstGen = 100;}
	printf("First Gen Steps \t \t %2d\n",values.uintFirstGen);
	
	//Get Amount the inputs increase by each generation
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %d",&values.uintGenIncrease);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintGenIncrease = 100;}
	printf("Generation Increase \t \t %2d\n",values.uintGenIncrease);
	
	//Get the number of generations past growth
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&values.uintGensPastGrowth);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintGensPastGrowth = 10;}
	printf("Gens Past Growth \t \t %u\n",values.uintGensPastGrowth);
	
	//Get the percent chance of mutation
	//Should I make this a float?
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %u", &values.uintMutationChance);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintMutationChance = 15;}
	printf("Percent Mutation Chance \t %u\n",values.uintMutationChance);	
	
	//Get if there is logging
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d", &intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
	printf("Log to file \t \t \t %d\n",intTempBool);
	if(intTempBool == 1) {values.blnLogging = true;}
	else {values.blnLogging = false;}
	
	//Check if hard mode is enabled.
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
	printf("Hard mode \t \t \t %u\n",intTempBool);
	if(intTempBool == 1) {values.blnHardMode = true;}
	else {values.blnHardMode = false;}
	
	//Check if show map is enabled
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
	printf("Show Map Update \t \t %d\n",intTempBool);
	if(intTempBool == 1) {values.blnShowMap = true;}
	else {values.blnShowMap = false;}		
	
	//Get the seed used in the config
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s [%u]", &values.uintSeed);
	if (intValuesScanned < 1) {printf("ERROR!"); values.uintSeed = 0;}
	printf("Random Seed \t \t \t %d\n",values.uintSeed);
	
	//Check if append time
	fgets(chrTempString,50,configFile);
	intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
	printf("Append Time \t \t \t %u\n",intTempBool);
	if(intTempBool == 1) {values.blnAppendTime = true;}
	else {values.blnAppendTime = false;}
	
	fclose(configFile);
	printf("\n\n");
}
/**********************************************************************************************************************************************/
void Config::Check(void) {
	char chrTempString[50], chrConfigVerison;
	
	if (Config::exists() != true) {
		printf("Config file was not found; creating now one\n");
		Config::make();
	} else {
		configFile = fopen(ConfigFileName,"r");
		printf("Config file found; loading values\n");
		fgets(chrTempString,50,configFile);
		fgets(chrTempString,50,configFile);
		chrConfigVerison = Config::verisonCheck(chrTempString);
		
		if (chrConfigVerison == NEWCONFIG) {
			printf("Current config file out of date. Making new one.\n");
			fclose(configFile);
			Config::make();
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
						fclose(configFile);
						Config::make();
						break;
					case 'n' :
					case 'N' :
						Config::load();
						break;
					default :
						//At some point I'll need to add a go back and propmt again
						//But you know, bigger and badder things await
						printf("\nUnknown answer; try again.\n");
						break;
				};
			} while (chrConfigVerison != 'n' || chrConfigVerison != 'N' || chrConfigVerison != 'Y' || chrConfigVerison != 'y');
		} else { Config::load();}
	}
}
/**********************************************************************************************************************************************/