#ifndef __MAIN__HEADER__
#define __MAIN__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************/
/*
This is the main header and defines many values, and structures that are used a lot of other places in the code, so this header should be refrenced by
all the cpps
*/
/**********************************************************************************************************************************************/
/*
These defines are for the size of the map. The default is 14 high by 217 wide
You should note that if you change this you have to change the base map under map.h as well to the same
size or you will most likely run into a lot of issues. Later versions where you can simply load your own map
will likely remove these and dynamically allocate the map size for you.
*/
#define DEFINED_MAP_HEIGHT 14
#define DEFINED_MAP_WIDTH 217
/**********************************************************************************************************************************************/
//Defines that are to allow certain aspects to be changed easier.
//These can be changed without any issue.

//What the target FPS is for the program, only effects things when show map is true
//See tick.cpp for what it does
#define DEFINED_GOAL_FPS 15

//The most steps a single player can take
#define DEFINED_MAX_PLAYER_STEPS 1000

//The number of players in each generation
#define DEFINED_PLAYERS_PER_GEN 40

//How many jumps the player has
#define DEFINED_MAX_JUMP_COUNT 2

//How high the player will jump
#define DEFINED_JUMP_HEIGHT 2

//The points the player gets for collecting a coin
#define DEFINED_COIN_WORTH 100

//The points the player gets for killing a monster
#define DEFINED_MONS_KILL_POINTS 500

//How many ticks to complete the level
//Considering that the "player" does a move every tick
//This doesn't matter so long as it is greater than DEFINED_MAX_PLAYER_STEPS
//Will be used when ported to a human version of the game.
#define DEFINED_TICK_LIMIT 300

//How many best players are called at the end of each generation
#define DEFINED_BEST_PLAYER_NUM 10
/**********************************************************************************************************************************************/
//These are the names for some files used. Change them as you will.
#define DEFINED_CONFIG_FILE_NAME "Config.ini"
#define DEFINED_LOG_FILE_NAME "Player.log"
/**********************************************************************************************************************************************/
//These defines are just to make the code a little easier to read in terms of random Characters being returned and thier meaning

//These are used when checking the version of the config file.
#define NEWCONFIG 'N'
#define USECONFIG 'U'
#define PROMPTUSER 'P'
/**********************************************************************************************************************************************/
//Structures for varies uses

// Location of Monster / Player
// It is its own structure so that I can pass just the location
// as well as for the baseplayer which only needs to hold the location.
struct stcLoc {
	unsigned int x;
	unsigned int y;
}; //prefix = loc

// Structure for the storage of a generation
struct stcGeneration {
	float fitness;
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS];
}; //prefix = gen

// Structure for the player, only used for the current player
// because once they are done the important values are placed in
// the Generation structure.
struct stcPlayer {
	struct stcLoc location;
	float fitness;
	unsigned int score;
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS];
	unsigned char state;
}; //prefix = ply

// Structure for the monsters
struct stcMonster {
	struct stcLoc location;
	bool living; //This is checked before trying to move the monster
	unsigned char state; //Keeps track of their state.
}; //prefix = mst

enum dir {
	dirNone = 0,
	dirUp, //1
	dirLeft, //2
	dirRight, //3
	dirDown //4
};

typedef struct stcGeneration GEN;
typedef struct stcPlayer PLYR;
typedef struct stcMonster MNSTR;
typedef struct stcLoc LOC;

//Ahh laziness at its finest
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
/**********************************************************************************************************************************************/
#endif
