/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <windows.h>
//Tried to use nCurses but got really confused, so I'll leave it commented out until a later time
//#include <ncurses.h>
/**********************************************************************************************************************************************/
#include "platformer.h"
/**********************************************************************************************************************************************/
using namespace std;
/**********************************************************************************************************************************************/
extern const unsigned char basemap[Map_Height][Map_Width]; //in platformer.h
uchar map[Map_Height][Map_Width];
/**********************************************************************************************************************************************/
//Debug mode is defined in platformer.h, it just creates a boolean called blnDebugMode
//I chose to have it defined in the header so that it is unviersal if I change it one place
DEBUG_MODE
/**********************************************************************************************************************************************/
//These values are declared in config.cpp when config is loaded
extern bool blnLogging; //If player direction will be logged to a file
extern bool blnShowMap; //if the console shows all the steps the players take
extern bool blnAppendTimeToSeed; //If time is added to the seed 
extern bool blnError; //If there was an error found
extern bool blnHardMode; //if hard mode is on. Hard mode basically makes the players die a lot more.
extern uint InputFirstGen; //How many inputs the first generation has
extern uint InputGenIncrease; //How many inputs that each generation increases by until the limit of Max_Player_Steps
extern uint intGensPastGrowth; //How many generations take place with Max_Player_Steps
extern uint intPercentMutationChance; //Percent chance that when using directions from a previous player, the direction will be replace with a random one.
extern uint intSeed; //The seed that is used with srand()
/**********************************************************************************************************************************************/
GEN pastplayers[Players_Per_Generation]; //This will hold all of the generation's players' directions and fitness
GEN bestplayers[10]; //This will hold only the top 10 of the generation's players
PLYR player; //The current player things are happening for
PLYR baseplayer; //The base player, which mostly just holds the x and y values where they start. 
MNSTR *monsters; //all the monsters on the map
MNSTR *basemonsters; //the monsters on the map while restarting (see loadMap if this is confusing)
uchar generationNum = 0; //The current Generation number
uchar numplayers = 0; //The current player number
uchar intNumMonsters = 0; //the number of monsters
uint intGenerationSteps = 50; //The number of steps the LAST generation will use. 
//(I use last as opposed to current so I can double that a value greater than Max_Player_Steps is not being used)
//bool curses_started = false;
/**********************************************************************************************************************************************/
FILE* logfile;
/**********************************************************************************************************************************************/
int main() 
{
	//startCurses();
	CheckConfigFile(); //Load config file (see config.cpp)
	loadMap(); //Load the map
	ErrorCheck //Error check is defined in platformer.h, all it really does is sees if blnError is true
				//If it is write a message, pause the screen, then return 1.
				//Its point is to allow main to end if an error is generated in a function
	restartmap();
	showmap();
	getchar();
	
	//Seeds random with the values that were selected
	if (blnAppendTimeToSeed) {srand(time(NULL)+intSeed);}
	else {srand(intSeed);}
	
	char playerstatus = 0;
	if(blnShowMap) {showmap();}
	if (blnLogging) { /*Opens log file to clear it */logfile = fopen(LogFileName,"w"); fclose(logfile);}
	
	//First Generation of players
	for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
		if (numplayers != 0) {nextplayer();} //For every player besides the first one record the player before them
		for (uint i = 0; i < InputFirstGen; i++) {player.direction[i] = (uint)(rand() % (dirRight + 1) + dirUp);} //Generate random directions for the player
		ErrorCheck
		for(uint step = 0; step < InputFirstGen; step++){
			moveMonsters();
			playerstatus = moveplayer(step);
			if(blnShowMap) {showmap();}
			ErrorCheck
			if(playerstatus == DEAD) {
				//If the player dies, then clear the rest of thier directions and end the for loop.
				//for (uint j = step; j < InputFirstGen; j++) {player.direction[j] = dirNone;}
				step = InputFirstGen;
			}
		}
		if(blnShowMap) {showmap();}
	}
	nextplayer(); //For the last player of the generation.
	getbestplayers(); //Get the top players of the last generation in terms of fitness
	ErrorCheck
	if (!(blnShowMap)) {printf("Best Player fitnesses are:\n"); for (uchar j = 0; j < 10; j++) {printf("%2.3f\n",bestplayers[j].fitness);}}
	intGenerationSteps = InputFirstGen;
	
	//The Growth generation. Each Generation will have more steps until whatever the maxium steps is set as
	while (intGenerationSteps + InputGenIncrease <= Max_Player_Steps){
		numplayers = 0;
		generationNum++;
		for (numplayers = 0; numplayers < Players_Per_Generation; numplayers ++){
			if (numplayers != 0) {nextplayer();}
			generateNewGenPlayer(intGenerationSteps);
			ErrorCheck
			for(uint step = 0; step < intGenerationSteps + InputGenIncrease; step++){
				moveMonsters();
				playerstatus = moveplayer(step);
				if(blnShowMap) {showmap();}
				if(playerstatus == DEAD) {
					//If the player dies, then clear the rest of thier directions and end the for loop.
					//for (uint j = step; j < intGenerationSteps + InputGenIncrease; j++) {player.direction[j] = dirNone;}
					step = intGenerationSteps + InputGenIncrease;
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
					//If the player dies, then clear the rest of thier directions and end the for loop.
					//for (uint j = step; j < Max_Player_Steps; j++) {player.direction[j] = dirNone;}
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
	free(monsters); //Clear up the dynamic memory that was used.
	free(basemonsters);
	getchar();
	//endCurses();
	return 0;
}
/**********************************************************************************************************************************************/
void restartmap(){
	//Restarts the map by restating tiles, monster locations, and player location
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
	//Records the last player into the array and log it if that is enabled.
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
		for (uint j = 0; j < 2*Max_Player_Steps + 42; j++) {fprintf(logfile,"=");}
	}
	if(blnLogging) {fprintf(logfile,"\n"); fclose(logfile);}
	restartmap();
}
/**********************************************************************************************************************************************/
void showmap(){
	Sleep(Sleep_Time); //This is so the updating on the console doesn't look really bad.
						//As it is now it turns out pretty good with only a few issues.
	//for (uchar i = 0; i < Map_Height; i++) {printf("\n");}
	printf("\n\n\n\n");
	for(uchar y = 0; y < Map_Height; y++){
		for(uchar x = player.x - 5; x < player.x + 73; x++){
			//Basically this prints each tile as a different character
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
	//Print this at the bottom of the screen.
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n",generationNum,numplayers+1,player.fitness);
}
/**********************************************************************************************************************************************/
void generateNewGenPlayer(uint GenerationInputs){
	/*
	This is a pretty confusing function so I'll try to explain it best I can.
	First it generates a random number that it uses as a reference to a player in the best player array
	Next it randomly generates a Number that is the amout of steps it will take from that player 
	(I tried to make it so it wouldn't be more than half of the last generation's inputs)
	It will then go back and reapeat getting a new player and new section number until it has filled the direciton
	with however many steps the last generation took. If the generation is increasing (by being less than the maxium)
	then it will generate random directions until full.
	*/
	uchar RandPlayer;
	uint RandSection, StepNum = 0;
	do {
		RandPlayer = PickBestPlayer(); //PickBestPlayer randomly generates a player to use with a slight weigh on the higher fitnesses
		RandSection = (rand() % (GenerationInputs - StepNum)/2) + StepNum;
		if (blnDebugMode) {printf("Player: %d Section of %d\n",RandPlayer,RandSection);}
		for (uint i = StepNum; i < RandSection; i ++) {
			if ( (uint)(rand() % 100) < intPercentMutationChance) {player.direction[i] =  (uint)(rand() % (dirRight + 1) + dirUp);}
			else {player.direction[i] = bestplayers[RandPlayer].direction[i];}
		}
		StepNum += RandSection;
	} while (StepNum < GenerationInputs);
	if (GenerationInputs + InputGenIncrease < Max_Player_Steps) {
		for (uint i = GenerationInputs; i < GenerationInputs + InputGenIncrease; i++) {player.direction[i] = (uint)(rand() % (dirRight + 1) + dirUp);}
	}
}
/**********************************************************************************************************************************************/
float getfitness(uint step){
	//Calculates the fitness of the player.
	//If it is hard mode than the longer the player takes
	//the more the fitness will decrease
	//Also note that 204 is considered the "finish" line
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
	static uchar jumpcount; //counts how many jumps have been made since last on the ground
	static bool playerfalling;
	if (playerfalling == false) {jumpcount = 0;} //reset the jump count if the player lands on the ground.
	uchar tempx = player.x; //These are temporary values used to see if a player can move into the place they are trying to go
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
			//make sure that the player has enough space to make the jump or th program will attempt to reference a 
			//non-existing array spot. Also note that jumping is consider in negative direction because the top of the 
			//map is considered array spot 0 while the bottom is 13.
			if (player.y > Jump_Height && jumpcount < Max_Jump_Count ) {tempy -= Jump_Height; jumpcount++;}
			break;
	};
	if (player.y == Map_Height - 1) {return DEAD;} //This makes the very last row of the arry a "kill plane"
	if (playerfalling) {tempy ++;}
	if (map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {KillMonster(tempx,tempy); player.score += MonsKill_Points;}
		else {return DEAD;}
	}
	if (map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (map[tempy][player.x] != tileWall) {tempx = player.x;}
		else if (map[player.y][tempx] != tileWall) {tempy = player.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = player.y; tempx = player.x;}
	}
	if (map[tempy][tempx] == tileCoin) {player.score += Coin_Points;}
	if (tempx < baseplayer.x - 2 && blnHardMode) {return DEAD;} //if the player goes too far to the left kill them.
	
	map[player.y][player.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	player.x = tempx;
	player.y = tempy;
	
	//If hard mode always get a new fitness value, if not then the fitness value will only update if it is higher than the current one.
	if (blnHardMode) {player.fitness = getfitness(stepnum);}
	else {if (getfitness(stepnum) > player.fitness) {player.fitness = getfitness(stepnum);}}
	
	if (player.fitness < 0.00f && blnHardMode) {return DEAD;}
	
	//Check if falling
	if ((player.y == Map_Height - 1) || (map[player.y + 1][player.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}
	return LIVING; //The player LIVES!
}
/**********************************************************************************************************************************************/
void getbestplayers(){
	//Gets the players with the highest fitness in the last generation
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
	uchar RandomNumber = (uint)(rand() % 100);
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
	//Move the monsters
	//Monsters will move left until they hit a wall, then turn around, and turn back when they hit a wall again.
	//I didn't want to have to worry about the monster falling, so if the space below them is empty they die.
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
			else if (map[monsters[i].y][tempx] == tilePlayer) {player.y = Map_Height - 1;} //Moves the player to the kill plane so on the next check they are dead.
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
	//Finds the monster at specificed place and kills them
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