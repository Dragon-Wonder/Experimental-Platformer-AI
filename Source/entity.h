#ifndef __ENTITY__HEADER__
#define __ENTITY__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************/
class Entity {
	private :
		//Functions
		float getFitness(void);
		void getBest(void);
		void nextplayer(void);
		void makeplayer(void);
		
		//Members
		static uint uintStepNum;
		static GEN pastplayers[Players_Per_Generation];
		static GEN bestplayers[10];
		static uint uintGenSteps;
		const char* FileName = LogFileName;
		FILE* logFile;
	
	public :
		//Default Constructor
		Entity();
		~Entity();
		
		//Members
		static PLYR player;
		static MNSTR *monsters;
		static uchar genNum;
		static uchar playerNum;
		
		//Functions
		void start(void);
		void killMonster(uchar,uchar);
		void allocateMonsters(uchar);
		void placeBaseMonsters(uchar, uchar, uchar);
};

/**********************************************************************************************************************************************/
#endif

