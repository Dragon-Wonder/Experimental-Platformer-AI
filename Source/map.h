#ifndef __MAP__HEADER__
#define __MAP__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
#include <cmath>
#include <new>
/**********************************************************************************************************************************************/
#include "main.h"
/**********************************************************************************************************************************************/
enum tile {
	tileSpace = 0,
	tileWall, //1
	tilePlayer, //2
	tilePole, //3
	tileMonster, //4
	tileCoin //5
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
// TODO (GamerMan7799#8#): Map loading.
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
