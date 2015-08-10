/**********************************************************************************************************************************************/
#include "entity.h"
#include "config.h"
#include "map.h"
/**********************************************************************************************************************************************/
/*
This will hold everything related to the entites that have to be kept track of including the players, monsters, past players, etc...
Later on we might spilt up players and monsters but since monsters only need to be stored at the moment I didn't see a need to make them
their own .cpp
*/
/**********************************************************************************************************************************************/
Entity::Entity() {
	//Default Constructor
	genNum = 0;
	playerNum = 0;
	uintGenSteps = 0;
}
/**********************************************************************************************************************************************/
void Entity::start(void) {
	//Starts the entire loop.
	Config Cnfg;
	Map m;
	
	if (Cnfg.values.blnLogging) {/*Open log file to clear it*/ logFile = fopen(FileName,"w"); fclose(logFile);}
	
	char chrPlayerStatus = 0;
	uintGenSteps = 0;
	genNum = 1;
	
	//First Generation
	for (playerNum = 0; playerNum < Players_Per_Generation; playerNum++) {
		makeplayer();
		for (uint step = 0; step < Cnfg.values.uintFirstGen; step++) {
			chrPlayerStatus = m.move(player.direction[step]);
			if (Cnfg.values.blnShowMap) {m.show();}
			if (chrPlayerStatus == DEAD) {
				//If the player dies clear the rest of thier directions (disabled) and end the loop.
				//for (uint j = step; j < Cnfg.values.uintFirstGen; j++) {player.direction[j] = dirNone;}
				step = Cnfg.values.uintFirstGen;
			} //end if dead
		} //End for steps
		nextplayer();
		if (Cnfg.values.blnShowMap) {m.show();}
	}//end for first gen
	
	getBest();
	if(!(Cnfg.values.blnShowMap)) {
		printf("Best Players are:\n");
		for (uint k = 0; k < 10; k++) {printf("%2.3f\n",bestplayers[k].fitness);}
	}
	
	uintGenSteps += Cnfg.values.uintFirstGen;
	
	//Growth Generation
	while (uintGenSteps + Cnfg.values.uintGenIncrease <= Max_Player_Steps) {
		genNum ++;
		for (playerNum = 0; playerNum < Players_Per_Generation; playerNum++) {
			makeplayer();
			for (uint step = 0; step < uintGenSteps + Cnfg.values.uintGenIncrease; step++) {
				chrPlayerStatus = m.move(player.direction[step]);
				if (Cnfg.values.blnShowMap) {m.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of thier directions (disabled) and end the loop.
					//for (uint j = step; j < uintGenSteps + Cnfg.values.uintGenIncrease; j++) {player.direction[j] = dirNone;}
					step = uintGenSteps + Cnfg.values.uintGenIncrease;
				} //end if dead
			}//end for steps
			nextplayer();
			if (Cnfg.values.blnShowMap) {m.show();}
		} //end for players
		getBest();
		if(!(Cnfg.values.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < 10; k++) {printf("%2.3f\n",bestplayers[k].fitness);}
		}
		uintGenSteps += Cnfg.values.uintGenIncrease;
	}//end while loop
	
	//Steady Generations
	for (uint i = 0; i < Cnfg.values.uintGensPastGrowth; i++) {
		genNum ++;
		for (playerNum = 0; playerNum < Players_Per_Generation; playerNum++) {
			makeplayer();
			for (uint step = 0; step < Max_Player_Steps; step++) {
				chrPlayerStatus = m.move(player.direction[step]);
				if (Cnfg.values.blnShowMap) {m.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of thier directions (disabled) and end the loop.
					//for (uint j = step; j < Max_Player_Steps; j++) {player.direction[j] = dirNone;}
					step = Max_Player_Steps;
				} //end if dead
			}//end for steps
			nextplayer();
			if (Cnfg.values.blnShowMap) {m.show();}
		} //end for players
		getBest();
		if(!(Cnfg.values.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < 10; k++) {printf("%2.3f\n",bestplayers[k].fitness);}
		}
	}//end for loop	
}
/**********************************************************************************************************************************************/
void Entity::nextplayer(void) {
	//Records the last player into the array of players, and logs directions to file if that is enabled.
	Config cnfg;
	Map m;
	if (cnfg.values.blnLogging) {logFile = fopen(FileName,"a");} //Open log file in append mode.
	if (Global::blnDebugMode) {printf("Player finished with fitness: %2.3f\n",player.fitness);}
	
	pastplayers[playerNum].fitness = player.fitness;
	if (cnfg.values.blnLogging) {fprintf(logFile,"Generation: %2d, Player: %2d, Fitness: %2.2f",genNum + 1,playerNum + 1,player.fitness);}
	
	player.fitness = 0.00f;
	player.score = 0;
	
	for (uint i = 0; i < Max_Player_Steps; i++) {
		pastplayers[playerNum].direction[i] = player.direction[i];
		if (cnfg.values.blnLogging) {
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
	
	if (cnfg.values.blnLogging) {
		fprintf(logFile, "\n");
		if (playerNum + 1 == Players_Per_Generation) {
			//If this is the last player add a line to better seperate the different generations
			//in the log file. The line will be as long as the longest possible string of directions
			
			for (uint j = 0; j < 2* Max_Player_Steps + 42; j++) {fprintf(logFile, "=");}
			fprintf(logFile, "\n");
		} //End of if last gen player
		fclose(logFile);
	} //end of if logging
	m.restart();
}
/**********************************************************************************************************************************************/
void Entity::makeplayer(void) {
	/*
	This is a pretty confusing function so I'll try to explain it best I can.
	
	It first checks to see what step of the program we are in, First, Growth or Steady. 
	This changes how it makes the player.

	If its in the First phase. It will just generate rand directions for the player until it hits 
	the cap specificed in the config file. With directions being Up, Right or Left (down does nothing at the moment)
	
	During the growth phase and steady phases:
	First it generates a random number that it uses as a reference to a player in the best player array
	Next it randomly generates a Number that is the amount of steps it will take from that player 
	(I tried to make it so it wouldn't be more than half of the last generation's inputs)
	It will then go back and reapeat getting a new player and new section number until it has filled the direcitons
	with however many steps the last generation took. If the generation is increasing (by being less than the maxium)
	then it will generate random directions until full.
	*/
	Config Cnfg;
	uchar uchrRandPlayer;
	uint uRandSection, uTempStep = 0;
	
	if (genNum == 1) { //First Generation
		for (uint i = 0; i < Cnfg.values.uintFirstGen; i++) {player.direction[i] = (uchar)(rand() % (dirRight + 1) + dirUp);}
	} else { //Growth Phase & Steady phase
		do {
			uchrRandPlayer = rand() % 10;
			uRandSection = (uint)(rand() % ((uintGenSteps - uTempStep) / 2) + uTempStep);
			if (Global::blnDebugMode) {printf("Player %d Section of %d\n",uchrRandPlayer,uRandSection);}
			for (uint j = uTempStep; j < uRandSection; j++) {
				if ((uint)(rand() % 100) < Cnfg.values.uintMutationChance) {player.direction[j] = (uchar)(rand() % (dirRight + 1) + dirUp);}
				else {player.direction[j] = bestplayers[uchrRandPlayer].direction[j];}
			}//End for
			uTempStep = uRandSection;
		} while (uTempStep < uintGenSteps);
		
		if (uintGenSteps + Cnfg.values.uintGenIncrease < Max_Player_Steps) {
			for (uint k = 0; k < uintGenSteps + Cnfg.values.uintGenIncrease; k++) {player.direction[k] = (uchar)(rand() % (dirRight + 1) + dirUp);}
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
	Config cnfg;
	Map m;
	float temp = 0.00f;
	temp += (player.score) / 800.0;
	temp += (5.0/2.0) * (player.x - m.baseplayer.x);
	temp += (player.x + player.y) / 6.0;
	temp += (m.baseplayer.y - player.y) / 4.0;
	if (player.x > 204) {temp += 200.0;}
	if (cnfg.values.blnHardMode) {temp -= uintStepNum / 80.0;}
	return temp;
}
/**********************************************************************************************************************************************/
void Entity::getBest(void) {
	//Gets best players in a generation.
	float fTempfit = 0;
	uchar uchrBestNum = 0;
	for (uchar j = 0; j < 10; j ++) {
		fTempfit = 0.00f;
		uchrBestNum = 0;
		for (uchar i = 0; i < Players_Per_Generation; i++) {
			if (pastplayers[i].fitness > fTempfit) {fTempfit = pastplayers[i].fitness; uchrBestNum = i;}
		}
		
		bestplayers[j].fitness = pastplayers[uchrBestNum].fitness;
		for (uint m = 0; m < Max_Player_Steps; m++) {
			bestplayers[j].direction[m] = pastplayers[uchrBestNum].direction[m];
		}
		pastplayers[uchrBestNum].fitness = 0.00f; //set fitness to 0 so they are recorded again.
	}
}
/**********************************************************************************************************************************************/
void Entity::killMonster(uchar xplace,uchar yplace) {
	//Finds monster at specificed place and kills them.
	//We don't have to worry about replace the tile they are in with empty
	//Because the player will be replace them.
	Map m;
	
	for (uchar i = 0; i < m.numMonsters; i++) {
		if (monsters[i].x == xplace && monsters[i].y == yplace) {
			monsters[i].living = false;
		}
	}
}
/**********************************************************************************************************************************************/
void Entity::allocateMonsters(uchar amount) {
	//Allocate memory for the monsters
	
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


