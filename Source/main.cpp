/**********************************************************************************************************************************************/
//The main function, most of the actual program is found in Entity.cpp under start.
//So see there for most of the functions.
/**********************************************************************************************************************************************/
#include "version.h"
#include "main.h"
#include "config.h"
#include "tick.h"
#include "map.h"
#include "entity.h"
#include "globals.h"
#include "screen.h"
/**********************************************************************************************************************************************/
//General Todos
// TODO (GamerMan7799#9#): Clean up the code since move start moved to main.
/* TODO (GamerMan7799#5#): Consider replacing rand / srand with another form of random generaters
							so we don't need cstdlib anymore */
// TODO (xPUREx#5#): Add a short message the varies with the Software status (ex warning if using a beta or alpha)
/**********************************************************************************************************************************************/
//Globals
namespace Global {
    //Holds if in debug mode or not. Causes more messages to appear in the console.
    //Defined in version.h
    #ifndef DEFINED_BUILD_MODE_PRIVATE
        const bool blnDebugMode = false;
    #else
        const bool blnDebugMode = true;
    #endif // DEFINED_BUILD_MODE_PRIVATE

	bool blnError = false; //if there was an error; then the program will exit when it checks this.

	//Call all of the classes as globals so they are the same everywhere.
	clsConfig Cnfg;
	clsEntity Enty;
	clsMap Map;
	clsTick Tick;
};
/**********************************************************************************************************************************************/
int main(int argc, char *argv[]) {
	Global::Cnfg.Check(); //Load the config file's values
	if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}

    Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();

    clsScreen Screen;
    if (!CnfgValues.blnShowMap) {Screen.~clsScreen();}

    if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}

	//Seed rand as defined in the config options.
	if (CnfgValues.blnAppendTime) {srand(time(NULL) + CnfgValues.uintSeed);}
	else {srand(CnfgValues.uintSeed);}

	Global::Map.load();
	if (Global::blnDebugMode) {printf("Map loaded\n");}
	if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
	Global::Map.restart();
	if (Global::blnDebugMode) {printf("Map Restarted\n");}
	if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
	//Global::Map.show();
	//getchar();

    //Starts the main part of the program
	//that will loop through each generation

	if (CnfgValues.blnLogging) {/*Open log file to clear it*/ Global::Enty.logFile = fopen(DEFINED_LOG_FILE_NAME,"w"); fclose(Global::Enty.logFile);}

	char chrPlayerStatus = 0;
	Global::Enty.uintGenSteps = 0;
	Global::Enty.uchrGenNum = 1;

	//First Generation
	for (Global::Enty.uchrPlayerNum = 0; Global::Enty.uchrPlayerNum < DEFINED_PLAYERS_PER_GEN; Global::Enty.uchrPlayerNum++) {
		Global::Enty.makeplayer();
		for (uint step = 0; step < CnfgValues.uintFirstGen; step++) {
			chrPlayerStatus = Global::Map.move(Global::Enty.plyPlayer.direction[step]);
			Global::Enty.getFitness();

			if (CnfgValues.blnShowMap) {Screen.update();}
			if (chrPlayerStatus == DEAD) {
				//If the player dies clear the rest of their directions (disabled) and end the loop.
				//for (uint j = step; j < Global::Cnfg.values.uintFirstGen; j++) {plyPlayer.direction[j] = dirNone;}
				step = CnfgValues.uintFirstGen;
				//if (CnfgValues.blnShowMap) {Global::Map.playerDeath();}
			} //end if dead
		} //End for steps
		Global::Enty.nextplayer();
		if (CnfgValues.blnShowMap) {Global::Map.show();}
	}//end for first gen

	Global::Enty.getBest();
	if(!(CnfgValues.blnShowMap)) {
		printf("Best Players are:\n");
		for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",Global::Enty.genBestPlayers[k].fitness);}
		if (Global::blnDebugMode) {getchar();}
	}

	Global::Enty.uintGenSteps += CnfgValues.uintFirstGen;

	//Growth Generation
	while (Global::Enty.uintGenSteps + CnfgValues.uintGenIncrease <= DEFINED_MAX_PLAYER_STEPS) {
		Global::Enty.uchrGenNum ++;
		for (Global::Enty.uchrPlayerNum = 0; Global::Enty.uchrPlayerNum < DEFINED_PLAYERS_PER_GEN; Global::Enty.uchrPlayerNum++) {
			Global::Enty.makeplayer();
			for (uint step = 0; step < Global::Enty.uintGenSteps + CnfgValues.uintGenIncrease; step++) {
				chrPlayerStatus = Global::Map.move(Global::Enty.plyPlayer.direction[step]);
                Global::Enty.getFitness();

				if (CnfgValues.blnShowMap) {Screen.update();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of their directions (disabled) and end the loop.
					//for (uint j = step; j < uintGenSteps + Global::Cnfg.values.uintGenIncrease; j++) {plyPlayer.direction[j] = dirNone;}
					step = Global::Enty.uintGenSteps + CnfgValues.uintGenIncrease;
					//if (CnfgValues.blnShowMap) {Global::Map.playerDeath();}
				} //end if dead
			}//end for steps
			Global::Enty.nextplayer();
			if (CnfgValues.blnShowMap) {Screen.update();}
		} //end for players
		Global::Enty.getBest();
		if(!(CnfgValues.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",Global::Enty.genBestPlayers[k].fitness);}
			if (Global::blnDebugMode) {getchar();}
		}
		Global::Enty.uintGenSteps += CnfgValues.uintGenIncrease;
	}//end while loop

	//Steady Generations
	for (uint i = 0; i < CnfgValues.uintGensPastGrowth; i++) {
		Global::Enty.uchrGenNum ++;
		for (Global::Enty.uchrPlayerNum = 0; Global::Enty.uchrPlayerNum < DEFINED_PLAYERS_PER_GEN; Global::Enty.uchrPlayerNum++) {
			Global::Enty.makeplayer();
			for (uint step = 0; step < DEFINED_MAX_PLAYER_STEPS; step++) {
				chrPlayerStatus = Global::Map.move(Global::Enty.plyPlayer.direction[step]);
                Global::Enty.getFitness();

				if (CnfgValues.blnShowMap) {Screen.update();}
				if (chrPlayerStatus == DEAD) {
					//If the player dies clear the rest of their directions (disabled) and end the loop.
					//for (uint j = step; j < DEFINED_MAX_PLAYER_STEPS; j++) {plyPlayer.direction[j] = dirNone;}
					step = DEFINED_MAX_PLAYER_STEPS;
					//if (CnfgValues.blnShowMap) {Global::Map.playerDeath();}
				} //end if dead
			}//end for steps
			Global::Enty.nextplayer();
			if (CnfgValues.blnShowMap) {Screen.update();}
		} //end for players
		Global::Enty.getBest();
		if(!(CnfgValues.blnShowMap)) {
			printf("Best Players are:\n");
			for (uint k = 0; k < DEFINED_BEST_PLAYER_NUM; k++) {printf("%2.3f\n",Global::Enty.genBestPlayers[k].fitness);}
			if (Global::blnDebugMode) {getchar();}
		}
	}//end for loop


	if (Global::blnDebugMode) {printf("Generations finished.\n");}
	if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}

    Screen.~clsScreen();

	printf("\nDone\n");

	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
