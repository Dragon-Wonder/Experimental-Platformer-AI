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
	if (Global::blnDebugMode) {printf("Player found at (%d,%d)\n",Global::Enty.player.x, Global::Enty.player.y);}
	printf("Time Remaining: %d\n",Global::Tock.getClockTime());
	for (uchar y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uchar x = Global::Enty.player.x - 5; x < Global::Enty.player.x + 73; x++) {
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
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n", Global::Enty.genNum, Global::Enty.playerNum + 1, Global::Enty.player.fitness);
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
	
	if (basemonsters != NULL) {
		for (uchar i = 0; i < numMonsters; i++) {
			//Pointers are a pain
			//pMonster = &enty.monsters[i];
			//pMonster->[i]->x = basemonsters[i].x;
			//pMonster->[i]->y = basemonsters[i].y;
			//pMonster->[i]->living = basemonsters[i].living;
			//pMonster->[i]->movingright = basemonsters[i].movingright;
			//*enty.monsters[i].x = basemonsters[i].x;
			//*enty.monsters[i].y = basemonsters[i].y;
			//*enty.monsters[i].living = basemonsters[i].living;
			//*enty.monsters[i].movingright = basemonsters[i].movingright;
			//enty.placeBaseMonsters(i,basemonsters[i].x,basemonsters[i].y);
			
			Global::Enty.monsters[i].x = basemonsters[i].x;
			Global::Enty.monsters[i].y = basemonsters[i].y;
			Global::Enty.monsters[i].living = true;
			Global::Enty.monsters[i].movingright = false;
			
			if (Global::blnDebugMode) {printf("Finished Monster %d.\n",i);}
		}
		if (Global::blnDebugMode) {printf("Base monsters placed.\n");}
	} else {if (Global::blnDebugMode) {printf("Base monsters equals NULL.\n");}}
	
	Global::Enty.player.x = baseplayer.x;
	Global::Enty.player.y = baseplayer.y;
	Global::Enty.player.score = 0;
	Global::Enty.player.fitness = 0.00f;
	
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
	if (Global::Enty.monsters != NULL) {
		for (uchar i = 0; i < numMonsters; i++) {
			if (Global::Enty.monsters[i].living) {
				tempx = Global::Enty.monsters[i].x;
				tempy = Global::Enty.monsters[i].y;
				if (Global::Enty.monsters[i].movingright) {tempx ++;}
				else {tempx--;}
				
				if (map[tempy][tempx] == tileSpace) {
					map[tempy][Global::Enty.monsters[i].x] = tileSpace;
					map[tempy][tempx] = tileMonster;
					Global::Enty.monsters[i].x = tempx;
				}
				else if (map[tempy][tempx] == tilePlayer) {return DEAD;}
				else if (map[tempy][tempx] == tileWall) {Global::Enty.monsters[i].movingright = !(Global::Enty.monsters[i].movingright);}
				
				//Because I don't want to have to deal with monsters falling, if the space below a monster is space 
				//kill the monster.
				if (map[tempy+1][Global::Enty.monsters[i].x] == tileSpace ) {Global::Enty.monsters[i].living = false;}
				if(Global::Enty.monsters[i].living == false) {map[Global::Enty.monsters[i].y][Global::Enty.monsters[i].x] = tileSpace;}
			}
		} //End of for monsters
	} //End if not NULL
	
	//Now the player can move.
	
	tempx = Global::Enty.player.x;
	tempy = Global::Enty.player.y;
	
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
	if (Global::Enty.player.y == DEFINED_MAP_HEIGHT - 1) {return DEAD;}//This makes the very last row of the arry a "kill plane"
	
	if (playerfalling) {tempy++;}
		if (map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {Global::Enty.killMonster(tempx,tempy); Global::Enty.player.score += DEFINED_MONS_KILL_POINTS;}
		else {return DEAD;}
	}
	
	if (map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (map[tempy][Global::Enty.player.x] != tileWall) {tempx = Global::Enty.player.x;}
		else if (map[Global::Enty.player.y][tempx] != tileWall) {tempy = Global::Enty.player.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = Global::Enty.player.y; tempx = Global::Enty.player.x;}
	}
	
	if (map[tempy][tempx] == tileCoin) {Global::Enty.player.score += DEFINED_COIN_WORTH;}
	if (tempx < baseplayer.x - 2 && Global::Cnfg.values.blnHardMode) {return DEAD;} //if the player goes too far to the left kill them.
	
	map[Global::Enty.player.y][Global::Enty.player.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	Global::Enty.player.x = tempx;
	Global::Enty.player.y = tempy;
	
	if ((Global::Enty.player.y == DEFINED_MAP_HEIGHT - 1) || (map[Global::Enty.player.y + 1][Global::Enty.player.x] == tileSpace)) {playerfalling = true;}
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
