#ifndef __MAIN__HEADER__
#define __MAIN__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
/**********************************************************************************************************************************************/
//Best not to change these.
#define PROGRAM_VERISON "v4.0.0-alpha.2"
#define ErrorCheck if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
/**********************************************************************************************************************************************/
//Defines that are to allow certain aspects to be changed easier.
//They can be changed without any issue.
#define Map_Height 14
#define Map_Width 217
#define Max_Player_Steps 1000
#define Players_Per_Generation 40
#define Sleep_Time 65
#define Max_Jump_Count 2
#define Jump_Height 2
#define Coin_Points 100
#define MonsKill_Points 500
/**********************************************************************************************************************************************/
//These are the names for some files used. Change them as you will.
#define ConfigFileName "Config.ini"
#define LogFileName "player.log"
/**********************************************************************************************************************************************/
//These defines are just to make the code a little easier to read in terms of random Characters being return and thier meaning
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
/**********************************************************************************************************************************************/
//Globals
namespace Global {
	const bool blnDebugMode = false;
	bool blnError = false;
	//const char* programverison = PROGRAM_VERISON;
};
/**********************************************************************************************************************************************/
#endif 