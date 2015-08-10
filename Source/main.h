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
#define PROGRAM_VERISON "v4.0.0-beta.5"
#define ErrorCheck if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
/**********************************************************************************************************************************************/
/*
These defines are for the size of the map. The default is 14 high by 217 width
You should note that if you change this you have to change the base map under map.h as well to the same
size or you will most likely run into a lot of issues. Later verisons where you can simply load your own map
will likely remove these and dynamically allocate the map size for you.
*/
#define Map_Height 14
#define Map_Width 217
/**********************************************************************************************************************************************/
//Defines that are to allow certain aspects to be changed easier.
//They can be changed without any issue.

//What the target FPS is for the program, only effects things when show map is true
//See tick.cpp for what it does
#define Goal_FPS 20

//The most steps a single player can take
#define Max_Player_Steps 1000

//The number of players in each generation
#define Players_Per_Generation 40

//How many jumps the player has
#define Max_Jump_Count 2

//How high the player will jump
#define Jump_Height 2

//The points the player gets for collecting a coin
#define Coin_Points 100

//The points the player gets for killing a monster
#define MonsKill_Points 500
/**********************************************************************************************************************************************/
//These are the names for some files used. Change them as you will.
#define ConfigFileName "Config.ini"
#define LogFileName "player.log"
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
	unsigned char direction[Max_Player_Steps];
};

struct person {
	unsigned char x;
	unsigned char y;
	float fitness;
	unsigned int score;
	unsigned char direction[Max_Player_Steps];
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
//Globals
namespace Global {
	extern const bool blnDebugMode;
	extern bool blnError;
	//char* programverison;
};
/**********************************************************************************************************************************************/
#endif 
