/**********************************************************************************************************************************************/
const char* version = "v2.1.0";	
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
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,0},
{0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,0},
{1,1,1,1,1,2,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,4,0,0,1,1,1,1,0,0,0,0,0,0,0,4,0,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,3,0,0,0,1,1,1,1,1,1,0,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
/**********************************************************************************************************************************************/
unsigned char map[14][217];
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
	bool movingright;
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
struct player player;
struct player baseplayer;
struct monster *monsters;
struct monster *basemonsters;
bool blnLogging = true;
bool blnShowMap = false;
bool blnAppendTimeToSeed = false;
bool blnError = false;
bool blnHardMode = false;
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
void moveMonsters(void);
void loadMap(void);
char CheckVerison(const char *);
void KillMonster(unsigned char, unsigned char);
/**********************************************************************************************************************************************/
int main() 
{
	CheckConfigFile();
	loadMap();
	if (blnError) {printf("\nThere was an error!\n"); return 1;}
	restartmap();
	showmap();
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
	if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (unsigned char j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
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
		if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (unsigned char j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
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
		if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (unsigned char j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
	}
	printf("\nDone!\n");
	free(monsters);
	free(basemonsters);
	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
unsigned char GenerateRandomNumber(unsigned char Min, unsigned char Max){
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
	for(unsigned char y = 0; y < 14; y++){for(unsigned char x = 0; x < 217; x++){map[y][x] = basemap[y][x];}}
	for(unsigned char i = 0; i < intNumMonsters; i++) {
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
	for (int i = 0; i < 1000; i++){
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
	Sleep(65);
	for (unsigned char i = 0; i < 14; i++) {printf("\n");}
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
	if(blnHardMode) {temp -= (step / 50.0);}
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
	if (tempx < 5 && blnHardMode) {return 'D';}
	if (player.y == 13 && player.falling) {return 'D';} //For Dead
	if (player.falling) {tempy ++;}
	if (map[tempy][tempx] == tileMonster) {
		if (player.falling) {KillMonster(tempx,tempy);}
		else {return 'D';}
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
	if (player.fitness < 0.00F && blnHardMode) {return 'D';}
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
	} else {chrConfigVerison = 'N';}
	
	if (chrConfigVerison == 'P') {
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
				chrConfigVerison = 'N';
				break;
			case 'N' :
			case 'n' :
				chrConfigVerison = 'U';
				break;
			default :
				printf("\nUnknown answer; try again.\n");
				break;
		};
	}
	
	if (chrConfigVerison == 'U') {
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
		printf("Gens Past Growth \t \t \t %u\n",intGensPastGrowth);
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %u", &intPercentMutationChance);
		if (intValuesScanned < 1) {printf("ERROR!"); intPercentMutationChance = 30;}
		printf("Percent Mutation Chance \t \t \t %u\n",intPercentMutationChance);		
		
		fgets(chrTempString,50,ConfigFile);
		intValuesScanned = sscanf(chrTempString,"%*s %*s %d",&intTempBool);
		if (intValuesScanned < 1) {printf("ERROR!"); intTempBool = 0;}
		printf("Hard mode \t \t \t %u\n",intTempBool);
		if(intTempBool == 1) {blnHardMode = true;}
		else {blnHardMode = false;}
		
		fclose(ConfigFile);
		printf("\n\n\n\n\n");
	}
	
	
	if (chrConfigVerison == 'N') {
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
void moveMonsters(){
	unsigned char tempx;
	for(unsigned char i = 0; i < intNumMonsters; i++){
		if (monsters[i].living){
			if (monsters[i].movingright){tempx = monsters[i].x + 1;}
			else{ tempx = monsters[i].x -1;}
			if (map[monsters[i].y][tempx] == tileSpace){
				map[monsters[i].y][monsters[i].x] = tileSpace;
				map[monsters[i].y][tempx] = tileMonster;
				monsters[i].x = tempx;
			}
			else if (map[monsters[i].y][tempx] == tilePlayer) {player.y = 13; player.falling = true;}
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
	for (unsigned char y = 0; y < 14; y ++) {
		for (unsigned char x = 0; x < 217; x++) {
			if (basemap[y][x] == tilePlayer) {
				baseplayer.x = x;
				baseplayer.y = y;
				baseplayer.falling = false;
			}
			else if (basemap[y][x] == tileMonster) {
				intNumMonsters++;
				if (intNumMonsters == 1) {
					basemonsters = (struct monster*)malloc(sizeof(struct monster) * 1);
					if (basemonsters == NULL) {blnError = true; return;}
					basemonsters[0].x = x;
					basemonsters[0].y = y;
					basemonsters[0].living = true;
					basemonsters[0].movingright = false;
				} else {
					basemonsters = (struct monster*)realloc(basemonsters,sizeof(struct monster) * intNumMonsters);
					if (basemonsters == NULL) {blnError = true; return;}
					basemonsters[intNumMonsters - 1].x = x;
					basemonsters[intNumMonsters - 1].y = y;
					basemonsters[intNumMonsters - 1].living = true;
					basemonsters[0].movingright = false;
				}
			}
		}
	}
	monsters = (struct monster*)malloc(sizeof(struct monster) * intNumMonsters);
	if (monsters == NULL) {blnError = true; return;}
}
/**********************************************************************************************************************************************/
char CheckVerison(const char *ConfigVerison) {
	unsigned int P_MajorNum, P_MinorNum, P_PatchNum;
	unsigned int C_MajorNum, C_MinorNum, C_PatchNum;
	sscanf(version,"v%u.%u.%u",&P_MajorNum,&P_MinorNum,&P_PatchNum);
	sscanf(ConfigVerison,"v%u.%u.%u",&C_MajorNum,&C_MinorNum,&C_PatchNum);
	//printf("\nProgram: v %u %u %u \n",P_MajorNum,P_MinorNum,P_PatchNum);
	//printf("Config: v %u %u %u \n",C_MajorNum,C_MinorNum,C_PatchNum);
	if (P_MajorNum != C_MajorNum) {return 'N';}
	else if (P_MinorNum != C_MinorNum) {return 'P';}
	else {return 'U';}
}
/**********************************************************************************************************************************************/
void KillMonster(unsigned char xplace, unsigned char yplace) {
	for (unsigned int i = 0; i < intNumMonsters; i++) {
		if (monsters[i].x == xplace && monsters[i].y == yplace) {
			monsters[i].living = false;
		}
	}
}
/**********************************************************************************************************************************************/
