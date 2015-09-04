/**********************************************************************************************************************************************/
#include "map.h"
#include "entity.h"
#include "config.h"
#include "tick.h"
#include "globals.h"
/* TODO (GamerMan7799#9#): Have Monster and player movements based off of velocity instead of set cells. Every
                           update would calculate how far they go based on time that has passed (like I did for the Cannon Simulation).
                           Inputs will add / subtract velocities. I will first have to figure out how to detect where tile bounding boxes are.
                           It will be a lot of work will likely be part of the next major release v5.0.0*/
/**********************************************************************************************************************************************/
clsMap::clsMap() {
	if (Global::blnDebugMode) {printf("Map Constructor called.\n");}
}
/**********************************************************************************************************************************************/
clsMap::~clsMap() {
	if(Global::blnDebugMode) {printf("Map Destructor called.\n");}
	delete[] pmstBaseMonsters;
}
/**********************************************************************************************************************************************/
void clsMap::restart(void) {
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y ++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
			map[y][x] = basemap[y][x];
		}
	}
	if (Global::blnDebugMode) {printf("Base map put into map.\n");}

	Global::Tick.resetClock();
	if (Global::blnDebugMode) {printf("Clock reset.\n");}

	MNSTR tempMonster;

	if (pmstBaseMonsters != nullptr) {
		for (uchar i = 0; i < numMonsters; i++) {
			tempMonster.location.x = pmstBaseMonsters[i].location.x;
			tempMonster.location.y = pmstBaseMonsters[i].location.y;
			tempMonster.living = pmstBaseMonsters[i].living;
			tempMonster.movingright = pmstBaseMonsters[i].movingright;
			Global::Enty.setMonster(i,tempMonster);
			if (Global::blnDebugMode) {printf("Finished Monster %d.\n",i);}
		}
		if (Global::blnDebugMode) {printf("Base monsters placed.\n");}
	} else {if (Global::blnDebugMode) {printf("Base monsters equals nullptr.\n");}}
	Global::Enty.setPlayer(locBasePlayer);
	if (Global::blnDebugMode) {printf("Player reset.\n");}
}
/**********************************************************************************************************************************************/
char clsMap::move(uchar direction) {
	static uchar jumpcount; //how many times the player has jumped
	static bool playerfalling; //if the player is falling
	uchar tempx;
	uchar tempy;
	if (playerfalling == false) {jumpcount = 0;}

	//Move monsters first, to see if player dies and we can then skip the rest

	MNSTR tempMonster;

	for (uchar i = 0; i < numMonsters; i++) {
		tempMonster = Global::Enty.getMonster(i);
		if (tempMonster.living) {
			tempx = tempMonster.location.x;
			tempy = tempMonster.location.y;
			if (tempMonster.movingright) {tempx ++;}
			else {tempx--;}

			if (map[tempy][tempx] == tileSpace) {
				map[tempy][tempMonster.location.x] = tileSpace;
				map[tempy][tempx] = tileMonster;
				tempMonster.location.x = tempx;
			}
			else if (map[tempy][tempx] == tilePlayer) {return deathMonster;}
			else if (map[tempy][tempx] == tileWall) {tempMonster.movingright = !(tempMonster.movingright);}

			//Because I don't want to have to deal with monsters falling, if the space below a monster is space
			//kill the monster.
			if (map[tempy+1][tempMonster.location.x] == tileSpace ) {tempMonster.living = false;}
			if(tempMonster.living == false) {map[tempMonster.location.y][tempMonster.location.x] = tileSpace;}
		} //end if living
		Global::Enty.setMonster(i,tempMonster);
	} //End of for monsters


	//Now the player can move.

	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();
    //Kill Player if their fitness gets too low in hard mode
    if (tempPlayer.fitness < -2.5 && (Global::Cnfg.getvalues(cnfgHardMode) == 1) ) {return deathDecay;}

	tempx = tempPlayer.location.x;
	tempy = tempPlayer.location.y;

	switch (direction) {
		case dirLeft :
			if (tempx != 0) {tempx --;}
			break;
		case dirRight :
			if (tempx != DEFINED_MAP_WIDTH - 1) {tempx ++;}
			break;
		case dirUp :
			if (map[tempy][tempx] == tileWall) {break;}
			//make sure that the player has enough space to make the jump or the program will attempt to reference a
			//non-existing array spot (crashing the program). Also note that jumping is consider in negative direction because the top of the
			//map is considered array spot 0 while the bottom is 13.
			if (tempy > DEFINED_JUMP_HEIGHT && jumpcount < DEFINED_MAX_JUMP_COUNT) {tempy -= DEFINED_JUMP_HEIGHT; jumpcount++;}
			break;
	};
	if (tempPlayer.location.y == DEFINED_MAP_HEIGHT - 1) {return deathFall;}//This makes the very last row of the array a "kill plane"

	if (playerfalling) {tempy++;}
		if (map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {Global::Enty.killMonster(tempx,tempy); tempPlayer.score += DEFINED_MONS_KILL_POINTS;}
		else {return deathMonster;}
	}

	if (map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (map[tempy][tempPlayer.location.x] != tileWall) {tempx = tempPlayer.location.x;}
		else if (map[tempPlayer.location.y][tempx] != tileWall) {tempy = tempPlayer.location.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = tempPlayer.location.y; tempx = tempPlayer.location.x;}
	}

	if (map[tempy][tempx] == tileCoin) {tempPlayer.score += DEFINED_COIN_WORTH;}
	if (tempx < locBasePlayer.x - 2 && (Global::Cnfg.getvalues(cnfgHardMode) == 1)) {return deathStupid;} //if the player goes too far to the left kill them.

	map[tempPlayer.location.y][tempPlayer.location.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	tempPlayer.location.x = tempx;
	tempPlayer.location.y = tempy;

	Global::Enty.setPlayer(tempPlayer.location);

	if ((tempPlayer.location.y == DEFINED_MAP_HEIGHT - 1) || (map[tempPlayer.location.y + 1][tempPlayer.location.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}

	//Reduce the clock time and check if it equals 0 and kill the player if it does.
	Global::Tick.decClock();
	if (Global::Tick.getClockTime() == 0) {return deathClock;}

	return statusLiving;
}
/**********************************************************************************************************************************************/
void clsMap::load(void) {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.

    /* TODO (xPUREx#5#): Look into vectors for basemonsters */
	numMonsters = 0;
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
			if (basemap[y][x] == tilePlayer) {
				if (Global::blnDebugMode) {printf("Found Player at (%d,%d).\n",x,y);}
				locBasePlayer.x = x;
				locBasePlayer.y = y;
			}
			else if (basemap[y][x] == tileMonster) {
				numMonsters++;
				if (Global::blnDebugMode) {printf("Found Monster at (%d,%d)\n",x,y);}
				if (numMonsters == 1) {
					pmstBaseMonsters = new (std::nothrow) MNSTR[1];
					if (pmstBaseMonsters == nullptr) {
						//Could not allocate the memory
						Global::blnError = true;
						return;
					}
					pmstBaseMonsters[0].location.x = x;
					pmstBaseMonsters[0].location.y = y;
					pmstBaseMonsters[0].living = true;
					pmstBaseMonsters[0].movingright = false;
				} else {
					//Allocate a temp array to hold the old array + the new monster
					MNSTR* pTemp = new (std::nothrow) MNSTR[numMonsters];
					if (pTemp == nullptr) {
						//Could not allocate the memory
						Global::blnError = true;
						return;
					}

					for (uchar i = 0; i < numMonsters - 1; i++) {
                        /* TODO (GamerMan7799#5#): Consider using std::copy from algorithm library
                            Do not worry about if xPUREx converts to vectors.*/
						//std::copy(pmstBaseMonsters, pmstBaseMonsters + numMonsters - 1, pTemp);
						pTemp[i].location.x = pmstBaseMonsters[i].location.x;
						pTemp[i].location.y = pmstBaseMonsters[i].location.y;
						pTemp[i].living = pmstBaseMonsters[i].living;
						pTemp[i].movingright = pmstBaseMonsters[i].movingright;
					}
					//Delete old array as it isn't needed.
					delete[] pmstBaseMonsters;

					//Place newest Monster values into temp array.
					pTemp[numMonsters - 1].location.x = x;
					pTemp[numMonsters - 1].location.y = y;
					pTemp[numMonsters - 1].living = true;
					pTemp[numMonsters - 1].movingright = false;

					//Generate new Base Monster array
					pmstBaseMonsters = new (std::nothrow) MNSTR[numMonsters];
					if (pmstBaseMonsters == nullptr) {
						//Could not allocate the memory
						Global::blnError = true;
						return;
					}

					for (uchar i = 0; i < numMonsters; i++) {
						//Copy Temp Array into new array.
						pmstBaseMonsters[i].location.x = pTemp[i].location.x;
						pmstBaseMonsters[i].location.y = pTemp[i].location.y;
						pmstBaseMonsters[i].living = pTemp[i].living;
						pmstBaseMonsters[i].movingright = pTemp[i].movingright;
					}

					//Delete Temp Array
					delete [] pTemp;

				} //end if first or not
			} //end if monster tile
		} //end for width
	} //end for height
	if (Global::blnDebugMode) {printf("Found %d Monsters.\n",numMonsters);}

	Global::Enty.allocateMonsters(numMonsters);
	if (Global::blnError) {printf("Could not allocate the memory!\n"); return;}
}
/**********************************************************************************************************************************************/
uchar clsMap::getMapCell(uint x, uint y) {
	return map[y][x];
}
/**********************************************************************************************************************************************/
void clsMap::setMapCell(uint x, uint y, uchar tile) {
	map[y][x] = tile;
}
/**********************************************************************************************************************************************/
LOC clsMap::getbasePlayer(void) {
	return locBasePlayer;
}
/**********************************************************************************************************************************************/
