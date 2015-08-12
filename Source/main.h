#ifndef __MAIN__HEADER__
#define __MAIN__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
/**********************************************************************************************************************************************/
/*
This is the main header and defines many values, and structures that are used a lot of other places in the code, so this header should be refrenced by
all the cpps
*/
/**********************************************************************************************************************************************/
//Best not to change these.
#define DEFINED_PROGRAM_VERSION "v4.0.0-beta.8"
#define ErrorCheck if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
/**********************************************************************************************************************************************/
/*
These defines are for the size of the map. The default is 14 high by 217 width
You should note that if you change this you have to change the base map under map.h as well to the same
size or you will most likely run into a lot of issues. Later verisons where you can simply load your own map
will likely remove these and dynamically allocate the map size for you.
*/
#define DEFINED_MAP_HEIGHT 14
#define DEFINED_MAP_WIDTH 217
/**********************************************************************************************************************************************/
//Defines that are to allow certain aspects to be changed easier.
//They can be changed without any issue.

//What the target FPS is for the program, only effects things when show map is true
//See tick.cpp for what it does
#define DEFINED_GOAL_FPS 20

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
#define DEFINED_TICK_LIMIT 5000

//How many best players are called at the end of each generation
#define DEFINED_BEST_PLAYER_NUM 10
/**********************************************************************************************************************************************/
//These are the names for some files used. Change them as you will.
#define DEFINED_CONFIG_FILE_NAME "Config.ini"
#define DEFINED_LOG_FILE_NAME "Player.log"
/**********************************************************************************************************************************************/
//These defines are just to make the code a little easier to read in terms of random Characters being returned and thier meaning
#define DEAD 'D'
#define LIVING 'L'
#define NEWCONFIG 'N'
#define USECONFIG 'U'
#define PROMPTUSER 'P'
/**********************************************************************************************************************************************/
//Structures for varies uses

struct generation {
	float fitness;
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS];
};

struct person {
	unsigned char x;
	unsigned char y;
	float fitness;
	unsigned int score;
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS];
};

struct creature {
	unsigned char x;
	unsigned char y;
	bool living;
	bool movingright;
};

enum dir {
	dirNone = 0,
	dirUp, //1
	dirLeft, //2
	dirRight, //3
	dirDown //4
};

typedef struct generation GEN;
typedef struct person PLYR;
typedef struct creature MNSTR;

//Ahh laziness at its finest
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
/**********************************************************************************************************************************************/
#endif 
