/**********************************************************************************************************************************************/
#include "map.h"
#include "config.h"
#include "tick.h"
#include "globals.h"
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
void clsMap::show(void) {

    /* TODO (Patrick.Rye#1#): Map does not appear correctly figure out why */


	printf("\n\n\n\n");
	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();
	uint x_start;


	if (tempPlayer.location.x - 5 <= 0) {x_start = 0;}
	else {x_start = tempPlayer.location.x - 5;}

	if (tempPlayer.location.x > DEFINED_MAP_WIDTH || tempPlayer.location.y > DEFINED_MAP_HEIGHT) {tempPlayer.location.x = 5; tempPlayer.location.y = 11;}

	if (Global::blnDebugMode) {printf("Player found at (%d,%d)\n",tempPlayer.location.x, tempPlayer.location.y);}
	//Player x and y starting at 16843009 for unknown reasons.
	printf("Time Remaining: %d\n",Global::Tick.getClockTime());
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uint x = x_start; x < tempPlayer.location.x + 73; x++) {
			switch (map[y][x]) {
				case tileSpace :
					printf(" ");
					break;
				case tileWall :
					printf("█");
					break;
				case tilePlayer :
					printf("@");
					break;
				case tilePole :
					printf("║");
					break;
				case tileMonster :
					printf("+");
					break;
				case tileCoin :
					printf("O");
					break;
				default :
					printf("#");
					break;
			}; //end switch
		}//end for x
		printf("\n");
	}//end for y
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n", Global::Enty.uchrGenNum, Global::Enty.uchrPlayerNum + 1, tempPlayer.fitness);
	Global::Tick.wait(); //waits for the time needed.
}
/**********************************************************************************************************************************************/
void clsMap::restart(void) {
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y ++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
            // TODO (Patrick.Rye#1#): Map does not seem to be properly set from basemap; ...
            //Figure out why.
			map[y][x] = basemap[y][x]; //Map does not appear to be set properly
			//Add a check?
		}
	}

	if (Global::blnDebugMode) {printf("Base map put into map.\n");}

	Global::Tick.resetClock();
	if (Global::blnDebugMode) {printf("Clock reset.\n");}

	MNSTR tempMonster;

	if (pmstBaseMonsters != nullptr) {
		for (uchar i = 0; i < numMonsters; i++) {

            /* TODO (Patrick.Rye#1#08/20/15): Figure out why program crashes when accessing basemonster 0 */
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
	static uchar jumpcount;
	static bool playerfalling;
	uchar tempx;
	uchar tempy;
	if (playerfalling == false) {jumpcount = 0;}

	//Move monsters first, to see if player dies and we can then skip the restart

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
			else if (map[tempy][tempx] == tilePlayer) {return DEAD;}
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

    /* TODO (Patrick.Rye#5#): Get rid of this line when I solve the issue of location being really high */
	//if (tempPlayer.location.x > DEFINED_MAP_WIDTH || tempPlayer.location.y > DEFINED_MAP_HEIGHT) {tempPlayer.location.x = 5; tempPlayer.location.y = 11;}


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
			//non-existing array spot. Also note that jumping is consider in negative direction because the top of the
			//map is considered array spot 0 while the bottom is 13.
			if (tempy > DEFINED_JUMP_HEIGHT && jumpcount < DEFINED_MAX_JUMP_COUNT) {tempy -= DEFINED_JUMP_HEIGHT; jumpcount++;}
			break;
	};
	if (tempPlayer.location.y == DEFINED_MAP_HEIGHT - 1) {return DEAD;}//This makes the very last row of the array a "kill plane"

	if (playerfalling) {tempy++;}
		if (map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {Global::Enty.killMonster(tempx,tempy); tempPlayer.score += DEFINED_MONS_KILL_POINTS;}
		else {return DEAD;}
	}

	if (map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (map[tempy][tempPlayer.location.x] != tileWall) {tempx = tempPlayer.location.x;}
		else if (map[tempPlayer.location.y][tempx] != tileWall) {tempy = tempPlayer.location.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = tempPlayer.location.y; tempx = tempPlayer.location.x;}
	}

	if (map[tempy][tempx] == tileCoin) {tempPlayer.score += DEFINED_COIN_WORTH;}
	if (tempx < locBasePlayer.x - 2 && (Global::Cnfg.getvalues(cnfgHardMode) == 1)) {return DEAD;} //if the player goes too far to the left kill them.

	map[tempPlayer.location.y][tempPlayer.location.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	tempPlayer.location.x = tempx;
	tempPlayer.location.y = tempy;

	Global::Enty.setPlayer(tempPlayer.location);

	if ((tempPlayer.location.y == DEFINED_MAP_HEIGHT - 1) || (map[tempPlayer.location.y + 1][tempPlayer.location.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}

	//Reduce the clock time and check if it equals 0 and kill the player if it does.
	Global::Tick.decClock();
	if (Global::Tick.getClockTime() == 0) {return DEAD;}

	return LIVING;
}
/**********************************************************************************************************************************************/
void clsMap::load(void) {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.

    /* TODO (Patrick.Rye#9#): Look into vectors for basemonsters */


	numMonsters = 0;
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
			if (basemap[y][x] == tilePlayer) {
				if (Global::blnDebugMode) {printf("Found Player at (%d,%d).\n",x,y);}
				locBasePlayer.x = x;
				locBasePlayer.y = y;
				//locBasePlayer.fitness = 0.0f;
				//locBasePlayer.score = 0;
			}
			else if (basemap[y][x] == tileMonster) {
				numMonsters++;
				//Consider making pmstBaseMonsters a vector for easier dynamic allocation
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
                        /* TODO (Patrick.Rye#5#08/20/15): Consider using std::copy from algorithm library */
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
MNSTR clsMap::getbaseMonster(uchar num) {
	MNSTR tempMNSTR;

	tempMNSTR.location.x = pmstBaseMonsters[num].location.x;
	tempMNSTR.location.y = pmstBaseMonsters[num].location.y;
	tempMNSTR.living = pmstBaseMonsters[num].living;
	tempMNSTR.movingright = pmstBaseMonsters[num].movingright;
	return tempMNSTR;
}
/**********************************************************************************************************************************************/
void clsMap::playerDeath(void) {
	//Plays short death animation
	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();

	show();
	for (uchar i = 0; i < DEFINED_GOAL_FPS; i++) {Global::Tick.wait();} //wait for a second.

	uint tempy = tempPlayer.location.y;

	for (uchar i = 0; i < 5; i++) {
		if (i < 3) {
			if (tempy != DEFINED_MAP_HEIGHT) { tempy--;}
		} else {
			if (tempy < DEFINED_MAP_HEIGHT - 2) {tempy+=2;}
		}

		map[tempPlayer.location.y][tempPlayer.location.x] = tileSpace;
		map[tempy][tempPlayer.location.x] = tilePlayer;

		tempPlayer.location.y = tempy;
		show();
	}
}
/**********************************************************************************************************************************************/
