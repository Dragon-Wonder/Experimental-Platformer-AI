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
/**********************************************************************************************************************************************/
//General Todos
// TODO (Patrick.Rye#1#): Move class values back to being non public
// TODO (Patrick#1#): Fitness seems to be decreasing even when Hard Mode is off
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
int main(void) {
	Global::Cnfg.Check(); //Load the config file's values
	ErrorCheck

	//Seed rand as defined in the config options.
	if (Global::Cnfg.getvalues(cnfgAppendTime) == 1) {srand(time(NULL) + Global::Cnfg.getvalues(cnfgSeed));}
	else {srand(Global::Cnfg.getvalues(cnfgSeed));}

	Global::Map.load();
	if (Global::blnDebugMode) {printf("Map loaded\n");}
	ErrorCheck
	Global::Map.restart();
	if (Global::blnDebugMode) {printf("Map Restarted\n");}
	ErrorCheck
	Global::Map.show();
	getchar();


	Global::Enty.start();
	if (Global::blnDebugMode) {printf("Generations finished.\n");}
	ErrorCheck

	printf("\nDone\n");

	//Deconstruct all the classes.
	Global::Cnfg.~clsConfig();
	Global::Enty.~clsEntity();
	Global::Map.~clsMap();
	Global::Tick.~clsTick();

	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
