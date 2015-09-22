#ifndef __MAP__HEADER__
#define __MAP__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
#include <math.h>
#include <new>
/**********************************************************************************************************************************************/
#include "main.h"
/**********************************************************************************************************************************************/
enum tile {
	tileSpace = 0x0,
	tileBricksLarge = 0x1,
	tilePlayer = 0x2,
	tilePole = 0x3,
	tileMonster = 0x4,
	tileCoin = 0x5,
	tileBricksSmall = 0x6,
	tileBricksGray = 0x7,
	tileBricksGreen = 0x8,
	tileBricksOrange = 0x9,
	tileBricksRed = 0xA
};
/**********************************************************************************************************************************************/
struct stcBox { //Collision box
    uint left;
    uint right;
    uint top;
    uint bottom;
};

typedef struct stcBox BOX;
/**********************************************************************************************************************************************/
class clsMap {
    public:
	//private:
		//Members
		uchar basemap[DEFINED_MAP_HEIGHT][DEFINED_MAP_WIDTH];
		uchar map[DEFINED_MAP_HEIGHT][DEFINED_MAP_WIDTH];
		BPLYR bplyBasePlayer; //The base player, which just holds the x and y values where they start.
		MNSTR *pmstBaseMonsters;//the monsters on the map while restarting (see load if this is confusing)

	//public:
		//Default Constructor
		clsMap();
		~clsMap();

		//Members
		uchar numMonsters;

		//Functions
		char move(uchar);
		void load(void);
		//void show(void);
		void restart(void);

		uchar getMapCell(uint,uint);
		void setMapCell(uint,uint,uchar);

		BPLYR getbasePlayer(void);
		char checkCollision(LOC, uchar);
		bool checkOverlap(BOX,BOX);
};
/**********************************************************************************************************************************************/
#endif
/* __MAP__HEADER__ */
