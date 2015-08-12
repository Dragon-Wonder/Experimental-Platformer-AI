/**********************************************************************************************************************************************/
#include "map.h"
#include "config.h"
#include "tick.h"
#include "globals.h"
/**********************************************************************************************************************************************/
Map::Map() {
	if (Global::blnDebugMode) {printf("Map Constructor called.\n");}
}
/**********************************************************************************************************************************************/
Map::~Map() {
	if(Global::blnDebugMode) {printf("Map Destructor called.\n");}
	free(basemonsters);
}
/**********************************************************************************************************************************************/
void Map::show(void) {
	printf("\n\n\n\n");
	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();
	uint x_start;
	
	
	if (tempPlayer.x - 5 <= 0) {x_start = 0;}
	else {x_start = tempPlayer.x - 5;}
	
	if (tempPlayer.x > DEFINED_MAP_WIDTH || tempPlayer.y > DEFINED_MAP_HEIGHT) {tempPlayer.x = 0; tempPlayer.y = 0;}
	
	if (Global::blnDebugMode) {printf("Player found at (%d,%d)\n",tempPlayer.x, tempPlayer.y);}
	printf("Time Remaining: %d\n",Global::Tock.getClockTime());
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uint x = x_start; x < tempPlayer.x + 73; x++) {
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
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n", Global::Enty.genNum, Global::Enty.playerNum + 1, tempPlayer.fitness);
	Global::Tock.wait(); //waits for the time needed.
}
/**********************************************************************************************************************************************/
void Map::restart(void) {
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y ++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
			map[y][x] = basemap[y][x];
		}
	}
	
	if (Global::blnDebugMode) {printf("Base map put into map.\n");}
	
	Global::Tock.resetClock();
	if (Global::blnDebugMode) {printf("Clock reset.\n");}
	
	//MNSTR *pMonster = &enty.monsters;
	
	/*
	A quick note. I hate pointers I've never fully figured them out, and I kept
	having the program crash and burn no matter what I did, so I added placeBaseMonsters 
	and placeBasePlayer as a work around. But I need to change this to be proper pointers
	allocateMonsters probably also doesn't need to exist but I made it when I was debugging.
	so I'll change it later.
	*/
	
	MNSTR tempMonster;
	
	if (basemonsters != NULL) {
		for (uchar i = 0; i < numMonsters; i++) {
			
			tempMonster.x = basemonsters[i].x;
			tempMonster.y = basemonsters[i].y;
			tempMonster.living = basemonsters[i].living;
			tempMonster.movingright = basemonsters[i].movingright;
			
			Global::Enty.setMonster(i,tempMonster);
			
			if (Global::blnDebugMode) {printf("Finished Monster %d.\n",i);}
		}
		if (Global::blnDebugMode) {printf("Base monsters placed.\n");}
	} else {if (Global::blnDebugMode) {printf("Base monsters equals NULL.\n");}}
	
	Global::Enty.setPlayer(baseplayer);
	
	if (Global::blnDebugMode) {printf("Player reset.\n");}
	
	//printf("Player at (%d,%d) ",enty.player.x,enty.player.y);
}
/**********************************************************************************************************************************************/
char Map::move(uchar direction) {
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
			tempx = tempMonster.x;
			tempy = tempMonster.y;
			if (tempMonster.movingright) {tempx ++;}
			else {tempx--;}
			
			if (map[tempy][tempx] == tileSpace) {
				map[tempy][tempMonster.x] = tileSpace;
				map[tempy][tempx] = tileMonster;
				tempMonster.x = tempx;
			}
			else if (map[tempy][tempx] == tilePlayer) {return DEAD;}
			else if (map[tempy][tempx] == tileWall) {tempMonster.movingright = !(tempMonster.movingright);}
			
			//Because I don't want to have to deal with monsters falling, if the space below a monster is space 
			//kill the monster.
			if (map[tempy+1][tempMonster.x] == tileSpace ) {tempMonster.living = false;}
			if(tempMonster.living == false) {map[tempMonster.y][tempMonster.x] = tileSpace;}
		} //end if living
		Global::Enty.setMonster(i,tempMonster);
	} //End of for monsters

	
	//Now the player can move.
	
	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();
	
	tempx = tempPlayer.x;
	tempy = tempPlayer.y;
	
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
	if (tempPlayer.y == DEFINED_MAP_HEIGHT - 1) {return DEAD;}//This makes the very last row of the array a "kill plane"
	
	if (playerfalling) {tempy++;}
		if (map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {Global::Enty.killMonster(tempx,tempy); tempPlayer.score += DEFINED_MONS_KILL_POINTS;}
		else {return DEAD;}
	}
	
	if (map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (map[tempy][tempPlayer.x] != tileWall) {tempx = tempPlayer.x;}
		else if (map[tempPlayer.y][tempx] != tileWall) {tempy = tempPlayer.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = tempPlayer.y; tempx = tempPlayer.x;}
	}
	
	if (map[tempy][tempx] == tileCoin) {tempPlayer.score += DEFINED_COIN_WORTH;}
	if (tempx < baseplayer.x - 2 && (Global::Cnfg.getvalues(cnfgHardMode) == 1)) {return DEAD;} //if the player goes too far to the left kill them.
	
	map[tempPlayer.y][tempPlayer.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	tempPlayer.x = tempx;
	tempPlayer.y = tempy;
	
	Global::Enty.setPlayer(tempPlayer);
	
	if ((tempPlayer.y == DEFINED_MAP_HEIGHT - 1) || (map[tempPlayer.y + 1][tempPlayer.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}
	
	//Reduce the clock time and check if it equals 0 and kill the player if it does.
	Global::Tock.decClock();
	if (Global::Tock.getClockTime() == 0) {return DEAD;}
	
	return LIVING;
}
/**********************************************************************************************************************************************/
void Map::load(void) {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.
	numMonsters = 0;
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
			if (basemap[y][x] == tilePlayer) {
				if (Global::blnDebugMode) {printf("Found Player at (%d,%d).\n",x,y);}
				baseplayer.x = x;
				baseplayer.y = y;
				baseplayer.fitness = 0.0;
				baseplayer.score = 0;
			}
			else if (basemap[y][x] == tileMonster) {
				numMonsters++;
				if (Global::blnDebugMode) {printf("Found Monster at (%d,%d)\n",x,y);}
				if (numMonsters == 1) {
					basemonsters = (MNSTR*)malloc(sizeof(MNSTR) * 1);
					if (basemonsters == NULL) {Global::blnError = true; return;}
					basemonsters[0].x = x;
					basemonsters[0].y = y;
					basemonsters[0].living = true;
					basemonsters[0].movingright = false;
				} else {
					basemonsters = (MNSTR*)realloc(basemonsters,sizeof(MNSTR) * numMonsters);
					if (basemonsters == NULL) {Global::blnError = true; return;}
					basemonsters[numMonsters - 1].x = x;
					basemonsters[numMonsters - 1].y = y;
					basemonsters[numMonsters - 1].living = true;
					basemonsters[numMonsters - 1].movingright = false;
				} //end if first or not
			} //end if monster tile
		} //end for width
	} //end for height
	if (Global::blnDebugMode) {printf("Found %d Monsters.\n",numMonsters);}
	
	Global::Enty.allocateMonsters(numMonsters);
	if (Global::blnError) {printf("Could not allocate the memory!\n"); return;}
}
/**********************************************************************************************************************************************/
uchar Map::getMapCell(uint x, uint y) {
	return map[y][x];
}
/**********************************************************************************************************************************************/
void Map::setMapCell(uint x, uint y, uchar tile) {
	map[y][x] = tile;
}
/**********************************************************************************************************************************************/
PLYR Map::getbasePlayer(void) {
	return baseplayer;
}
/**********************************************************************************************************************************************/
MNSTR Map::getbaseMonster(uchar num) {
	MNSTR tempMNSTR;
	
	tempMNSTR.x = basemonsters[num].x;
	tempMNSTR.y = basemonsters[num].y;
	tempMNSTR.living = basemonsters[num].living;
	tempMNSTR.movingright = basemonsters[num].movingright;
	return tempMNSTR;
}
/**********************************************************************************************************************************************/
