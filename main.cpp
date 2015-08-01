/**********************************************************************************************************************************************/
/*
Made By: Patrick J. Rye
Purpose: A very makeshift AI learning program. Its kind of like Mario but much simpler	
*/
const char version[] = "v1.1.1";	
/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <windows.h>
/**********************************************************************************************************************************************/
using namespace std;
/**********************************************************************************************************************************************/
//Basemap should have array sizes of 14x217
const unsigned char basemap[14][217] = {{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,0},
{0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,0},
{1,1,1,1,1,2,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,4,0,0,1,1,1,0,0,0,0,0,0,0,0,4,0,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,3,0,0,0,1,1,1,1,1,1,0,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
/**********************************************************************************************************************************************/
unsigned char map[14][217];
//unsigned char basemap[14][217];
/**********************************************************************************************************************************************/
struct generation {
	double fitness;
	unsigned char direction[1000];
};

struct player {
	unsigned char x;
	unsigned char y;
	double fitness;
	unsigned char direction[1000];
	bool falling;
};

struct monster {
	unsigned char x;
	unsigned char y;
	bool living;
};

enum dir {
	dirNone = 0,
	dirUp,
	dirLeft,
	dirRight
};

enum tile {
	tileSpace = 0, 
	tileWall, //1
	tilePlayer, //2
	tilePole, //3
	tileMonster //4
};
/**********************************************************************************************************************************************/
inline bool fileexists(const char *fileName) {std::ifstream infile(fileName); return infile.good();}
/**********************************************************************************************************************************************/
generation pastplayers[40];
generation bestplayers[10];
player player;
monster *monsters;
bool blnLogging = true;
bool blnShowMap = false;
bool blnAppendTimeToSeed = false;
bool blnError = false;
unsigned char generationNum = 0;
unsigned char numplayers = 0;
unsigned int intSeed;
unsigned int InputFirstGen = 50;
unsigned int InputGenIncrease = 50;
unsigned int intGensPastGrowth = 10;
unsigned int intPercentMutationChance = 30;
unsigned char intNumMonsters = 0;
unsigned int intGenerationSteps = 50;
/**********************************************************************************************************************************************/
FILE* logfile;
FILE* ConfigFile;
FILE* mapFile;
/**********************************************************************************************************************************************/
unsigned char GenerateRandomNumber(unsigned char, unsigned char);
void restartmap(void);
void nextplayer(void);
void showmap(void);
void generateNewPlayer(int);
void generateNewGenPlayer(unsigned int);
double getfitness(unsigned int);
char moveplayer(int);
void getbestplayers(void);
void CheckConfigFile(void);
unsigned char PickBestPlayer(void);
void findMonsters(void);
void moveMonsters(void);
/**********************************************************************************************************************************************/
int main() 
{
	CheckConfigFile();
	if (blnError) {printf("\nThere was an error!\n"); return 1;}
	restartmap();
	if (blnError) {printf("\nThere was an error!\n"); return 1;}
	showmap();
	if (blnError) {printf("\nThere was an error!\n"); return 1;}
	getchar();
	if (intSeed != 0 && blnAppendTimeToSeed) {srand(time(NULL)+intSeed);}
	else if (intSeed != 0) {srand(intSeed);}
	else {srand(time(NULL));}
	char playerstatus = 0;
	if(blnShowMap) {showmap();}
	if (blnLogging) { logfile = fopen("log.txt","w"); fclose(logfile);}
	for (numplayers = 0; numplayers < 40; numplayers ++){
		if (numplayers != 0) {nextplayer();}
		for (unsigned int i = 0; i < InputFirstGen; i++) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}
		if (blnError) {printf("\nThere was an error!\n"); return 1;}
		for(unsigned int step = 0; step < InputFirstGen; step++){
			moveMonsters();
			playerstatus = moveplayer(step);
			if(blnShowMap) {showmap();}
			if (blnError) {printf("\nThere was an error!\n"); return 1;}
			if(playerstatus == 'D') {
				for (unsigned int j = step; j < InputFirstGen; j++) {player.direction[j] = dirNone;}
				step = InputFirstGen;
			}
		}
		if(blnShowMap) {showmap();}
	}
	getbestplayers();
	if (blnError) {printf("\nThere was an error!\n"); return 1;}
	printf("Best Player fitnesses are:\n");
	for (unsigned char j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}
	//getchar();
	while (intGenerationSteps + InputGenIncrease <= 1000){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < 40; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(intGenerationSteps);
			if (blnError) {printf("\nThere was an error!\n"); return 1;}
			for(unsigned int step = 0; step < intGenerationSteps; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == 'D') {
					for (unsigned int j = step; j < intGenerationSteps; j++) {player.direction[j] = dirNone;}
					step = intGenerationSteps;
				}
			}
			if(blnShowMap) {showmap();}
		}
		getbestplayers();
		if (blnError) {printf("\nThere was an error!\n"); return 1;}
		printf("Best Player fitnesses are:\n");
		for (unsigned char j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}
		//getchar();
		intGenerationSteps += InputGenIncrease;
	}
	for (unsigned int j = 0; j < intGensPastGrowth; j++){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < 40; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(intGenerationSteps);
			if (blnError) {printf("\nThere was an error!\n"); return 1;}
			for(unsigned int step = 0; step < intGenerationSteps; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == 'D') {
					for (unsigned int j = step; j < intGenerationSteps; j++) {player.direction[j] = dirNone;}
					step = intGenerationSteps;
				}
			}
			if(blnShowMap) {showmap(); getchar();}
		}
		getbestplayers();
		if (blnError) {printf("\nThere was an error!\n"); return 1;}
		printf("Best Player fitnesses are:\n");
		for (unsigned char j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}
		//getchar();
	}
	printf("\nDone!\n");
	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
unsigned char GenerateRandomNumber(unsigned char Min, unsigned char Max){
	//srand(time(NULL));
	unsigned char temprand = 0;
	int inters = 0;
	do{
		inters ++;
		temprand = (rand() % (Max - Min)) + Min;
	} while (inters <= rand() % 1000);
	return temprand;
}
/**********************************************************************************************************************************************/
void restartmap(){
	free(monsters);
	for(unsigned char y = 0; y < 14; y++){
		for(unsigned char x = 0; x < 217; x++){
			map[y][x] = basemap[y][x];
			if(basemap[y][x] == tilePlayer){
				player.x = x;
				player.y = y;
				player.falling = false;
			}
		}
	}
	//printf("Start monster finder");
	findMonsters();
}
/**********************************************************************************************************************************************/
void nextplayer(){
	if(blnLogging) {logfile = fopen("log.txt","a");}
	restartmap();
	pastplayers[numplayers].fitness = player.fitness;
	if(blnLogging) {fprintf(logfile,"Player: %2d, Fitness: %2.2f",numplayers,player.fitness);}
	player.fitness = 0.00;
	for (int i = 0; i < 1000; i++){
		pastplayers[numplayers].direction[i] = player.direction[i];
		if(blnLogging) {
			if (player.direction[i] == dirUp) {fprintf(logfile, " ↑");}
			else if (player.direction[i] == dirLeft) {fprintf(logfile, " ←");}
			else if (player.direction[i] == dirRight) {fprintf(logfile, " →");}
			//else {fprintf(logfile,"");}
		}
	}
	if(blnLogging) {fprintf(logfile,"\n"); fclose(logfile);}
}
/**********************************************************************************************************************************************/
void showmap(){
	Sleep(65);
	for (unsigned char i = 0; i < 14; i++) {printf("\n");}
	//system("CLS");
	for(unsigned char y = 0; y < 14; y++){
		for(unsigned char x = player.x - 5; x < player.x + 73; x++){
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
void generateNewGenPlayer(unsigned int GenerationInputs){
	unsigned char RandPlayer;
	unsigned int i;
	for (i = 0; i < GenerationInputs; i++) {
		RandPlayer = PickBestPlayer(); 
		if (GenerateRandomNumber(0,100) < intPercentMutationChance) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}
		else {player.direction[i] = bestplayers[RandPlayer].direction[i];}
	}
	if(GenerationInputs < 1000) {for (i = GenerationInputs; i < GenerationInputs + InputGenIncrease; i++) {player.direction[i] = GenerateRandomNumber(dirUp, dirRight + 1);}}
}
/**********************************************************************************************************************************************/
double getfitness(unsigned int step){
	double temp = 0.00F;
	temp += (5.0/2.0)*(player.x - 6.0);
	temp += (player.x + player.y) / 6.0;
	temp += (12.0 - player.y) / 4.0;
	if (player.x > 204) {temp += 200.0;}
	temp -= (step / 80.0);
	return temp;
}
/**********************************************************************************************************************************************/
char moveplayer(int stepnum){
	unsigned char tempx = player.x;
	unsigned char tempy = player.y;
	switch (player.direction[stepnum]){
		case dirLeft :
			if (player.x != 0) {tempx --;}
			break;
		case dirRight :
			if (player.x != 216) {tempx ++;}
			break;
		case dirUp :
			if (map[player.y - 1][player.x] == tileWall) {break;}
			if (!player.falling && player.y != 0) {tempy-=2;}
			break;
	};
	if (tempx < 5) {return 'D';}
	if (player.y == 13 && player.falling) {return 'D';} //For Dead
	if (player.falling) {tempy ++;}
	if (map[tempy][tempx] == tileMonster) {return 'D';}
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
	if (getfitness(stepnum) > player.fitness) {player.fitness = getfitness(stepnum);}
	player.fitness = getfitness(stepnum);
	if (player.fitness < 0.00F) {return 'D';}
	if ((player.y == 13) || (map[player.y + 1][player.x] == tileSpace)) {player.falling = true;}
	else {player.falling = false;}
	return 'L';
}
/**********************************************************************************************************************************************/
void getbestplayers(){
	for (unsigned char j = 0; j < 10; j++){
		double tempfitness = 0.00;
		unsigned char bestplayernum = 0;
		for(unsigned char i = 0; i < 40; i++) {if (pastplayers[i].fitness > tempfitness) {tempfitness = pastplayers[i].fitness; bestplayernum = i;}}
		bestplayers[j].fitness = pastplayers[bestplayernum].fitness;
		for (int m = 0; m < 1000; m++) {bestplayers[j].direction[m] = pastplayers[bestplayernum].direction[m];}
		pastplayers[bestplayernum].fitness = 0.00; //Make 0 so that they aren't recorded again.
	}
}
/**********************************************************************************************************************************************/
void CheckConfigFile(){
	char chrTempString[50];
	bool blnFileExists = fileexists("MarioConfig.ini");
	if (blnFileExists){
		printf("Config File Found, loading values\n");
		int intTempBool;
		int intValuesScanned;
		ConfigFile = fopen("MarioConfig.ini","r");
		
		fgets(chrTempString,50,ConfigFile);
		printf("%s\n",chrTempString);
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %d",&InputFirstGen);
		if (intValuesScanned < 1) {printf("ERROR!"); InputFirstGen = 50;}
		printf("First Gen Steps \t \t %2d\n",InputFirstGen);
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %d",&InputGenIncrease);
		if (intValuesScanned < 1) {printf("ERROR!"); InputGenIncrease = 50;}
		printf("Generation Increase \t \t %2d\n",InputGenIncrease);
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d", &intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
		printf("Log to file \t \t \t %d\n",intTempBool);
		if(intTempBool == 1) {blnLogging = true;}
		else {blnLogging = false;}
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
		printf("Show Map Update \t \t %d\n",intTempBool);
		if(intTempBool == 1) {blnShowMap = true;}
		else {blnShowMap = false;}		
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString,"%*s %*s [%u]", &intSeed);
		if (intValuesScanned < 1) {printf("ERROR!"); intSeed = 0;}
		printf("Random Seed \t \t \t %d\n",intSeed);
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 1;}
		printf("Append Time \t \t \t %u\n",intTempBool);
		if(intTempBool == 1) {blnAppendTimeToSeed = true;}
		else {blnAppendTimeToSeed = false;}
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&intGensPastGrowth);
		if (intValuesScanned < 1) {printf("ERROR!"); intGensPastGrowth = 10;}
		printf("Gens Past Growth \t \t \t %u\n",intGensPastGrowth);
		
		fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %u", &intPercentMutationChance);
		if (intValuesScanned < 1) {printf("ERROR!"); intPercentMutationChance = 30;}
		printf("Percent Mutation Chance \t \t \t %u\n",intPercentMutationChance);		
		
		//fgets(chrTempString,50,ConfigFile);
		//printf("%s\n",chrTempString);
		//intValuesScanned = sscanf(chrTempString, "%*s %*s %*s [%s]", &strMapName);
		//if (intValuesScanned < 1) {printf("ERROR!"); strMapName = "Level1.dat";}
		//printf("Map Name \t \t \t %s\n",strMapName);
		
		fclose(ConfigFile);
		printf("\n\n\n\n\n");
	}else{
		printf("Config File not found it will be created!\n");
		ConfigFile = fopen("MarioConfig.ini","w");
		fprintf(ConfigFile,"Config File for the program.\n");
		fprintf(ConfigFile,"First Generation Steps: 50\n");
		fprintf(ConfigFile,"Generation Increase: 50\n");
		fprintf(ConfigFile,"Log to File: 1\n");
		fprintf(ConfigFile,"Show map on update: 0\n");
		fprintf(ConfigFile,"Random Seed: [00000]\n");
		fprintf(ConfigFile,"Append Time: 1\n");
		fprintf(ConfigFile,"Gens Past Growth: 10\n");
		fprintf(ConfigFile,"Percent Mutation Chance: 30\n");
		//fprintf(ConfigFile,"Load map name: [Level1.dat]\n");
		fclose(ConfigFile);
	}
}
/**********************************************************************************************************************************************/
unsigned char PickBestPlayer(){
	//Makes it so Number 1 is more likely to be randomly selected, followed by number 2 and so on.
	unsigned char RandomNumber = GenerateRandomNumber(0,100);
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
void findMonsters(){
	intNumMonsters = 0;
	for (unsigned char y = 0; y < 14; y++){
		for (unsigned char x = 0; x < 217; x++){
			if(map[y][x] == tileMonster){
				if(intNumMonsters == 0){
					monsters = (struct monster*)malloc(sizeof(struct monster) * 1);
					if (monsters == NULL) {blnError = true; return;}
					intNumMonsters++;
					monsters[0].x = x;
					monsters[0].y = y;
					monsters[0].living = true;
				}else{
					intNumMonsters++;
					monsters = (struct monster*)realloc(monsters,sizeof(struct monster) * intNumMonsters);
					if (monsters == NULL) {blnError = true; return;}
					monsters[intNumMonsters - 1].x = x;
					monsters[intNumMonsters - 1].y = y;
					monsters[intNumMonsters - 1].living = true;
				}
			}
		}
	}
}
/**********************************************************************************************************************************************/
void moveMonsters(){
	unsigned char Direction = 0;
	for(unsigned char i = 0; i < intNumMonsters; i++){
		if (monsters[i].living){
			Direction = GenerateRandomNumber(dirLeft, dirRight + 1);
			unsigned char tempx = monsters[i].x;
			switch (Direction){
				case dirLeft:
					tempx --;
					break;
				case dirRight:
					tempx ++;
					break;
			};
			if(map[monsters[i].y+1][monsters[i].x] == tileSpace) {tempx = monsters[i].x; monsters[i].living = false;}
			if (map[monsters[i].y][tempx] == tileSpace){
				map[monsters[i].y][monsters[i].x] = tileSpace;
				map[monsters[i].y][tempx] = tileMonster;
				monsters[i].x = tempx;
			}
			else if (map[monsters[i].y][tempx] == tilePlayer) {player.y = 13; player.falling = true;}
			if(monsters[i].living == false) {map[monsters[i].y][monsters[i].x] = tileSpace;}
		}
	}
}
/**********************************************************************************************************************************************/
