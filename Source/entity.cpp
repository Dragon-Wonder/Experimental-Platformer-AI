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
clsEntity::clsEntity() {
	if (Global::blnDebugMode) {printf("Entity Constructor called.\n");}
	//Default Constructor
	uchrGenNum = 0;
	uchrPlayerNum = 0;
	uintGenSteps = 0;

	plyPlayer.location.x = 5u;
	plyPlayer.location.y = 11u;
	plyPlayer.fitness = 0.00f;
	plyPlayer.score = 0u;

}
/**********************************************************************************************************************************************/
clsEntity::~clsEntity() {
	if(Global::blnDebugMode) {printf("Entity Destructor called.\n");}
	delete[] pmstMonsters;
}
/**********************************************************************************************************************************************/
void clsEntity::start(void) {
	//Starts the entire loop.

	Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();
	if (CnfgValues.blnLogging) {/*Open log file to clear it*/ logFile = fopen(FileName,"w"); fclose(logFile);}

	char chrPlayerStatus = 0;
	uintGenSteps = 0;
	uchrGenNum = 1;

	//First Generation
	for (uchrPlayerNum = 0; uchrPlayerNum < DEFINED_PLAYERS_PER_GEN; uchrPlayerNum++) {
		makeplayer();
		for (uint step = 0; step < CnfgValues.uintFirstGen; step++) {
			chrPlayerStatus = Global::Map.move(plyPlayer.direction[step]);
			plyPlayer.fitness = getFitness();
			if (CnfgValues.blnShowMap) {Global::Map.show();}
			if (chrPlayerStatus == DEAD) {
				//If the player dies clear the rest of their directions (disabled) and end the loop.
				//for (uint j = step; j < Global::Cnfg.values.uintFirstGen; j++) {plyPlayer.direction[j] = dirNone;}
				step = CnfgValues.uintFirstGen;
				if (CnfgValues.blnShowMap) {Global::Map.playerDeath();}
			} //end if dead
		} //End for steps
		nextplayer();
		if (CnfgValues.blnShowMap) {Global::Map.show();}
	}//end for first gen

	getBest();
	if(!(CnfgValues.blnShowMap)) {
		printf("Best Players are:\n");
		for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",genBestPlayers[k].fitness);}
		if (Global::blnDebugMode) {getchar();}
	}

	uintGenSteps += CnfgValues.uintFirstGen;

	//Growth Generation
	while (uintGenSteps +CnfgValues.uintGenIncrease <= DEFINED_MAX_PLAYER_STEPS) {
		uchrGenNum ++;
		for (uchrPlayerNum = 0; uchrPlayerNum < DEFINED_PLAYERS_PER_GEN; uchrPlayerNum++) {
			makeplayer();
			for (uint step = 0; step < uintGenSteps + CnfgValues.uintGenIncrease; step++) {
				chrPlayerStatus = Global::Map.move(plyPlayer.direction[step]);
				plyPlayer.fitness = getFitness();
				if (CnfgValues.blnShowMap) {Global::Map.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of their directions (disabled) and end the loop.
					//for (uint j = step; j < uintGenSteps + Global::Cnfg.values.uintGenIncrease; j++) {plyPlayer.direction[j] = dirNone;}
					step = uintGenSteps + CnfgValues.uintGenIncrease;
					if (CnfgValues.blnShowMap) {Global::Map.playerDeath();}
				} //end if dead
			}//end for steps
			nextplayer();
			if (CnfgValues.blnShowMap) {Global::Map.show();}
		} //end for players
		getBest();
		if(!(CnfgValues.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",genBestPlayers[k].fitness);}
			if (Global::blnDebugMode) {getchar();}
		}
		uintGenSteps += CnfgValues.uintGenIncrease;
	}//end while loop

	//Steady Generations
	for (uint i = 0; i < CnfgValues.uintGensPastGrowth; i++) {
		uchrGenNum ++;
		for (uchrPlayerNum = 0; uchrPlayerNum < DEFINED_PLAYERS_PER_GEN; uchrPlayerNum++) {
			makeplayer();
			for (uint step = 0; step < DEFINED_MAX_PLAYER_STEPS; step++) {
				chrPlayerStatus = Global::Map.move(plyPlayer.direction[step]);
				plyPlayer.fitness = getFitness();
				if (CnfgValues.blnShowMap) {Global::Map.show();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of their directions (disabled) and end the loop.
					//for (uint j = step; j < DEFINED_MAX_PLAYER_STEPS; j++) {plyPlayer.direction[j] = dirNone;}
					step = DEFINED_MAX_PLAYER_STEPS;
					if (CnfgValues.blnShowMap) {Global::Map.playerDeath();}
				} //end if dead
			}//end for steps
			nextplayer();
			if (CnfgValues.blnShowMap) {Global::Map.show();}
		} //end for players
		getBest();
		if(!(CnfgValues.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",genBestPlayers[k].fitness);}
			if (Global::blnDebugMode) {getchar();}
		}
	}//end for loop
}
/**********************************************************************************************************************************************/
void clsEntity::nextplayer(void) {

	Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();

	//Records the last player into the array of players, and logs directions to file if that is enabled.
	if (CnfgValues.blnLogging) {logFile = fopen(FileName,"a");} //Open log file in append mode.
	if (Global::blnDebugMode) {printf("Player finished with fitness: %2.3f\n",plyPlayer.fitness); getchar(); }

	genPastPlayers[uchrPlayerNum].fitness = plyPlayer.fitness;
	if (CnfgValues.blnLogging) {fprintf(logFile,"Generation: %2d, Player: %2d, Fitness: %2.2f",uchrGenNum,uchrPlayerNum + 1,plyPlayer.fitness);}

	plyPlayer.fitness = 0.00f;
	plyPlayer.score = 0;

	for (uint i = 0; i < DEFINED_MAX_PLAYER_STEPS; i++) {
		genPastPlayers[uchrPlayerNum].direction[i] = plyPlayer.direction[i];
		if (CnfgValues.blnLogging) {
			switch (plyPlayer.direction[i]) {
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

	if (CnfgValues.blnLogging) {
		fprintf(logFile, "\n");
		if (uchrPlayerNum + 1 == DEFINED_PLAYERS_PER_GEN) {
			//If this is the last player add a line to better separate the different generations
			//in the log file. The line will be as long as the longest possible string of directions

			for (uint j = 0; j < 2* DEFINED_MAX_PLAYER_STEPS + 42; j++) {fprintf(logFile, "=");}
			fprintf(logFile, "\n");
		} //End of if last gen player
		fclose(logFile);
	} //end of if logging
	Global::Map.restart();
}
/**********************************************************************************************************************************************/
void clsEntity::makeplayer(void) {
	/*
	This is a pretty confusing function so I'll try to explain it best I can.

	It first checks to see what step of the program we are in, First, Growth or Steady.
	This changes how it makes the plyPlayer.

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

	Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();

	uchar uchrRandPlayer;
	uint uRandSection, uTempStep = 0;

	if (uchrGenNum == 1) { //First Generation
		for (uint i = 0; i < CnfgValues.uintFirstGen; i++) {plyPlayer.direction[i] = (uint)(rand() % (dirRight) + dirUp);}
		for (uint i = CnfgValues.uintFirstGen; i < DEFINED_MAX_PLAYER_STEPS; i++ ) {plyPlayer.direction[i] = dirNone;}
	} else { //Growth Phase & Steady phase
		do {
			uchrRandPlayer = rand() % DEFINED_BEST_PLAYER_NUM;
			uRandSection = (uint)(rand() % ((uintGenSteps - uTempStep)) + uTempStep);
			if (Global::blnDebugMode) {printf("Player %d Section of %d\n",uchrRandPlayer,uRandSection);}
			for (uint j = uTempStep; j <= uRandSection; j++) {
				if ((uint)(rand() % 100) < CnfgValues.uintMutationChance) {plyPlayer.direction[j] = (uint)(rand() % (dirRight) + dirUp);}
				else {plyPlayer.direction[j] = genBestPlayers[uchrRandPlayer].direction[j];}
			}//End for
			uTempStep = uRandSection;
		} while (uTempStep < uintGenSteps - 1);

		if (uintGenSteps + CnfgValues.uintGenIncrease < DEFINED_MAX_PLAYER_STEPS) {
			for (uint k = 0; k < uintGenSteps + CnfgValues.uintGenIncrease; k++) {plyPlayer.direction[k] = (uint)(rand() % (dirRight) + dirUp);}
		}
	}
}
/**********************************************************************************************************************************************/
float clsEntity::getFitness(void) {
	/* Calculates the fitness of the plyPlayer.
	If it is hard mode then the longer the player takes
	the more fitness will decrease
	Note that 204 is considered the "finish" line.
	*/

	float temp = 0.00f;

	LOC locPlayerBase;
	locPlayerBase = Global::Map.getbasePlayer();

	temp += (plyPlayer.score) / 250.0;
	temp += (5.0/2.0) * (plyPlayer.location.x - locPlayerBase.x);
	temp += (plyPlayer.location.x + plyPlayer.location.y) / 6.0;
	temp += (locPlayerBase.y - plyPlayer.location.y) / 4.0;
	if (plyPlayer.location.x > 204) {temp += 200.0;}
	if (Global::Cnfg.getvalues(cnfgHardMode) == 1) {temp -= uintStepNum / 80.0;}
	return temp;
}
/**********************************************************************************************************************************************/
void clsEntity::getBest(void) {
	//Gets best players in a generation.
	float fTempfit = 0;
	uchar uchrBestNum = 0;
	for (uchar j = 0; j < DEFINED_BEST_PLAYER_NUM; j ++) {
		fTempfit = 0.00f;
		uchrBestNum = 0;
		for (uchar i = 0; i < DEFINED_PLAYERS_PER_GEN; i++) {
			if (genPastPlayers[i].fitness > fTempfit) {fTempfit = genPastPlayers[i].fitness; uchrBestNum = i;}
		}

		genBestPlayers[j].fitness = genPastPlayers[uchrBestNum].fitness;
		for (uint m = 0; m < DEFINED_MAX_PLAYER_STEPS; m++) {
			genBestPlayers[j].direction[m] = genPastPlayers[uchrBestNum].direction[m];
		}
		genPastPlayers[uchrBestNum].fitness = 0.00f; //set fitness to 0 so they are recorded again.
	}
}
/**********************************************************************************************************************************************/
void clsEntity::killMonster(uchar xplace,uchar yplace) {
	//Finds monster at specified place and kills them.
	//We don't have to worry about replace the tile they are in with empty
	//Because the player will be replace them.

	for (uchar i = 0; i < Global::Map.numMonsters; i++) {
		if (pmstMonsters[i].location.x == xplace && pmstMonsters[i].location.y == yplace) {
			pmstMonsters[i].living = false;
		}
	}
}
/**********************************************************************************************************************************************/
void clsEntity::allocateMonsters(uchar amount) {
	//Allocate memory for the monsters
	pmstMonsters = new (std::nothrow) MNSTR [amount];

	//Quick Note: the no throw is used here so that the program doesn't just end when
	//it cannot allocate the memory instead it checks if the pointer = nullptr
	//which happens when allocation fails, and then runs the three lines below.

	if (pmstMonsters == nullptr) {
		//Could not allocate the memory
		if (Global::blnDebugMode) {printf("Monsters were not allocated\n");}
		Global::blnError = true;
		return;
	} else {
		if (Global::blnDebugMode) {printf("Monsters correctly allocated\n");}
	}
}
/**********************************************************************************************************************************************/
MNSTR clsEntity::getMonster(uchar num) {
	MNSTR tempMNSTR;

	tempMNSTR.location.x = pmstMonsters[num].location.x;
	tempMNSTR.location.y = pmstMonsters[num].location.y;
	tempMNSTR.living = pmstMonsters[num].living;
	tempMNSTR.movingright = pmstMonsters[num].movingright;
	return tempMNSTR;
}
/**********************************************************************************************************************************************/
void clsEntity::setMonster(uchar num, MNSTR MonsterSet) {

	pmstMonsters[num].location.x = MonsterSet.location.x;
	pmstMonsters[num].location.y = MonsterSet.location.y;
	pmstMonsters[num].living = MonsterSet.living;
	pmstMonsters[num].movingright = MonsterSet.movingright;
}
/**********************************************************************************************************************************************/
PLYR clsEntity::getPlayer(void) {
	if(Global::blnDebugMode) {printf("Get Player's values, x = %d, y = %d, fitness = %3.2f, score = %d.\n",plyPlayer.location.x,plyPlayer.location.y,plyPlayer.fitness,plyPlayer.score);}
	return plyPlayer;
}
/**********************************************************************************************************************************************/
void clsEntity::setPlayer(LOC Place) {
	plyPlayer.location.x = (uint) Place.x;
	plyPlayer.location.y = (uint) Place.y;
	//plyPlayer.score = (uint) PlayerSet.score;
	//plyPlayer.fitness = (float) PlayerSet.fitness;
}
/**********************************************************************************************************************************************/
