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
#define DEFINED_GOAL_FPS 30

//The most steps a single player can take
#define DEFINED_MAX_PLAYER_STEPS 1000

//The number of players in each generation
#define DEFINED_PLAYERS_PER_GEN 40

//How many jumps the player has
#define DEFINED_MAX_JUMP_COUNT 2

//The points the player gets for collecting a coin
#define DEFINED_COIN_WORTH 100

//The points the player gets for killing a monster
#define DEFINED_MONS_KILL_POINTS 500

//How many seconds to complete the level
//Considering that the "player" does a move every tick
//Will be used when ported to a human version of the game.
#define DEFINED_TICK_LIMIT 300

//How many best players are called at the end of each generation
#define DEFINED_BEST_PLAYER_NUM 10

//How many different map tiles there are (used to make the array for clipping)
#define DEFINED_NUM_MAP_TILES 6

//Size of each tile in pixels
#define DEFINED_PIC_SIZE 24

//if human inputs are allowed or not
#define DEFINED_BUILD_HUMAN
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

struct stcVel {
    float x;
    float y;
}; //prefix = vel

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
	struct stcVel vel;
	float fitness;
	unsigned int score;
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS];
	unsigned char state;
}; //prefix = ply

struct stcBasePlayer {
	struct stcLoc location;
	struct stcVel vel;
}; //prefix = bply

// Structure for the monsters
struct stcMonster {
	struct stcLoc location;
	struct stcVel vel;
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
typedef struct stcVel VEL;
typedef struct stcBasePlayer BPLYR;

//Ahh laziness at its finest
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
/**********************************************************************************************************************************************/
#endif
