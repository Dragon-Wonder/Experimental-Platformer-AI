#ifndef __ENTITY__HEADER__
#define __ENTITY__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include "config.h"
#include "tick.h"
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************/
class Entity {
	private :
		float getFitness(void);
		void getBest(void);
		void nextplayer(void);
		void makeplayer(void);
		
		uint uintStepNum;
		GEN pastplayers[Players_Per_Generation];
		GEN bestplayers[10];
		uint uintGenSteps;
		const char* FileName = LogFileName;
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