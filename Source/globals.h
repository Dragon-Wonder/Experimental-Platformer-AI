#ifndef __GLOBAL__HEADER__
#define __GLOBAL__HEADER__
/**********************************************************************************************************************************************/
//This will hold the global namespace with the global values.
/**********************************************************************************************************************************************/
//Call the headers so the classes can be found.
#include "config.h"
#include "tick.h"
#include "map.h"
#include "entity.h"
/**********************************************************************************************************************************************/
//Globals
namespace Global {
	extern const bool blnDebugMode; //Holds if in debug mode or not. Causes more messages to appear in the console
	extern bool blnError; //if there was an error; then the program will exit when it checks this.
	
	//Call all of the classes as globals so they are the same everywhere.
	extern clsConfig Cnfg;
	extern clsEntity Enty;
	extern clsMap Map;
	extern clsTick Tick;
};
/**********************************************************************************************************************************************/
#endif
