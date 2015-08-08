/**********************************************************************************************************************************************/
#include "map.h"
#include "config.h"
#include "tick.h"
/**********************************************************************************************************************************************/
void Map::show(void) {
	Entity enty;
	Tick tick; 
	printf("\n\n\n\n");
	for (uint y = 0; y < Map_Height; y++) {
		for (int x = enty.player.x - 5; x < enty.player.x + 73; x++) {
			switch (Map::map[y][x]) {
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
			Map::map[y][x] = Map::basemap[y][x];
		}
	}
	
	for (uchar i = 0; i < Map::numMonsters; i++) {
		enty.monsters[i].x = Map::basemonsters[i].x;
		enty.monsters[i].y = Map::basemonsters[i].y;
		enty.monsters[i].living = Map::basemonsters[i].living;
		enty.monsters[i].movingright = Map::basemonsters[i].movingright;
	}
	
	enty.player.x = Map::baseplayer.x;
	enty.player.y = Map::baseplayer.y;
	enty.player.score = 0;
	enty.player.fitness = 0.00;
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
	for (uchar i = 0; i < Map::numMonsters; i++) {
		if (enty.monsters[i].living) {
			tempx = enty.monsters[i].x;
			tempy = enty.monsters[i].y;
			if (enty.monsters[i].movingright) {tempx ++;}
			else {tempx--;}
			
			if (Map::map[tempy][tempx] == tileSpace) {
				Map::map[tempy][enty.monsters[i].x] = tileSpace;
				Map::map[tempy][tempx] = tileMonster;
				enty.monsters[i].x = tempx;
			}
			else if (Map::map[tempy][tempx] == tilePlayer) {return DEAD;}
			else if (Map::map[tempy][tempx] == tileWall) {enty.monsters[i].movingright = !(enty.monsters[i].movingright);}
			
			//Because I don't want to have to deal with monsters falling, if the space below a monster is space 
			//kill the monster.
			if (Map::map[tempy+1][enty.monsters[i].x] == tileSpace ) {enty.monsters[i].living = false;}
			if(enty.monsters[i].living == false) {Map::map[enty.monsters[i].y][enty.monsters[i].x] = tileSpace;}
		}
	} //End of for monsters
	
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
			if (Map::map[tempy][tempx] == tileWall) {break;}
			//make sure that the player has enough space to make the jump or the program will attempt to reference a 
			//non-existing array spot. Also note that jumping is consider in negative direction because the top of the 
			//map is considered array spot 0 while the bottom is 13.
			if (tempy > Jump_Height && jumpcount < Max_Jump_Count) {tempy -= Jump_Height; jumpcount++;}
			break;
	};
	if (enty.player.y == Map_Height - 1) {return DEAD;}//This makes the very last row of the arry a "kill plane"
	
	if (playerfalling) {tempy++;}
		if (Map::map[tempy][tempx] == tileMonster) {
		//If the player falls on a monster kill the monster,
		//but if the player is trying to walk into a monster kill the player
		if (playerfalling) {enty.killMonster(tempx,tempy); enty.player.score += MonsKill_Points;}
		else {return DEAD;}
	}
	
	if (Map::map[tempy][tempx] == tileWall){
		//This checks a few spots for an open area they can move into if they run into a wall
		if (Map::map[tempy][enty.player.x] != tileWall) {tempx = enty.player.x;}
		else if (Map::map[enty.player.y][tempx] != tileWall) {tempy = enty.player.y;}
		else if (Map::map[tempy + 1][tempx] != tileWall) {tempy += 1;}
		else {tempy = enty.player.y; tempx = enty.player.x;}
	}
	
	if (Map::map[tempy][tempx] == tileCoin) {enty.player.score += Coin_Points;}
	if (tempx < Map::baseplayer.x - 2 && cnfg.values.blnHardMode) {return DEAD;} //if the player goes too far to the left kill them.
	
	Map::map[enty.player.y][enty.player.x] = tileSpace;
	Map::map[tempy][tempx] = tilePlayer;
	enty.player.x = tempx;
	enty.player.y = tempy;
	
	if ((enty.player.y == Map_Height - 1) || (Map::map[enty.player.y + 1][enty.player.x] == tileSpace)) {playerfalling = true;}
	else {playerfalling = false;}
	
	return LIVING;
}
/**********************************************************************************************************************************************/
void Map::load(void) {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.
	Entity enty;
	Map::numMonsters = 0;
	for (uint y = 0; y < Map_Height; y++) {
		for (uint x = 0; x < Map_Width; x++) {
			if (Map::basemap[y][x] == tilePlayer) {
				Map::baseplayer.x = x;
				Map::baseplayer.y = y;
			}
			else if (Map::basemap[y][x] == tileMonster) {
				Map::numMonsters++;
				if (Map::numMonsters == 1) {
					Map::basemonsters = (MNSTR*)malloc(sizeof(MNSTR) * 1);
					if (Map::basemonsters == NULL) {Global::blnError = true; return;}
					Map::basemonsters[0].x = x;
					Map::basemonsters[0].y = y;
					Map::basemonsters[0].living = true;
					Map::basemonsters[0].movingright = false;
				} else {
					Map::basemonsters = (MNSTR*)realloc(Map::basemonsters,sizeof(MNSTR) * Map::numMonsters);
					if (Map::basemonsters == NULL) {Global::blnError = true;}
					Map::basemonsters[Map::numMonsters - 1].x = x;
					Map::basemonsters[Map::numMonsters - 1].y = y;
					Map::basemonsters[Map::numMonsters - 1].living = true;
					Map::basemonsters[Map::numMonsters - 1].movingright = false;
				}
			}
		}
	}
	enty.monsters = (MNSTR*) malloc (sizeof(MNSTR)* Map::numMonsters);
	if (enty.monsters == NULL) {Global::blnError = true; return;}
}
/**********************************************************************************************************************************************/