/*****************************************************************************/
#include <cstdlib>
#include <cstdio>
/*****************************************************************************/
#include "entity.h"
#include "config.h"
#include "map.h"
#include "globals.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file entity.cpp
/// @brief Holds all of the functions for the Entity Class
/////////////////////////////////////////////////
/*****************************************************************************/
clsEntity::clsEntity() {
    /////////////////////////////////////////////////
    /// @brief Default constructor. Sets class values as the following:
    ///        * Gen Number = 1
    ///        * Player Num = 0
    ///        * Gen Steps = 0
    ///        Then it will set the player values to:
    ///        * Location = values based on where it would be in the default map
    ///        * Velocity = 0
    ///        * Fitness = 0
    ///        * Score = 0
    ///        * State = Rest
    ///        Then it will open the log file to clear it out.
    /////////////////////////////////////////////////

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
/*****************************************************************************/
clsEntity::~clsEntity() {
    /////////////////////////////////////////////////
    /// @brief Default deconstructor. Will delete pmstMonsters from memory.
    /////////////////////////////////////////////////

	if(Global::blnDebugMode) {printf("Entity Destructor called.\n");}
	delete[] pmstMonsters;
}
/*****************************************************************************/
void clsEntity::nextplayer(char death) {
    /////////////////////////////////////////////////
    /// @brief Sets things up for the next player in a generation by doing the following
    ///        * Add the player's inputs and cause of death if logging is enabled.
    ///        * Stores the player's fitness and inputs into the genPastPlayers array
    ///        * Resets player values back to default
    /// @param death = The death of the last player, see status
    /// @return void
    /////////////////////////////////////////////////


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

    /** \todo (xPUREx#9#): Find unicode symbols for each death. */
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
/*****************************************************************************/
void clsEntity::newplayer(void) {
    /////////////////////////////////////////////////
    /// @brief This is a pretty confusing function so I'll try to explain it best I can.
    ///        It first checks to see what step of the program we are in, First, Growth or Steady.
	///        This changes how it makes the plyPlayer.
    ///
	///        If its in the First phase. It will just generate rand directions for the player until it hits
	///        the cap specified in the config file. With directions being Up, Right or Left (down does nothing at the moment)
    ///
    ///        During the growth phase and steady phases:
	///        First it generates a random number that it uses as a reference to a player in the best player array
	///        Next it randomly generates a Number that is the amount of steps it will take from that player
	///        (I tried to make it so it wouldn't be more than half of the last generation's inputs)
	///        It will then go back and repeat getting a new player and new section number until it has filled the directions
	///        with however many steps the last generation took. If the generation is increasing (by being less than the maximum)
	///        then it will generate random directions until full.
    /// @return void
    /////////////////////////////////////////////////
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
                plyPlayer.direction[j] = (uint)(rand() % 100) < CnfgValues.uintMutationChance ? (uint)(rand() % dirDown) : genBestPlayers[uchrRandPlayer].direction[j];
			}//End for
			uTempStep = uRandSection;
		} while (uTempStep < uintGenSteps - 1);

		if (uintGenSteps + CnfgValues.uintGenIncrease < DEFINED_MAX_PLAYER_STEPS) {
			for (uint k = 0; k < uintGenSteps + CnfgValues.uintGenIncrease; k++) {plyPlayer.direction[k] = (uint)(rand() % dirDown);}
		}
	}
}
/*****************************************************************************/
void clsEntity::getFitness(void) {
    /////////////////////////////////////////////////
    /// @brief Gets the fitness for the Current Player. Fitness is based on the following
    ///        * How far left the player is
    ///        * How high the player is
    ///        * Their Score
    ///        * How many steps they have taken so far
    ///        If Hard mode is enabled the fitness will update every time the player moved
    ///        if not fitness will only update if it is higher than current fitness.
    /// @return void (Fitness is placed in the player structure)
    /////////////////////////////////////////////////
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
/*****************************************************************************/
void clsEntity::getBest(void) {
    /////////////////////////////////////////////////
    /// @brief Finds the players with the highest fitnesses in genPastPlayers and
    ///        moves them to genBestPlayers.
    /// @return void
    /////////////////////////////////////////////////

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
/*****************************************************************************/
void clsEntity::killMonster(LOC place) {
    /////////////////////////////////////////////////
    /// @brief Will kill a monster at place.
    /// @param place = Location of the top left spot where to check for monster to kill.
    /// @return void
    /////////////////////////////////////////////////

    BOX A;
    A.left = place.x;
    A.right = place.x + DEFINED_PIC_SIZE;
    A.top = place.y;
    A.bottom = place.y + DEFINED_PIC_SIZE;
    BOX B;

	for (uchar i = 0; i < Global::Map.numMonsters; i++) {
		B.left = pmstMonsters[i].location.x;
		B.right = B.left + DEFINED_PIC_SIZE;
		B.top = pmstMonsters[i].location.y;
		B.bottom = B.top + DEFINED_PIC_SIZE;
		if (Global::Map.checkOverlap(A,B)) {pmstMonsters[i].living = false;}
	}
}
/*****************************************************************************/
void clsEntity::allocateMonsters(uchar amount) {
    /////////////////////////////////////////////////
    /// @brief Grows pmstMonsters to be an array of size amount.
    /// @param amount = number of array spots that need to be made.
    /// @return void
    /////////////////////////////////////////////////
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
/*****************************************************************************/
MNSTR clsEntity::getMonster(uchar num) {
    /////////////////////////////////////////////////
    /// @brief Returns monster number num.
    /// @param num = Array spot of the monster to return.
    /// @return MNSTR
    /////////////////////////////////////////////////

	MNSTR tempMNSTR;
	tempMNSTR.location.x = pmstMonsters[num].location.x;
	tempMNSTR.location.y = pmstMonsters[num].location.y;
	tempMNSTR.vel.x = pmstMonsters[num].vel.x;
	tempMNSTR.vel.y = pmstMonsters[num].vel.y;
	tempMNSTR.living = pmstMonsters[num].living;
	tempMNSTR.state = pmstMonsters[num].state;
	return tempMNSTR;
}
/*****************************************************************************/
void clsEntity::setMonster(uchar num, MNSTR MonsterSet) {
    /////////////////////////////////////////////////
    /// @brief Sets Monster.
    /// @param num = Number array spot of monster to change.
    /// @param MonsterSet = MNSTR structure of what the monster is being set to.
    /// @return void
    /////////////////////////////////////////////////

	pmstMonsters[num].location.x = MonsterSet.location.x;
	pmstMonsters[num].location.y = MonsterSet.location.y;
    pmstMonsters[num].vel.x = MonsterSet.vel.x;
	pmstMonsters[num].vel.y = MonsterSet.vel.y;
	pmstMonsters[num].living = MonsterSet.living;
	pmstMonsters[num].state = MonsterSet.state;
}
/*****************************************************************************/
PLYR clsEntity::getPlayer(void) {
    /////////////////////////////////////////////////
    /// @brief Get player values
    /// @return plyPlayer
    /////////////////////////////////////////////////

	return plyPlayer;
}
/*****************************************************************************/
void clsEntity::setPlayer(BPLYR baseplayer) {
    /////////////////////////////////////////////////
    /// @brief Sets player (will only set its location and velocity)
    /// @param baseplayer = Location and Velocity values to set player to.
    /// @return void
    /////////////////////////////////////////////////

	plyPlayer.location.x = (uint) baseplayer.location.x;
	plyPlayer.location.y = (uint) baseplayer.location.y;
	plyPlayer.vel.x = baseplayer.vel.x;
	plyPlayer.vel.y = baseplayer.vel.y;
}
/*****************************************************************************/
char clsEntity::doPlayerStep(uint stepnum, char stage) {
    /////////////////////////////////////////////////
    /// @brief This is called to have player to input stepnum in their direction array.
    /// @param stepnum = The Array spot of the direction to pass to the map.
    /// @param stage = The stage the program is in
    /// @return Player's status
    /////////////////////////////////////////////////

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
/*****************************************************************************/
void clsEntity::doNextGeneration(char stage) {
    /////////////////////////////////////////////////
    /// @brief Gets ready to run the next generation by:
    ///        * Getting best PLayers
    ///        * Incrementing uintGenSteps based on stage
    /// @param stage = Stage the program is in.
    /// @return void
    /////////////////////////////////////////////////

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
/*****************************************************************************/
BPLYR clsEntity::getPlayerBase() {
    /////////////////////////////////////////////////
    /// @brief Gets base player values (location and velocity)
    /// @return BPLYR
    /////////////////////////////////////////////////

    BPLYR temp;
    temp.location = plyPlayer.location;
    temp.vel = plyPlayer.vel;
    return temp;
}
/*****************************************************************************/
char clsEntity::getPlayerState() {
    /////////////////////////////////////////////////
    /// @brief Gets player state
    /// @return plyPlayer.state
    /////////////////////////////////////////////////

    return plyPlayer.state;
}
/*****************************************************************************/
void clsEntity::setPlayerState(char newstate) {
    /////////////////////////////////////////////////
    /// @brief Sets player state
    /////////////////////////////////////////////////
    plyPlayer.state = newstate;
}
/*****************************************************************************/
