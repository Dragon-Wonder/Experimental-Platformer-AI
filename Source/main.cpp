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
	const bool blnDebugMode = false;
	bool blnError = false;
	//const char* programverison = PROGRAM_VERISON;
};
/**********************************************************************************************************************************************/
int main(void) {
	Config Cnfg;
	Cnfg.Check();
	ErrorCheck
	
	if (Cnfg.values.blnAppendTime) {srand(time(NULL) + Cnfg.values.uintSeed);}
	else {srand(Cnfg.values.uintSeed);}
	
	Map m;
	m.load();
	ErrorCheck
	m.restart();
	ErrorCheck
	m.show();
	getchar();
	
	Entity plyr;
	plyr.start();
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
