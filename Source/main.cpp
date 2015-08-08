/**********************************************************************************************************************************************/
//The main function, most of the acuatual program is found in Entity.cpp under start. 
//So see there for most of the funtions.
/**********************************************************************************************************************************************/
#include "main.h"
#include "map.h"
#include "config.h"
#include "entity.h"
#include "tick.h"
/**********************************************************************************************************************************************/
//Globals
namespace Global {
	const bool blnDebugMode = true;
	bool blnError = false;
	//const char* programverison = PROGRAM_VERISON;
};
/**********************************************************************************************************************************************/
int main(void) {
	Entity plyr;
	Config Cnfg;
	Cnfg.Check();
	ErrorCheck
	
	if (Cnfg.values.blnAppendTime) {srand(time(NULL) + Cnfg.values.uintSeed);}
	else {srand(Cnfg.values.uintSeed);}
	
	plyr.genNum = 0;
	plyr.playerNum = 0;
	
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
	free(m.basemonsters);
	free(plyr.monsters);
	
	getchar();
	return 0;
}
/**********************************************************************************************************************************************/
