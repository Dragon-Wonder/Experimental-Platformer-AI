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
		uint uintStepNum;
		GEN pastplayers[DEFINED_PLAYERS_PER_GEN];
		GEN bestplayers[DEFINED_BEST_PLAYER_NUM];
		uint uintGenSteps;
		const char* FileName = DEFINED_LOG_FILE_NAME;
		FILE* logFile;
		
		PLYR player;
		MNSTR *monsters;
		
	public :
		//Default Constructor
		Entity();
		~Entity();
		
		//Members
		uchar genNum;
		uchar playerNum;
		
		//Functions
		void start(void);
		void killMonster(uchar,uchar);
		void allocateMonsters(uchar);
		
		MNSTR getMonster(uchar);
		void setMonster(uchar, MNSTR);
		
		PLYR getPlayer(void);
		void setPlayer(PLYR);
};
/**********************************************************************************************************************************************/
#endif

