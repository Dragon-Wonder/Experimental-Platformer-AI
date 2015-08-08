/**********************************************************************************************************************************************/
#include "entity.h"
#include "config.h"
/**********************************************************************************************************************************************/
/*
This will hold everything related to the entites that have to be kept track of including the players, monsters, past players, etc...
Later on we might spilt up players and monsters but since monsters only need to be stored at the moment I didn't see a need to make them
their own .cpp
*/
/**********************************************************************************************************************************************/
void Entity::start(void) {
	//Starts the entire loop.
	Config Cnfg;
	Map m;
	
	if (Cnfg.values.blnLogging) {/*Open log file to clear it*/ Entity::logFile = fopen(Entity::FileName,"w"); flclose(Entity::logFile);}
	
	char chrPlayerStatus = 0;
	Entity::uintGenSteps = 0;
	Entity::genNum = 1;
	
	//First Generation
	for (Entity::playerNum = 0; Entity::playerNum < Players_Per_Generation; Entity::playerNum++) {
		Entity::makeplayer();
		for (uint step = 0; step < Cnfg.values.uintFirstGen; step++) {
			chrPlayerStatus = m.move(Entity::player.direction[step]);
			if (Cnfg.values.blnShowMap) {m.show();}
			if (chrPlayerStatus == DEAD) {
				//If the player dies clear the rest of thier directions (disabled) and end the loop.
				//for (uint j = step; j < Cnfg.values.uintFirstGen; j++) {Entity::player.direction[j] = dirNone;}
				step = Cnfg.values.uintFirstGen;
			} //end if dead
		} //End for steps
		Entity::nextplayer();
		if (Cnfg.values.blnShowMap) {m.show();}
	}//end for first gen
	
	Entity::getBest();
	if(!(Cnfg.values.blnShowMap)) {
		printf("Best Players are:\n");
		for (uint k = 0; k < 10; k++) {printf("%2.3f\n",Entity::bestplayers[k].fitness);}
	}
	
	uintGenSteps += Cnfg.values.uintFirstGen;
	
	//Growth Generation
	while (uintGenSteps + Cnfg.values.uintGenIncrease <= Max_Player_Steps) {
		Entity::genNum ++;
		for (Entity::playerNum = 0; Entity::playerNum < Players_Per_Generation; Entity::playerNum++) {
			Entity::makeplayer();
			for (uint step = 0; step < uintGenSteps + Cnfg.values.uintGenIncrease; step++) {
				chrPlayerStatus = m.move(Entity::player.direction[step]);
				if (Cnfg.values.blnShowMap) {m.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of thier directions (disabled) and end the loop.
					//for (uint j = step; j < uintGenSteps + Cnfg.values.uintGenIncrease; j++) {Entity::player.direction[j] = dirNone;}
					step = uintGenSteps + Cnfg.values.uintGenIncrease;
				} //end if dead
			}//end for steps
			Entity::nextplayer();
			if (Cnfg.values.blnShowMap) {m.show();}
		} //end for players
		Entity::getBest();
		if(!(Cnfg.values.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < 10; k++) {printf("%2.3f\n",Entity::bestplayers[k].fitness);}
		}
		uintGenSteps += Cnfg.values.uintGenIncrease;
	}//end while loop
	
	//Steady Generations
	for (uint i = 0; i < Cnfg.values.uintGensPastGrowth; i++) {
		Entity::genNum ++;
		for (Entity::playerNum = 0; Entity::playerNum < Players_Per_Generation; Entity::playerNum++) {
			Entity::makeplayer();
			for (uint step = 0; step < Max_Player_Steps; step++) {
				chrPlayerStatus = m.move(Entity::player.direction[step]);
				if (Cnfg.values.blnShowMap) {m.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of thier directions (disabled) and end the loop.
					//for (uint j = step; j < Max_Player_Steps; j++) {Entity::player.direction[j] = dirNone;}
					step = Max_Player_Steps;
				} //end if dead
			}//end for steps
			Entity::nextplayer();
			if (Cnfg.values.blnShowMap) {m.show();}
		} //end for players
		Entity::getBest();
		if(!(Cnfg.values.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < 10; k++) {printf("%2.3f\n",Entity::bestplayers[k].fitness);}
		}
	}//end for loop	
}
/**********************************************************************************************************************************************/
void Entity::nextplayer(void) {
	//Records the last player into the array of players, and logs directions to file if that is enabled.
	Config cnfg;
	if (cnfg.values.blnLogging) {Entity::logFile = fopen(Entity::FileName,"a");} //Open log file in append mode.
	if (Gloabl::blnDebugMode) {printf("Player finished with fitness: %2.3f\n",Entity::player.fitness);}
	
	Entity::pastplayers[Entity::playerNum].fitness = Entity::player.fitness;
	if (cnfg.values.blnLogging) {fprintf(Entity::logFile,"Generation: %2d, Player: %2d, Fitness: %2.2f",Entity::genNum + 1,Entity::playerNum + 1,Entity::player.fitness);}
	
	Entity::player.fitness = 0.00f;
	Entity::player.score = 0;
	
	for (uint i = 0; i < Max_Player_Steps; i++) {
		Entity::pastplayers[Entity::playerNum].direction[i] = Entity::player.direction[i];
		if (cnfg.values.blnLogging) {
			switch (Entity::player.direction[i]) {
				case dirNone :
					//Do nothing
					break;
				case dirUp :
					fprintf(Entity::logFile," ↑");
					break;
				case dirLeft :
					fprintf(Entity::logFile, " ←");
					break;
				case dirRight :
					fprintf(Entity::logFile, " →");
					break;
				case dirDown :
					fprintf(Entity::logFile, " ↓");
					break;
				default :
					fprintf(Entity::logFile, " ?");
					break;
			} //End Switch
		}// End logging if
	}//end for
	
	if (cnfg.values.blnLogging) {
		fprintf(Entity::logFile, "\n");
		if (Entity::playerNum + 1 == Players_Per_Generation) {
			//If this is the last player add a line to better seperate the different generations
			//in the log file. The line will be as long as the longest possible string of directions
			
			for (uint j = 0; j < 2* Max_Player_Steps + 42; j++) {fprintf(Entity::logFile, "=");}
			fprintf(Entity::logFile, "\n");
		} //End of if last gen player
		fclose(Entity::logFile);
	} //end of if logging
	Map::restart();
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
		for (uint i = 0; i < Cnfg.values.uintFirstGen) {Entity::player.direction[i] = (uchar)(rand() % (dirRight + 1) + dirUp);}
	} else { //Growth Phase & Steady phase
		do {
			uchrRandPlayer = rand() % 10;
			uRandSection = (uint)(rand() % ((Entity::uintGenSteps - uTempStep) / 2) + uTempStep);
			if (Gloabl::blnDebugMode) {printf("Player %d Section of %d\n",uchrRandPlayer,uRandSection);}
			for (uint j = uTempStep; j < uRandSection; j++) {
				if ((uint)(rand() % 100) < Cnfg.values.uintMutationChance) {Entity::player.direction[j] = (uchar)(rand() % (dirRight + 1) + dirUp);}
				else {Entity::player.direction[j] = Entity::bestplayers[uchrRandPlayer].direction[j];}
			}//End for
			uTempStep = uRandSection;
		} while (uTempStep < uintGenSteps);
		
		if (uintGenSteps + Cnfg.values.uintGenIncrease < Max_Player_Steps) {
			for (uint k = 0; k < uintGenSteps + Cnfg.values.uintGenIncrease; k++) {Entity::player.direction[i] = (uchar)(rand() % (dirRight + 1) + dirUp);}
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
	temp += (Entity::player.score) / 800.0;
	temp += (5.0/2.0) * (Entity::player.x - m.baseplayer.x);
	temp += (Entity::player.x + Entity::player.y) / 6.0;
	temp += (m.baseplayer.y - Entity::player.y) / 4.0;
	if (Entity::player.x > 204) {temp += 200.0;}
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
			if (Entity::pastplayers[i].fitness > fTempfit;) {fTempfit = Entity::pastplayers[i].fitness; uchrBestNum = i;}
		}
		
		Entity::bestplayers[j].fitness = Entity::pastplayers[uchrBestNum].fitness;
		for (uint m = 0; m < Max_Player_Steps; m++) {
			Entity::bestplayers[j].direction[m] = Entity::pastplayers[uchrBestNum].direction[m];
		}
		Entity::pastplayers[uchrBestNum].fitness = 0.00f; //set fitness to 0 so they are recorded again.
	}
}
/**********************************************************************************************************************************************/
void Entity::killMonster(uchar xplace,uchar yplace) {
	//Finds monster at specificed place and kills them.
	//We don't have to worry about replace the tile they are in with empty
	//Because the player will be replace them.
	Map m;
	
	for (uchar i = 0; i < m.numMonsters; i++) {
		if (Entity::monsters[i].x == xplace && Entity::monsters[i].y == yplace) {
			Entity::monsters[i].living = false;
		}
	}
}
/**********************************************************************************************************************************************/
