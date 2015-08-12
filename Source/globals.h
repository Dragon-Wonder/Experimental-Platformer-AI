#ifndef __GLOBAL__HEADER__
#define __GLOBAL__HEADER__
/**********************************************************************************************************************************************/
//This will hold all of the global values.
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
	//char* programverison;
	
	//Call all of the classes as globals so they are the same everywhere.
	extern Config Cnfg;
	extern Entity Enty;
	extern Map Course;
	extern Tick Tock;
};
/**********************************************************************************************************************************************/
#endif
