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
// TODO (xPUREx#2#): Add a short message the varies with the Software status (ex warning if using a beta or alpha)
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

    //Put all of the config values into CnfgValues for easier reference
    Configures CnfgValues;
	CnfgValues = Global::Cnfg.getvalues();

    clsScreen Screen; //create the screen, here so it is always called
    if (!CnfgValues.blnShowMap) {Screen.~clsScreen();} //deconstruct the screen if we aren't updating the screen.
    //check if there was an error creating the SDL window
    //exit program if there was
    if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
	//Seed rand as defined in the config options.
	if (CnfgValues.blnAppendTime) {srand(time(NULL) + CnfgValues.uintSeed);}
	else {srand(CnfgValues.uintSeed);}
    //Load the base map by locating the monster and player starting locations
	Global::Map.load();
	if (Global::blnDebugMode) {printf("Map loaded\n");}
    //Check if there was an error loading map (usually with monster allocation)
    //exit program if there was.
	if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
	//Place base map back onto map
	Global::Map.restart();
	if (Global::blnDebugMode) {printf("Map Restarted\n");}
    //Check if there was an error restarting map (usually with monsters)
    //exit program if there was.
	if (Global::blnError) {printf("\nThere was an error!\n"); return 1;}
    //Starts the main part of the program
	//that will loop through each generation
	char chrPlayerStatus = 0;
	//First Generation
	for (uint i = 0; i < DEFINED_PLAYERS_PER_GEN; i++) {
		Global::Enty.newplayer();
		for (uint step = 0; step < CnfgValues.uintFirstGen; step++) {
            chrPlayerStatus = Global::Enty.doPlayerStep(step, stageFirst);
			if (chrPlayerStatus != statusLiving) {
                if (CnfgValues.blnShowMap) {Screen.playerDeath();}
                step = DEFINED_MAX_PLAYER_STEPS; //Forces for loop to stop
                Global::Map.restart();
            } else {if (CnfgValues.blnShowMap) {Screen.update();}}
		} //End for steps
		if (CnfgValues.blnShowMap) {Screen.update();}
	}//end for first gen
	Global::Enty.doNextGeneration(stageFirst);

	//Growth Generation
	while (Global::Enty.uintGenSteps + CnfgValues.uintGenIncrease <= DEFINED_MAX_PLAYER_STEPS) {
		for (uint i = 0; i < DEFINED_PLAYERS_PER_GEN; i++) {
			Global::Enty.newplayer();
			for (uint step = 0; step < Global::Enty.uintGenSteps + CnfgValues.uintGenIncrease; step++) {
				 chrPlayerStatus = Global::Enty.doPlayerStep(step, stageGrowth);
                if (chrPlayerStatus != statusLiving) {
                    if (CnfgValues.blnShowMap) {Screen.playerDeath();}
                    step = DEFINED_MAX_PLAYER_STEPS; //Forces for loop to stop
                    Global::Map.restart();
                } else {if (CnfgValues.blnShowMap) {Screen.update();}}
			}//end for steps
			if (CnfgValues.blnShowMap) {Screen.update();}
		} //end for players
		Global::Enty.doNextGeneration(stageGrowth);
	}//end while loop

	//Steady Generations
	for (uint i = 0; i < CnfgValues.uintGensPastGrowth; i++) {
		for (uint j = 0; j < DEFINED_PLAYERS_PER_GEN; j++) {
			Global::Enty.newplayer();
			for (uint step = 0; step < DEFINED_MAX_PLAYER_STEPS; step++) {
                chrPlayerStatus = Global::Enty.doPlayerStep(step, stageSteady);
                if (chrPlayerStatus != statusLiving) {
                    if (CnfgValues.blnShowMap) {Screen.playerDeath();}
                    step = DEFINED_MAX_PLAYER_STEPS; //Forces for loop to stop
                    Global::Map.restart();
                } else {if (CnfgValues.blnShowMap) {Screen.update();}}
			}//end for steps
			if (CnfgValues.blnShowMap) {Screen.update();}
		} //end for players
		Global::Enty.doNextGeneration(stageSteady);
	}//end for gens loop


	if (Global::blnDebugMode) {printf("Generations finished.\n");}
    Screen.~clsScreen();
	printf("\nDone\n");
	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
