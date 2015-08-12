/**********************************************************************************************************************************************/
#include "entity.h"
#include "config.h"
#include "map.h"
#include "globals.h"
/**********************************************************************************************************************************************/
/*
This will hold everything related to the entities that have to be kept track of including the players, monsters, past players, etc...
Later on we might spilt up players and monsters but since monsters only need to be stored at the moment I didn't see a need to make them
their own .cpp
*/
/**********************************************************************************************************************************************/
Entity::Entity() {
	if (Global::blnDebugMode) {printf("Entity Constructor called.\n");}
	//Default Constructor
	genNum = 0;
	playerNum = 0;
	uintGenSteps = 0;
	
	player.x = 5;
	player.y = 11;
	player.fitness = 0.00f;
	player.score = 0;
}
/**********************************************************************************************************************************************/
Entity::~Entity() {
	if(Global::blnDebugMode) {printf("Entity Destructor called.\n");}
	free(monsters);
}
/**********************************************************************************************************************************************/
void Entity::start(void) {
	//Starts the entire loop.
	
	if (Global::Cnfg.values.blnLogging) {/*Open log file to clear it*/ logFile = fopen(FileName,"w"); fclose(logFile);}
	
	char chrPlayerStatus = 0;
	uintGenSteps = 0;
	genNum = 1;
	
	//First Generation
	for (playerNum = 0; playerNum < DEFINED_PLAYERS_PER_GEN; playerNum++) {
		makeplayer();
		for (uint step = 0; step < Global::Cnfg.values.uintFirstGen; step++) {
			chrPlayerStatus = Global::Course.move(player.direction[step]);
			player.fitness = getFitness();
			if (Global::Cnfg.values.blnShowMap) {Global::Course.show();}
			if (chrPlayerStatus == DEAD) {
				//If the player dies clear the rest of their directions (disabled) and end the loop.
				//for (uint j = step; j < Global::Cnfg.values.uintFirstGen; j++) {player.direction[j] = dirNone;}
				step = Global::Cnfg.values.uintFirstGen;
			} //end if dead
		} //End for steps
		nextplayer();
		if (Global::Cnfg.values.blnShowMap) {Global::Course.show();}
	}//end for first gen
	
	getBest();
	if(!(Global::Cnfg.values.blnShowMap)) {
		printf("Best Players are:\n");
		for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",bestplayers[k].fitness);}
		if (Global::blnDebugMode) {getchar();}
	}
	
	uintGenSteps += Global::Cnfg.values.uintFirstGen;
	
	//Growth Generation
	while (uintGenSteps + Global::Cnfg.values.uintGenIncrease <= DEFINED_MAX_PLAYER_STEPS) {
		genNum ++;
		for (playerNum = 0; playerNum < DEFINED_PLAYERS_PER_GEN; playerNum++) {
			makeplayer();
			for (uint step = 0; step < uintGenSteps + Global::Cnfg.values.uintGenIncrease; step++) {
				chrPlayerStatus = Global::Course.move(player.direction[step]);
				player.fitness = getFitness();
				if (Global::Cnfg.values.blnShowMap) {Global::Course.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of their directions (disabled) and end the loop.
					//for (uint j = step; j < uintGenSteps + Global::Cnfg.values.uintGenIncrease; j++) {player.direction[j] = dirNone;}
					step = uintGenSteps + Global::Cnfg.values.uintGenIncrease;
				} //end if dead
			}//end for steps
			nextplayer();
			if (Global::Cnfg.values.blnShowMap) {Global::Course.show();}
		} //end for players
		getBest();
		if(!(Global::Cnfg.values.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",bestplayers[k].fitness);}
			if (Global::blnDebugMode) {getchar();}
		}
		uintGenSteps += Global::Cnfg.values.uintGenIncrease;
	}//end while loop
	
	//Steady Generations
	for (uint i = 0; i < Global::Cnfg.values.uintGensPastGrowth; i++) {
		genNum ++;
		for (playerNum = 0; playerNum < DEFINED_PLAYERS_PER_GEN; playerNum++) {
			makeplayer();
			for (uint step = 0; step < DEFINED_MAX_PLAYER_STEPS; step++) {
				chrPlayerStatus = Global::Course.move(player.direction[step]);
				player.fitness = getFitness();
				if (Global::Cnfg.values.blnShowMap) {Global::Course.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of their directions (disabled) and end the loop.
					//for (uint j = step; j < DEFINED_MAX_PLAYER_STEPS; j++) {player.direction[j] = dirNone;}
					step = DEFINED_MAX_PLAYER_STEPS;
				} //end if dead
			}//end for steps
			nextplayer();
			if (Global::Cnfg.values.blnShowMap) {Global::Course.show();}
		} //end for players
		getBest();
		if(!(Global::Cnfg.values.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",bestplayers[k].fitness);}
			if (Global::blnDebugMode) {getchar();}
		}
	}//end for loop	
}
/**********************************************************************************************************************************************/
void Entity::nextplayer(void) {
	//Records the last player into the array of players, and logs directions to file if that is enabled.
	if (Global::Cnfg.values.blnLogging) {logFile = fopen(FileName,"a");} //Open log file in append mode.
	if (Global::blnDebugMode) {printf("Player finished with fitness: %2.3f\n",player.fitness); getchar(); }
	
	pastplayers[playerNum].fitness = player.fitness;
	if (Global::Cnfg.values.blnLogging) {fprintf(logFile,"Generation: %2d, Player: %2d, Fitness: %2.2f",genNum,playerNum + 1,player.fitness);}
	
	player.fitness = 0.00f;
	player.score = 0;
	
	for (uint i = 0; i < DEFINED_MAX_PLAYER_STEPS; i++) {
		pastplayers[playerNum].direction[i] = player.direction[i];
		if (Global::Cnfg.values.blnLogging) {
			switch (player.direction[i]) {
				case dirNone :
					//Do nothing
					break;
				case dirUp :
					fprintf(logFile," ↑");
					break;
				case dirLeft :
					fprintf(logFile, " ←");
					break;
				case dirRight :
					fprintf(logFile, " →");
					break;
				case dirDown :
					fprintf(logFile, " ↓");
					break;
				default :
					fprintf(logFile, " ?");
					break;
			} //End Switch
		}// End logging if
	}//end for
	
	if (Global::Cnfg.values.blnLogging) {
		fprintf(logFile, "\n");
		if (playerNum + 1 == DEFINED_PLAYERS_PER_GEN) {
			//If this is the last player add a line to better separate the different generations
			//in the log file. The line will be as long as the longest possible string of directions
			
			for (uint j = 0; j < 2* DEFINED_MAX_PLAYER_STEPS + 42; j++) {fprintf(logFile, "=");}
			fprintf(logFile, "\n");
		} //End of if last gen player
		fclose(logFile);
	} //end of if logging
	Global::Course.restart();
}
/**********************************************************************************************************************************************/
void Entity::makeplayer(void) {
	/*
	This is a pretty confusing function so I'll try to explain it best I can.
	
	It first checks to see what step of the program we are in, First, Growth or Steady. 
	This changes how it makes the player.

	If its in the First phase. It will just generate rand directions for the player until it hits 
	the cap specified in the config file. With directions being Up, Right or Left (down does nothing at the moment)
	
	During the growth phase and steady phases:
	First it generates a random number that it uses as a reference to a player in the best player array
	Next it randomly generates a Number that is the amount of steps it will take from that player 
	(I tried to make it so it wouldn't be more than half of the last generation's inputs)
	It will then go back and repeat getting a new player and new section number until it has filled the directions
	with however many steps the last generation took. If the generation is increasing (by being less than the maximum)
	then it will generate random directions until full.
	*/
	uchar uchrRandPlayer;
	uint uRandSection, uTempStep = 0;
	
	if (genNum == 1) { //First Generation
		for (uint i = 0; i < Global::Cnfg.values.uintFirstGen; i++) {player.direction[i] = (uint)(rand() % (dirRight) + dirUp);}
		for (uint i = Global::Cnfg.values.uintFirstGen; i < DEFINED_MAX_PLAYER_STEPS; i++ ) {player.direction[i] = dirNone;}
	} else { //Growth Phase & Steady phase
		do {
			uchrRandPlayer = rand() % DEFINED_BEST_PLAYER_NUM;
			uRandSection = (uint)(rand() % ((uintGenSteps - uTempStep)) + uTempStep);
			if (Global::blnDebugMode) {printf("Player %d Section of %d\n",uchrRandPlayer,uRandSection);}
			for (uint j = uTempStep; j <= uRandSection; j++) {
				if ((uint)(rand() % 100) < Global::Cnfg.values.uintMutationChance) {player.direction[j] = (uint)(rand() % (dirRight) + dirUp);}
				else {player.direction[j] = bestplayers[uchrRandPlayer].direction[j];}
			}//End for
			uTempStep = uRandSection;
		} while (uTempStep < uintGenSteps - 1);
		
		if (uintGenSteps + Global::Cnfg.values.uintGenIncrease < DEFINED_MAX_PLAYER_STEPS) {
			for (uint k = 0; k < uintGenSteps + Global::Cnfg.values.uintGenIncrease; k++) {player.direction[k] = (uint)(rand() % (dirRight) + dirUp);}
		}
	}
}
/**********************************************************************************************************************************************/
float Entity::getFitness(void) {
	/* Calculates the fitness of the player.
	If it is hard mode then the longer the player takes
	the more fitness will decrease
	Note that 204 is considered the "finish" line. 
	*/
	float temp = 0.00f;
	temp += (player.score) / 250.0;
	temp += (5.0/2.0) * (player.x - Global::Course.baseplayer.x);
	temp += (player.x + player.y) / 6.0;
	temp += (Global::Course.baseplayer.y - player.y) / 4.0;
	if (player.x > 204) {temp += 200.0;}
	if (Global::Cnfg.values.blnHardMode) {temp -= uintStepNum / 80.0;}
	return temp;
}
/**********************************************************************************************************************************************/
void Entity::getBest(void) {
	//Gets best players in a generation.
	float fTempfit = 0;
	uchar uchrBestNum = 0;
	for (uchar j = 0; j < DEFINED_BEST_PLAYER_NUM; j ++) {
		fTempfit = 0.00f;
		uchrBestNum = 0;
		for (uchar i = 0; i < DEFINED_PLAYERS_PER_GEN; i++) {
			if (pastplayers[i].fitness > fTempfit) {fTempfit = pastplayers[i].fitness; uchrBestNum = i;}
		}
		
		bestplayers[j].fitness = pastplayers[uchrBestNum].fitness;
		for (uint m = 0; m < DEFINED_MAX_PLAYER_STEPS; m++) {
			bestplayers[j].direction[m] = pastplayers[uchrBestNum].direction[m];
		}
		pastplayers[uchrBestNum].fitness = 0.00f; //set fitness to 0 so they are recorded again.
	}
}
/**********************************************************************************************************************************************/
void Entity::killMonster(uchar xplace,uchar yplace) {
	//Finds monster at specified place and kills them.
	//We don't have to worry about replace the tile they are in with empty
	//Because the player will be replace them.
	
	for (uchar i = 0; i < Global::Course.numMonsters; i++) {
		if (monsters[i].x == xplace && monsters[i].y == yplace) {
			monsters[i].living = false;
		}
	}
}
/**********************************************************************************************************************************************/
void Entity::allocateMonsters(uchar amount) {
	//Allocate memory for the monsters
	//Possibly unneeded
	
	monsters = (MNSTR*) malloc(sizeof(MNSTR) * amount);
	if (monsters == NULL) {Global::blnError = true; return;}
	if (Global::blnDebugMode) {printf("Monsters correctly allocated\n");}	
}
/**********************************************************************************************************************************************/
void Entity::placeBaseMonsters(uchar num, uchar x, uchar y) {
	
	MNSTR* pMonster = &monsters[num]; 
	
	pMonster->x = x;
	pMonster->y = y;
	pMonster->living = true;
	pMonster->movingright = false;
		
	
	
	//monsters[num]->x = x;
	//monsters[num]->y = y;
	//monsters[num]->living = true;
	//monsters[num]->movingright = false;
}
/**********************************************************************************************************************************************/


