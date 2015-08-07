#ifndef __ENTITY__HEADER__
#define __ENTITY__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include "config.h"
#include <cstdio>
/**********************************************************************************************************************************************/
class Entity {
	private :
		float getFitness(void);
		uint uintStepNum;
		GEN pastplayers[Players_Per_Generation];
		GEN bestplayers[10];
		uint uintGenSteps;
		FILE* logFile;
		
		
		
	public :
		PLYR player;
		MNSTR *monsters;
		uchar genNum;
		uchar playerNum;
		
		void start(void);
		void killMonster(uchar,uchar);
};

/**********************************************************************************************************************************************/
#endif