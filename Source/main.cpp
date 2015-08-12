/**********************************************************************************************************************************************/
//The main function, most of the actual program is found in Entity.cpp under start. 
//So see there for most of the functions.
/**********************************************************************************************************************************************/
#include "main.h"
#include "config.h"
#include "tick.h"
#include "map.h"
#include "entity.h"
#include "globals.h"
/**********************************************************************************************************************************************/
//Foreward Declare Classes because of issues
class Config;
class Entity;
class Map;
class Tick;
/**********************************************************************************************************************************************/
//Globals
namespace Global {
	const bool blnDebugMode = true; //Holds if in debug mode or not. Causes more messages to appear in the console.
	bool blnError = false; //if there was an error; then the program will exit when it checks this.
	//const char* programverison = DEFINED_PROGRAM_VERSION;
	
	//Call all of the classes as globals so they are the same everywhere.
	Config Cnfg;
	Entity Enty;
	Map Course;
	Tick Tock;
};
/**********************************************************************************************************************************************/
int main(void) {
	Global::Cnfg.Check(); //Load the config file's values
	ErrorCheck 
	
	//Seed rand as defined in the config options.
	if (Global::Cnfg.values.blnAppendTime) {srand(time(NULL) + Global::Cnfg.values.uintSeed);}
	else {srand(Global::Cnfg.values.uintSeed);}

	Global::Course.load();
	if (Global::blnDebugMode) {printf("Map loaded\n");}
	ErrorCheck
	Global::Course.restart();
	if (Global::blnDebugMode) {printf("Map Restarted\n");}
	ErrorCheck
	Global::Course.show();
	getchar();
	
	
	Global::Enty.start();
	if (Global::blnDebugMode) {printf("Generations finished.\n");}
	ErrorCheck
	
	printf("\nDone\n");
	
	//Destructe everything
	Global::Cnfg.~Config();
	Global::Enty.~Entity();
	Global::Course.~Map();
	Global::Tock.~Tick();
	
	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
