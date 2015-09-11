/**********************************************************************************************************************************************/
#include "entity.h"
#include "config.h"
#include "map.h"
#include "globals.h"
/**********************************************************************************************************************************************/
/*
This will hold everything related to the entities that have to be kept track of including the players, monsters, past players, etc...
Later on we might split up players and monsters but since monsters only need to be stored at the moment I didn't see a need to make them
their own .cpp
*/
/**********************************************************************************************************************************************/
clsEntity::clsEntity() {
	if (Global::blnDebugMode) {printf("Entity Constructor called.\n");}
	//Default Constructor
	uchrGenNum = 1;
	uchrPlayerNum = 0;
	uintGenSteps = 0;

	plyPlayer.location.x = 4 * DEFINED_PIC_SIZE;
	plyPlayer.location.y = 10 * DEFINED_PIC_SIZE;
	plyPlayer.vel.x = 0.00;
	plyPlayer.vel.y = 0.00;
	plyPlayer.fitness = 0.00f;
	plyPlayer.score = 0u;
	plyPlayer.state = stateRest;

	if (Global::Cnfg.getvalues(cnfgLogging) == 1) {/*Open log file to clear it*/ logFile = fopen(DEFINED_LOG_FILE_NAME,"w"); fclose(logFile);}
}
/**********************************************************************************************************************************************/
clsEntity::~clsEntity() {
	if(Global::blnDebugMode) {printf("Entity Destructor called.\n");}
	delete[] pmstMonsters;
}
/**********************************************************************************************************************************************/
void clsEntity::nextplayer(char death) {

	Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();

	//Records the last player into the array of players, and logs directions to file if that is enabled.
	if (CnfgValues.blnLogging) {logFile = fopen(FileName,"a");} //Open log file in append mode.
	if (Global::blnDebugMode) {printf("Player finished with fitness: %2.3f\n",plyPlayer.fitness); }

	genPastPlayers[uchrPlayerNum].fitness = plyPlayer.fitness;
	if (CnfgValues.blnLogging) {fprintf(logFile,"Generation: %2d, Player: %2d, Fitness: %2.2f",uchrGenNum,uchrPlayerNum,plyPlayer.fitness);}

	plyPlayer.fitness = 0.00f;
	plyPlayer.score = 0;
	plyPlayer.state = stateRest;

	for (uint i = 0; i < DEFINED_MAX_PLAYER_STEPS; i++) {
		genPastPlayers[uchrPlayerNum].direction[i] = plyPlayer.direction[i];
		if (CnfgValues.blnLogging) {
			switch (plyPlayer.direction[i]) {
				case dirNone :
					fprintf(logFile," _");
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

    /* TODO (xPUREx#9#): Find unicode symbols for each death. */
	if (CnfgValues.blnLogging) {
        //Write cause of death to log
        switch (death) {
        case deathMonster:
            fprintf(logFile," M");
            break;
        case deathClock:
            fprintf(logFile," C");
            break;
        case deathFall:
            fprintf(logFile, " F");
            break;
        case deathDecay:
            fprintf(logFile, " D");
            break;
        case deathStupid:
            fprintf(logFile, " S");
            break;
        case deathInputs:
            fprintf(logFile, " I");
            break;
        case deathError:
            fprintf(logFile, " E");
            break;
        default:
            fprintf(logFile, " G");
            break;
        }

		fprintf(logFile, "\n");
		if (uchrPlayerNum >= DEFINED_PLAYERS_PER_GEN) {
			//If this is the last player add a line to better separate the different generations
			//in the log file. The line will be as long as the longest possible string of directions
			for (uint j = 0; j < 2 * DEFINED_MAX_PLAYER_STEPS + 42; j++) {fprintf(logFile, "=");}
			fprintf(logFile, "\n");
		} //End of if last gen player
		fclose(logFile);
	} //end of if logging
}
/**********************************************************************************************************************************************/
void clsEntity::newplayer(void) {
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

    uchrPlayerNum++;

	Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();

	uchar uchrRandPlayer;
	uint uRandSection, uTempStep = 0;

	if (uchrGenNum == 1) { //First Generation
		for (uint i = 0; i < CnfgValues.uintFirstGen; i++) {plyPlayer.direction[i] = (uint)(rand() % dirDown);}
		for (uint i = CnfgValues.uintFirstGen; i < DEFINED_MAX_PLAYER_STEPS; i++ ) {plyPlayer.direction[i] = dirNone;}
	} else { //Growth Phase & Steady phase
		do {
			uchrRandPlayer = rand() % DEFINED_BEST_PLAYER_NUM;
			uRandSection = (uint)(rand() % ((uintGenSteps - uTempStep)) + uTempStep);
			if (Global::blnDebugMode) {printf("Player %d Section of %d\n",uchrRandPlayer,uRandSection);}
			for (uint j = uTempStep; j <= uRandSection; j++) {
				if ((uint)(rand() % 100) < CnfgValues.uintMutationChance) {plyPlayer.direction[j] = (uint)(rand() % dirDown);}
				else {plyPlayer.direction[j] = genBestPlayers[uchrRandPlayer].direction[j];}
			}//End for
			uTempStep = uRandSection;
		} while (uTempStep < uintGenSteps - 1);

		if (uintGenSteps + CnfgValues.uintGenIncrease < DEFINED_MAX_PLAYER_STEPS) {
			for (uint k = 0; k < uintGenSteps + CnfgValues.uintGenIncrease; k++) {plyPlayer.direction[k] = (uint)(rand() % dirDown);}
		}
	}
}
/**********************************************************************************************************************************************/
void clsEntity::getFitness(void) {
	/*
	Calculates the fitness of the plyPlayer.
	If it is hard mode then the longer the player takes
	the more fitness will decrease
	Note that 204 is considered the "finish" line.
	*/

	float temp = 0.00f;

	//Get the spot that the player starts at for reference
	BPLYR bplyPlayerBase;
	bplyPlayerBase = Global::Map.getbasePlayer();

	temp += (plyPlayer.score) / 250.0;
	if (plyPlayer.location.x >= bplyPlayerBase.location.x) {temp += (0.25) * (plyPlayer.location.x - bplyPlayerBase.location.x);}
	if (bplyPlayerBase.location.y >= plyPlayer.location.y) {temp += (0.15) * (bplyPlayerBase.location.y - plyPlayer.location.y) ;}
	temp += (0.05) * (plyPlayer.location.x + plyPlayer.location.y);
	if (plyPlayer.location.x > (203 * DEFINED_PIC_SIZE) ) {temp += 200.0;}
	if (Global::Cnfg.getvalues(cnfgHardMode) == 1) {temp -= uintStepNum / 80.0;}

	//In hard mode player fitness is updated every frame, while when not hard mode
    //it will only update if the new fitness value is higher than the old one.
	if (Global::Cnfg.getvalues(cnfgHardMode) == 1 || temp > plyPlayer.fitness) {plyPlayer.fitness = temp;}
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
		genPastPlayers[uchrBestNum].fitness = 0.00f; //set fitness to 0 so they aren't recorded again.
	}
}
/**********************************************************************************************************************************************/
void clsEntity::killMonster(uchar xplace,uchar yplace) {
	//Finds monster at specified place and kills them.
	//We don't have to worry about replacing the tile they are in with empty
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
	tempMNSTR.vel.x = pmstMonsters[num].vel.x;
	tempMNSTR.vel.y = pmstMonsters[num].vel.y;
	tempMNSTR.living = pmstMonsters[num].living;
	tempMNSTR.state = pmstMonsters[num].state;
	return tempMNSTR;
}
/**********************************************************************************************************************************************/
void clsEntity::setMonster(uchar num, MNSTR MonsterSet) {
	pmstMonsters[num].location.x = MonsterSet.location.x;
	pmstMonsters[num].location.y = MonsterSet.location.y;
    pmstMonsters[num].vel.x = MonsterSet.vel.x;
	pmstMonsters[num].vel.y = MonsterSet.vel.y;
	pmstMonsters[num].living = MonsterSet.living;
	pmstMonsters[num].state = MonsterSet.state;
}
/**********************************************************************************************************************************************/
PLYR clsEntity::getPlayer(void) {
	return plyPlayer;
}
/**********************************************************************************************************************************************/
void clsEntity::setPlayer(BPLYR baseplayer) {
	plyPlayer.location.x = (uint) baseplayer.location.x;
	plyPlayer.location.y = (uint) baseplayer.location.y;
	plyPlayer.vel.x = baseplayer.vel.x;
	plyPlayer.vel.y = baseplayer.vel.y;
}
/**********************************************************************************************************************************************/
char clsEntity::doPlayerStep(uint stepnum, char stage) {
    char chrPlayerStatus;
    switch (plyPlayer.direction[stepnum]) {
    case dirNone:
        plyPlayer.state = stateRest;
        break;
    case dirDown:
        plyPlayer.state = stateDuck;
        break;
    case dirLeft:
        plyPlayer.state = stateLeft;
        break;
    case dirUp:
        plyPlayer.state = stateJump;
        break;
    case dirRight:
        plyPlayer.state = stateRight;
        break;
    default:
        plyPlayer.state = stateRest;
        break;
    }

    chrPlayerStatus = Global::Map.move(plyPlayer.direction[stepnum]);
    getFitness();
    //Do some checks to see if player is at the end of their inputs and change chrPlayerStatus to dead if they are.
    if (stage == stageFirst && stepnum >= Global::Cnfg.getvalues(cnfgFirstGen) - 1 ) {chrPlayerStatus = deathInputs;}
    else if (stage == stageGrowth && stepnum >= uintGenSteps + Global::Cnfg.getvalues(cnfgGenIncrease) - 1) {chrPlayerStatus = deathInputs;}
    else if (stage == stageSteady && stepnum >= DEFINED_MAX_PLAYER_STEPS - 1) {chrPlayerStatus = deathInputs;}

    if (chrPlayerStatus != statusLiving) {
        //If the player dies clear the rest of their directions (disabled) and end the loop.
        //for (uint j = step; j < DEFINED_MAX_PLAYER_STEPS; j++) {plyPlayer.direction[j] = dirNone;}
        nextplayer(chrPlayerStatus);
    }
    return chrPlayerStatus;
}
/**********************************************************************************************************************************************/
void clsEntity::doNextGeneration(char stage) {
    uchrPlayerNum = 0;
    getBest();
    if(Global::Cnfg.getvalues(cnfgShowMap) != 1) {
        printf("Best Players are:\n");
        for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",genBestPlayers[k].fitness);}
        if (Global::blnDebugMode) {getchar();}
    }

    uchrGenNum++;
    //Increment Gen Steps depending on what stage we are in.
    if (stage == stageFirst) {uintGenSteps += Global::Cnfg.getvalues(cnfgFirstGen);}
    else if (stage == stageGrowth) {uintGenSteps += Global::Cnfg.getvalues(cnfgGenIncrease);}
    else {uintGenSteps = DEFINED_MAX_PLAYER_STEPS;}
}
/**********************************************************************************************************************************************/
BPLYR clsEntity::getPlayerBase() {
    BPLYR temp;
    temp.location = plyPlayer.location;
    temp.vel = plyPlayer.vel;
    return temp;
}
/**********************************************************************************************************************************************/
