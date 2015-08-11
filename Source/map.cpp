/**********************************************************************************************************************************************/
#include "map.h"
#include "config.h"
#include "tick.h"
/**********************************************************************************************************************************************/
void Map::show(void) {
	Entity enty;
	Tick tick; 
	printf("\n\n\n\n");
	if (Global::blnDebugMode) {printf("Player found at (%d,%d)\n",enty.player.x,enty.player.y);}
	printf("Time Remaining: %d\n",tick.Clock);
	for (uchar y = 0; y < Map_Height; y++) {
		for (uchar x = enty.player.x - 5; x < enty.player.x + 73; x++) {
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
	printf("Generation: %2d 		Player: %2d 		Fitness: %2.3f\n", enty.genNum,enty.playerNum + 1,enty.player.fitness);
	tick.wait(); //waits for the time needed.
}
/**********************************************************************************************************************************************/
void Map::restart(void) {
	Entity enty;
	for (uint y = 0; y < Map_Height; y ++) {
		for (uint x = 0; x < Map_Width; x++) {
			map[y][x] = basemap[y][x];
		}
	}
	
	if (Global::blnDebugMode) {printf("Base map put into map.\n");}
	
	//MNSTR *pMonster = &enty.monsters;
	
	/*
	A quick note. I hate pointers I've never fully figured them out, and I kept
	having the program crash and burn no matter what I did, so I added placeBaseMonsters 
	and placeBasePlayer as a work around. But I need to change this to be proper pointers
	allocateMonsters probably also doesn't need to exist but I made it when I was debugging.
	so I'll change it later.
	*/
	
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
		/*
		enty.monsters[i].x = basemonsters[i].x;
		enty.monsters[i].y = basemonsters[i].y;
		enty.monsters[i].living = true;
		enty.monsters[i].movingright = false;
		
		if (Global::blnDebugMode) {printf("Finished Monster %d.\n",i);}*/
	}
	
	
	if (Global::blnDebugMode) {printf("Base monsters placed.\n");}
	
	enty.player.x = baseplayer.x;
	enty.player.y = baseplayer.y;
	//enty.player.x = 5;
	//enty.player.y = 11;
	enty.player.score = 0;
	enty.player.fitness = 0.00f;
	
	if (Global::blnDebugMode) {printf("Player reset.\n");}
	
	//printf("Player at (%d,%d) ",enty.player.x,enty.player.y);
}
/**********************************************************************************************************************************************/
char Map::move(uchar direction) {
	Entity enty;
	Config cnfg;
	static uchar jumpcount;
	static bool playerfalling;
	uchar tempx;
	uchar tempy;
	if (playerfalling == false) {jumpcount = 0;}
	
	//Move monsters first, to see if player dies and we can then skip the restart
	/*for (uchar i = 0; i < numMonsters; i++) {
		if (enty.monsters[i].living) {
			tempx = enty.monsters[i].x;
			tempy = enty.monsters[i].y;
			if (enty.monsters[i].movingright) {tempx ++;}
			else {tempx--;}
			
			if (map[tempy][tempx] == tileSpace) {
				map[tempy][enty.monsters[i].x] = tileSpace;
				map[tempy][tempx] = tileMonster;
				enty.monsters[i].x = tempx;
			}
			else if (map[tempy][tempx] == tilePlayer) {return DEAD;}
			else if (map[tempy][tempx] == tileWall) {enty.monsters[i].movingright = !(enty.monsters[i].movingright);}
			
			//Because I don't want to have to deal with monsters falling, if the space below a monster is space 
			//kill the monster.
			if (map[tempy+1][enty.monsters[i].x] == tileSpace ) {enty.monsters[i].living = false;}
			if(enty.monsters[i].living == false) {map[enty.monsters[i].y][enty.monsters[i].x] = tileSpace;}
		}
	} //End of for monsters
	*/
	
	//Now the player can move.
	
	tempx = enty.player.x;
	tempy = enty.player.y;
	
	switch (direction) {
		case dirLeft :
			if (tempx != 0) {tempx --;}
			break;
		case dirRight :
			if (tempx != Map_Width - 1) {tempx ++;}
			break;
		case dirUp :
			if (map[tempy][tempx] == tileWall) {break;}
			//make sure that the player has enough space to make the jump or the program will attempt to reference a 
			//non-existing array spot. Also note that jumping is consider in negative direction because the top of the 
			//map is considered array spot 0 while the bottom is 13.
			if (tempy > Jump_Height && jumpcount < Max_Jump_Count) {tempy -= Jump_Height; jumpcount++;}
			break;
	};
	if (enty.player.y == Map_Height - 1) {return DEAD;}//This makes the very last row of the arry a "kill plane"
	
	if (playerfalling) {tempy++;}
		if (map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {enty.killMonster(tempx,tempy); enty.player.score += MonsKill_Points;}
		else {return DEAD;}
	}
	
	if (map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (map[tempy][enty.player.x] != tileWall) {tempx = enty.player.x;}
		else if (map[enty.player.y][tempx] != tileWall) {tempy = enty.player.y;}
		else if (map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = enty.player.y; tempx = enty.player.x;}
	}
	
	if (map[tempy][tempx] == tileCoin) {enty.player.score += Coin_Points;}
	if (tempx < baseplayer.x - 2 && cnfg.values.blnHardMode) {return DEAD;} //if the player goes too far to the left kill them.
	
	map[enty.player.y][enty.player.x] = tileSpace;
	map[tempy][tempx] = tilePlayer;
	enty.player.x = tempx;
	enty.player.y = tempy;
	
	if ((enty.player.y == Map_Height - 1) || (map[enty.player.y + 1][enty.player.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}
	
	return LIVING;
}
/**********************************************************************************************************************************************/
void Map::load(void) {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.
	Entity enty;
	numMonsters = 0;
	for (uint y = 0; y < Map_Height; y++) {
		for (uint x = 0; x < Map_Width; x++) {
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
	
	enty.allocateMonsters(numMonsters);
	if (Global::blnError) {printf("Could not allocate the memory!\n"); return;}
}
/**********************************************************************************************************************************************/
Map::Map() {
	if (Global::blnDebugMode) {printf("Map Constructor called.\n");}
}
/**********************************************************************************************************************************************/
