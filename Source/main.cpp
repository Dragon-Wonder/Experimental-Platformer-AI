/**********************************************************************************************************************************************/
//The main function, most of the actual program is found in Entity.cpp under start. 
//So see there for most of the functions.
/**********************************************************************************************************************************************/
#include "main.h"
#include "map.h"
#include "config.h"
#include "entity.h"
#include "tick.h"
/**********************************************************************************************************************************************/
//Globals
namespace Global {
	const bool blnDebugMode = true; //Holds if in debug mode or not. Causes more messages to appear in the console.
	bool blnError = false; //if there was an error; then the program will exit when it checks this.
	//const char* programverison = PROGRAM_VERSION;
};
/**********************************************************************************************************************************************/
int main(void) {
	Entity plyr;
	Config Cnfg;
	Cnfg.Check(); //Load the config file's values
	ErrorCheck 
	
	//Seed rand as defined in the config options.
	if (Cnfg.values.blnAppendTime) {srand(time(NULL) + Cnfg.values.uintSeed);}
	else {srand(Cnfg.values.uintSeed);}
	
	Map m;
	m.load();
	if (Global::blnDebugMode) {printf("Map loaded\n");}
	ErrorCheck
	m.restart();
	if (Global::blnDebugMode) {printf("Map Restarted\n");}
	ErrorCheck
	m.show();
	getchar();
	
	
	plyr.start();
	if (Global::blnDebugMode) {printf("Generations finished.\n");}
	ErrorCheck
	
	printf("\nDone\n");
	//Clear dynamic memory used to prevent
	//Memory leaks
	plyr.~Entity();
	m.~Map();
	
	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
