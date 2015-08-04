/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <windows.h>
//#include <ncurses.h>
/**********************************************************************************************************************************************/
#include "mario.h"
/**********************************************************************************************************************************************/
const char* version = PROGRAM_VERISON; //PROGRAM_VERISON is define in mario.h
/**********************************************************************************************************************************************/
using namespace std;
/**********************************************************************************************************************************************/
uchar map[Map_Height][Map_Width];
/**********************************************************************************************************************************************/
inline bool fileexists(const char *fileName) {std::ifstream infile(fileName); return infile.good();}
/**********************************************************************************************************************************************/
GEN pastplayers[Players_Per_Generation];
GEN bestplayers[10];
PLYR player;
PLYR baseplayer;
MNSTR *monsters;
MNSTR *basemonsters;
bool blnLogging = true;
bool blnShowMap = false;
bool blnAppendTimeToSeed = false;
bool blnError = false;
bool blnHardMode = false;
uchar generationNum = 0;
uchar numplayers = 0;
unit intSeed;
unit InputFirstGen = 50;
unit InputGenIncrease = 50;
unit intGensPastGrowth = 10;
unit intPercentMutationChance = 30;
uchar intNumMonsters = 0;
unit intGenerationSteps = 50;
bool curses_started = false;
/**********************************************************************************************************************************************/
FILE* logfile;
FILE* ConfigFile;
FILE* mapFile;
/**********************************************************************************************************************************************/
int main() 
{
	//startCurses();
	CheckConfigFile();
	loadMap();
	ErrorCheck
	restartmap();
	showmap();
	getchar();
	
	if (intSeed != 0 && blnAppendTimeToSeed) {srand(time(NULL)+intSeed);}
	else if (intSeed != 0) {srand(intSeed);}
	else {srand(time(NULL));}
	
	char playerstatus = 0;
	if(blnShowMap) {showmap();}
	if (blnLogging) { logfile = fopen("log.txt","w"); fclose(logfile);}
	for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
		if (numplayers != 0) {nextplayer();}
		for (unit i = 0; i < InputFirstGen; i++) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}
		ErrorCheck
		for(unit step = 0; step < InputFirstGen; step++){
			moveMonsters();
			playerstatus = moveplayer(step);
			if(blnShowMap) {showmap();}
			ErrorCheck
			if(playerstatus == DEAD) {
				for (unit j = step; j < InputFirstGen; j++) {player.direction[j] = dirNone;}
				step = InputFirstGen;
			}
		}
		if(blnShowMap) {showmap();}
	}
	getbestplayers();
	ErrorCheck
	if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (uchar j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
	while (intGenerationSteps + InputGenIncrease <= Max_Player_Steps){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(intGenerationSteps);
			ErrorCheck
			for(unit step = 0; step < intGenerationSteps; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == DEAD) {
					for (unit j = step; j < intGenerationSteps; j++) {player.direction[j] = dirNone;}
					step = intGenerationSteps;
				}
			}
			if(blnShowMap) {showmap();}
		}
		getbestplayers();
		ErrorCheck
		if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (uchar j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
		intGenerationSteps += InputGenIncrease;
	}
	for (unit j = 0; j < intGensPastGrowth; j++){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(intGenerationSteps);
			ErrorCheck
			for(unit step = 0; step < intGenerationSteps; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == DEAD) {
					for (unit j = step; j < intGenerationSteps; j++) {player.direction[j] = dirNone;}
					step = intGenerationSteps;
				}
			}
			if(blnShowMap) {showmap(); getchar();}
		}
		getbestplayers();
		ErrorCheck
		if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (uchar j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
	}
	printf("\nDone!\n");
	free(monsters);
	free(basemonsters);
	getchar();
	//endCurses();
	return 0;
}
/**********************************************************************************************************************************************/
uchar GenerateRandomNumber(uchar Min, uchar Max){
	uchar temprand = 0;
	int inters = 0;
	do{
		inters ++;
		temprand = (rand() % (Max - Min)) + Min;
	} while (inters <= rand() % 1000);
	return temprand;
}
/**********************************************************************************************************************************************/
void restartmap(){
	for(uchar y = 0; y < Map_Height; y++){for(uchar x = 0; x < Map_Width; x++){map[y][x] = basemap[y][x];}}
	for(uchar i = 0; i < intNumMonsters; i++) {
		monsters[i].x = basemonsters[i].x;
		monsters[i].y = basemonsters[i].y;
		monsters[i].living = basemonsters[i].living;
		monsters[i].movingright = basemonsters[i].movingright;
	}
	player.x = baseplayer.x;
	player.y = baseplayer.y;
	player.falling = baseplayer.falling;
	player.fitness = baseplayer.fitness;
}
/**********************************************************************************************************************************************/
void nextplayer(){
	if(blnLogging) {logfile = fopen("log.txt","a");}
	restartmap();
	pastplayers[numplayers].fitness = player.fitness;
	if(blnLogging) {fprintf(logfile,"Player: %2d, Fitness: %2.2f",numplayers,player.fitness);}
	player.fitness = 0.00;
	for (int i = 0; i < Max_Player_Steps; i++){
		pastplayers[numplayers].direction[i] = player.direction[i];
		if(blnLogging) {
			if (player.direction[i] == dirUp) {fprintf(logfile, " ↑");}
			else if (player.direction[i] == dirLeft) {fprintf(logfile, " ←");}
			else if (player.direction[i] == dirRight) {fprintf(logfile, " →");}
		}
	}
	if(blnLogging) {fprintf(logfile,"\n"); fclose(logfile);}
}
/**********************************************************************************************************************************************/
void showmap(){
	Sleep(Sleep_Time);
	for (uchar i = 0; i < Map_Height; i++) {printf("\n");}
	for(uchar y = 0; y < Map_Height; y++){
		for(uchar x = player.x - 5; x < player.x + 73; x++){
			if (map[y][x] == tileSpace) {printf(" ");}
			else if (map[y][x] == tileWall) {printf("█");}
			else if (map[y][x] == tilePlayer) {printf("@");}
			else if (map[y][x] == tilePole) {printf("|");}
			else if (map[y][x] == tileMonster) {printf("?");}
			else {printf("#");}
		}
		printf("\n");
	}
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n",generationNum,numplayers+1,player.fitness);
}
/**********************************************************************************************************************************************/
void generateNewGenPlayer(unit GenerationInputs){
	uchar RandPlayer;
	unit i;
	for (i = 0; i < GenerationInputs; i++) {
		RandPlayer = PickBestPlayer(); 
		if (GenerateRandomNumber(0,100) < intPercentMutationChance) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}
		else {player.direction[i] = bestplayers[RandPlayer].direction[i];}
	}
	if(GenerationInputs < Max_Player_Steps) {for (i = GenerationInputs; i < GenerationInputs + InputGenIncrease; i++) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}}
}
/**********************************************************************************************************************************************/
double getfitness(unit step){
	double temp = 0.00F;
	temp += (5.0/2.0)*(player.x - 6.0);
	temp += (player.x + player.y) / 6.0;
	temp += (12.0 - player.y) / 4.0;
	if (player.x > 204) {temp += 200.0;}
	if(blnHardMode) {temp -= (step / 25.0);}
	return temp;
}
/**********************************************************************************************************************************************/
char moveplayer(int stepnum){
	uchar tempx = player.x;
	uchar tempy = player.y;
	switch (player.direction[stepnum]){
		case dirLeft :
			if (player.x != 0) {tempx --;}
			break;
		case dirRight :
			if (player.x != Map_Width - 1) {tempx ++;}
			break;
		case dirUp :
			if (map[player.y - 1][player.x] == tileWall) {break;}
			if (!player.falling && player.y != 0) {tempy-=2;}
			break;
	};
	if (tempx < 5 && blnHardMode) {return DEAD;}
	if (player.y == Map_Height - 1 && player.falling) {return DEAD;} //For Dead
	if (player.falling) {tempy ++;}
	if (map[tempy][tempx] == tileMonster) {
		if (player.falling) {KillMonster(tempx,tempy);}
		else {return DEAD;}
	}
	if (map[tempy][tempx] == tileWall){
		if (map[tempy][player.x] == tileSpace) {tempx = player.x;}
		else if (map[player.y][tempx] == tileSpace) {tempy = player.y;}
		else if (map[tempy + 1][tempx] == tileSpace) {tempy += 1;}
		else {tempy = player.y; tempx = player.x;}
	}
	map[player.y][player.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	player.x = tempx;
	player.y = tempy;
	if (blnHardMode) {player.fitness = getfitness(stepnum);}
	else {if (getfitness(stepnum) > player.fitness) {player.fitness = getfitness(stepnum);}}
	if (player.fitness < 0.00F && blnHardMode) {return DEAD;}
	if ((player.y == Map_Height - 1) || (map[player.y + 1][player.x] == tileSpace)) {player.falling = true;}
	else {player.falling = false;}
	return LIVING;
}
/**********************************************************************************************************************************************/
void getbestplayers(){
	for (uchar j = 0; j < 10; j++){
		double tempfitness = 0.00;
		uchar bestplayernum = 0;
		for(uchar i = 0; i < Players_Per_Generation; i++) {if (pastplayers[i].fitness > tempfitness) {tempfitness = pastplayers[i].fitness; bestplayernum = i;}}
		bestplayers[j].fitness = pastplayers[bestplayernum].fitness;
		for (int m = 0; m < Max_Player_Steps; m++) {bestplayers[j].direction[m] = pastplayers[bestplayernum].direction[m];}
		pastplayers[bestplayernum].fitness = 0.00; //Make 0 so that they aren't recorded again.
	}
}
/**********************************************************************************************************************************************/
void CheckConfigFile(){
	char chrTempString[50];
	char chrConfigVerison;
	int intTempBool;
	int intValuesScanned;
	bool blnFileExists = fileexists("MarioConfig.ini");
	if (blnFileExists){
		printf("Config File Found, loading values\n");
		ConfigFile = fopen("MarioConfig.ini","r");
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
		ConfigFile = fopen("MarioConfig.ini","w");
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
uchar PickBestPlayer(){
	//Makes it so Number 1 is more likely to be randomly selected, followed by number 2 and so on.
	uchar RandomNumber = GenerateRandomNumber(0,100);
	if (RandomNumber < 20) {return 0;}
	else if (RandomNumber < 35) {return 1;}
	else if (RandomNumber < 50) {return 2;}
	else if (RandomNumber < 60) {return 3;}
	else if (RandomNumber < 70) {return 4;}
	else if (RandomNumber < 80) {return 5;}
	else if (RandomNumber < 85) {return 6;}
	else if (RandomNumber < 90) {return 7;}
	else if (RandomNumber < 95) {return 8;}
	else {return 9;}
}
/**********************************************************************************************************************************************/
void moveMonsters(){
	uchar tempx;
	for(uchar i = 0; i < intNumMonsters; i++){
		if (monsters[i].living){
			if (monsters[i].movingright){tempx = monsters[i].x + 1;}
			else{ tempx = monsters[i].x -1;}
			if (map[monsters[i].y][tempx] == tileSpace){
				map[monsters[i].y][monsters[i].x] = tileSpace;
				map[monsters[i].y][tempx] = tileMonster;
				monsters[i].x = tempx;
			}
			else if (map[monsters[i].y][tempx] == tilePlayer) {player.y = Map_Height - 1; player.falling = true;}
			else if (map[monsters[i].y][tempx] == tileWall) {monsters[i].movingright = !(monsters[i].movingright);}
			if(map[monsters[i].y+1][monsters[i].x] == tileSpace) {tempx = monsters[i].x; monsters[i].living = false;} 
			if(monsters[i].living == false) {map[monsters[i].y][monsters[i].x] = tileSpace;}
		}
	}
}
/**********************************************************************************************************************************************/
void loadMap() {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.
	intNumMonsters = 0;
	for (uchar y = 0; y < 14; y ++) {
		for (uchar x = 0; x < 217; x++) {
			if (basemap[y][x] == tilePlayer) {
				baseplayer.x = x;
				baseplayer.y = y;
				baseplayer.falling = false;
			}
			else if (basemap[y][x] == tileMonster) {
				intNumMonsters++;
				if (intNumMonsters == 1) {
					basemonsters = (MNSTR*)malloc(sizeof(MNSTR) * 1);
					if (basemonsters == NULL) {blnError = true; return;}
					basemonsters[0].x = x;
					basemonsters[0].y = y;
					basemonsters[0].living = true;
					basemonsters[0].movingright = false;
				} else {
					basemonsters = (MNSTR*)realloc(basemonsters,sizeof(MNSTR) * intNumMonsters);
					if (basemonsters == NULL) {blnError = true; return;}
					basemonsters[intNumMonsters - 1].x = x;
					basemonsters[intNumMonsters - 1].y = y;
					basemonsters[intNumMonsters - 1].living = true;
					basemonsters[0].movingright = false;
				}
			}
		}
	}
	monsters = (MNSTR*)malloc(sizeof(MNSTR) * intNumMonsters);
	if (monsters == NULL) {blnError = true; return;}
}
/**********************************************************************************************************************************************/
char CheckVerison(const char *ConfigVerison) {
	unit P_MajorNum, P_MinorNum, P_PatchNum;
	unit C_MajorNum, C_MinorNum, C_PatchNum;
	sscanf(version,"v%u.%u.%u",&P_MajorNum,&P_MinorNum,&P_PatchNum);
	sscanf(ConfigVerison,"v%u.%u.%u",&C_MajorNum,&C_MinorNum,&C_PatchNum);
	//printf("\nProgram: v %u %u %u \n",P_MajorNum,P_MinorNum,P_PatchNum);
	//printf("Config: v %u %u %u \n",C_MajorNum,C_MinorNum,C_PatchNum);
	if (P_MajorNum != C_MajorNum) {return NEWCONFIG;}
	else if (P_MinorNum != C_MinorNum) {return PROMPTUSER;}
	else {return USECONFIG;}
}
/**********************************************************************************************************************************************/
void KillMonster(uchar xplace, uchar yplace) {
	for (unit i = 0; i < intNumMonsters; i++) {
		if (monsters[i].x == xplace && monsters[i].y == yplace) {
			monsters[i].living = false;
		}
	}
}
/**********************************************************************************************************************************************/
/*void endCurses() {
	if (curses_started && !isendwin()) {endwin();}
}*/
/**********************************************************************************************************************************************/
/*void startCurses() {
	if (curses_started) {refresh();}
	else {
		initscr();
		cbreak();
		noecho();
		intrflush(stdscr, false);
		keypad(stdscr, true);
		atexit(endCurses);
		curses_started = true;
	}
}*/
/**********************************************************************************************************************************************/
