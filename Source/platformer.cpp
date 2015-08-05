/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <windows.h>
//#include <ncurses.h>
/**********************************************************************************************************************************************/
#include "platformer.h"
/**********************************************************************************************************************************************/
using namespace std;
/**********************************************************************************************************************************************/
extern const unsigned char basemap[Map_Height][Map_Width];
uchar map[Map_Height][Map_Width];
/**********************************************************************************************************************************************/
DEBUG_MODE
/**********************************************************************************************************************************************/
//These values are declared in config.cpp when config is loaded
extern bool blnLogging;
extern bool blnShowMap;
extern bool blnAppendTimeToSeed;
extern bool blnError;
extern bool blnHardMode;
extern uint InputFirstGen;
extern uint InputGenIncrease;
extern uint intGensPastGrowth;
extern uint intPercentMutationChance;
extern uint intSeed;
/**********************************************************************************************************************************************/
GEN pastplayers[Players_Per_Generation];
GEN bestplayers[10];
PLYR player;
PLYR baseplayer;
MNSTR *monsters;
MNSTR *basemonsters;
uchar generationNum = 0;
uchar numplayers = 0;
uchar intNumMonsters = 0;
uint intGenerationSteps = 50;
//bool curses_started = false;
/**********************************************************************************************************************************************/
FILE* logfile;
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
	if (blnLogging) { /*Opens log file to clear it */logfile = fopen(LogFileName,"w"); fclose(logfile);}
	
	//First Generation of players
	for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
		if (numplayers != 0) {nextplayer();}
		for (uint i = 0; i < InputFirstGen; i++) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}
		ErrorCheck
		for(uint step = 0; step < InputFirstGen; step++){
			moveMonsters();
			playerstatus = moveplayer(step);
			if(blnShowMap) {showmap();}
			ErrorCheck
			if(playerstatus == DEAD) {
				for (uint j = step; j < InputFirstGen; j++) {player.direction[j] = dirNone;}
				step = InputFirstGen;
			}
		}
		if(blnShowMap) {showmap();}
	}
	nextplayer(); //For the last player of the generation.
	getbestplayers();
	ErrorCheck
	if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (uchar j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
	
	//The Growth generation. Each Generation will have more steps until whatever the maxium steps is set as
	while (intGenerationSteps + InputGenIncrease <= Max_Player_Steps){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(intGenerationSteps);
			ErrorCheck
			for(uint step = 0; step < intGenerationSteps; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == DEAD) {
					for (uint j = step; j < intGenerationSteps; j++) {player.direction[j] = dirNone;}
					step = intGenerationSteps;
				}
			}
			if(blnShowMap) {showmap();}
		}
		nextplayer(); //For the last player of the generation.
		getbestplayers();
		ErrorCheck
		if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (uchar j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
		intGenerationSteps += InputGenIncrease;
	}
	
	//Generations past growth.
	for (uint j = 0; j < intGensPastGrowth; j++){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(Max_Player_Steps);
			ErrorCheck
			for(uint step = 0; step < Max_Player_Steps; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == DEAD) {
					for (uint j = step; j < Max_Player_Steps; j++) {player.direction[j] = dirNone;}
					step = Max_Player_Steps;
				}
			}
			if(blnShowMap) {showmap();}
		}
		nextplayer(); //For the last player of the generation.
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
	player.fitness = 0.00;
}
/**********************************************************************************************************************************************/
void nextplayer(){
	if(blnLogging) {logfile = fopen(LogFileName,"a");}
	if(blnDebugMode) {printf("Player finished with fitness = %2.3f\n",player.fitness);}
	pastplayers[numplayers].fitness = player.fitness;
	if(blnLogging) {fprintf(logfile,"Generation: %2d, Player: %2d, Fitness: %2.2f",generationNum + 1,numplayers,player.fitness);}
	player.fitness = 0.00f;
	player.score = 0;
	for (int i = 0; i < Max_Player_Steps; i++){
		pastplayers[numplayers].direction[i] = player.direction[i];
		if(blnLogging) {
			if (player.direction[i] == dirUp) {fprintf(logfile, " ↑");}
			else if (player.direction[i] == dirLeft) {fprintf(logfile, " ←");}
			else if (player.direction[i] == dirRight) {fprintf(logfile, " →");}
			else if (player.direction[i] == dirDown) {fprintf(logfile, " ↓");}
		}
	}
	if (blnLogging && (numplayers == Players_Per_Generation)) {
		fprintf(logfile,"\n");
		for (uint j = 0; j < Max_Player_Steps + 42; j++) {fprintf(logfile,"=");}
	}
	if(blnLogging) {fprintf(logfile,"\n"); fclose(logfile);}
	restartmap();
}
/**********************************************************************************************************************************************/
void showmap(){
	Sleep(Sleep_Time);
	//for (uchar i = 0; i < Map_Height; i++) {printf("\n");}
	printf("\n\n\n\n");
	for(uchar y = 0; y < Map_Height; y++){
		for(uchar x = player.x - 5; x < player.x + 73; x++){
			if (map[y][x] == tileSpace) {printf(" ");}
			else if (map[y][x] == tileWall) {printf("█");}
			else if (map[y][x] == tilePlayer) {printf("@");}
			else if (map[y][x] == tilePole) {printf("║");}
			else if (map[y][x] == tileMonster) {printf("+");}
			else if (map[y][x] == tileCoin) {printf("O");}
			else {printf("#");}
		}
		printf("\n");
	}
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n",generationNum,numplayers+1,player.fitness);
}
/**********************************************************************************************************************************************/
void generateNewGenPlayer(uint GenerationInputs){
	uchar RandPlayer;
	uint i;
	for (i = 0; i < GenerationInputs; i++) {
		RandPlayer = PickBestPlayer(); 
		if (GenerateRandomNumber(0,100) < intPercentMutationChance) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}
		else {player.direction[i] = bestplayers[RandPlayer].direction[i];}
	}
	if(GenerationInputs < Max_Player_Steps) {for (i = GenerationInputs; i < GenerationInputs + InputGenIncrease; i++) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}}
}
/**********************************************************************************************************************************************/
float getfitness(uint step){
	float temp = 0.00f;
	temp += (player.score) / 800.0;
	temp += (5.0/2.0)*(player.x - 6.0);
	temp += (player.x + player.y) / 6.0;
	temp += (12.0 - player.y) / 4.0;
	if (player.x > 204) {temp += 200.0;}
	if(blnHardMode) {temp -= (step / 80.0);}
	return temp;
}
/**********************************************************************************************************************************************/
char moveplayer(int stepnum){
	static uchar jumpcount;
	static bool playerfalling;
	if (playerfalling == false) {jumpcount = 0;}
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
			if (player.y > Jump_Height && jumpcount < Max_Jump_Count ) {tempy -= Jump_Height; jumpcount++;}
			break;
	};
	if (tempx < 5 && blnHardMode) {return DEAD;}
	if (player.y == Map_Height - 1 && playerfalling) {return DEAD;}
	if (playerfalling) {tempy ++;}
	if (map[tempy][tempx] == tileMonster) {
		if (playerfalling) {KillMonster(tempx,tempy); player.score += MonsKill_Points;}
		else {return DEAD;}
	}
	if (map[tempy][tempx] == tileWall){
		if (map[tempy][player.x] != tileWall) {tempx = player.x;}
		else if (map[player.y][tempx] != tileWall) {tempy = player.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = player.y; tempx = player.x;}
	}
	if (map[tempy][tempx] == tileCoin) {player.score += Coin_Points;}
	map[player.y][player.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	player.x = tempx;
	player.y = tempy;
	if(stepnum == 0) {player.fitness = getfitness(stepnum);}
	if (blnHardMode) {player.fitness = getfitness(stepnum);}
	else {if (getfitness(stepnum) > player.fitness) {player.fitness = getfitness(stepnum);}}
	if (player.fitness < 0.00f && blnHardMode) {return DEAD;}
	if ((player.y == Map_Height - 1) || (map[player.y + 1][player.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}
	return LIVING;
}
/**********************************************************************************************************************************************/
void getbestplayers(){
	for (uchar j = 0; j < 10; j++){
		float tempfitness = 0.00f;
		uchar bestplayernum = 0;
		for(uchar i = 0; i < Players_Per_Generation; i++) {if (pastplayers[i].fitness > tempfitness) {tempfitness = pastplayers[i].fitness; bestplayernum = i;}}
		bestplayers[j].fitness = pastplayers[bestplayernum].fitness;
		if (blnDebugMode) {printf("Player: %d found with fitness %2.3f\n",bestplayernum,pastplayers[bestplayernum].fitness);}
		for (int m = 0; m < Max_Player_Steps; m++) {bestplayers[j].direction[m] = pastplayers[bestplayernum].direction[m];}
		pastplayers[bestplayernum].fitness = 0.00; //Make 0 so that they aren't recorded again.
	}
	printf("\n\n");
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
			tempx = monsters[i].x;
			if (monsters[i].movingright){tempx = monsters[i].x + 1;}
			else{ tempx = monsters[i].x -1;}
			if (map[monsters[i].y][tempx] == tileSpace){
				map[monsters[i].y][monsters[i].x] = tileSpace;
				map[monsters[i].y][tempx] = tileMonster;
				monsters[i].x = tempx;
			}
			else if (map[monsters[i].y][tempx] == tilePlayer) {player.y = Map_Height - 1;}
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
	for (uchar y = 0; y < Map_Height; y ++) {
		for (uchar x = 0; x < Map_Width; x++) {
			if (basemap[y][x] == tilePlayer) {
				baseplayer.x = x;
				baseplayer.y = y;
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
void KillMonster(uchar xplace, uchar yplace) {
	for (uint i = 0; i < intNumMonsters; i++) {
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