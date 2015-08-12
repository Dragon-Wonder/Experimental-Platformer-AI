#ifndef __ENTITY__HEADER__
#define __ENTITY__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <cstdio>
#include <cstdlib>
/**********************************************************************************************************************************************/
class clsEntity {
	private :
		//Functions
		float getFitness(void);
		void getBest(void);
		void nextplayer(void);
		void makeplayer(void);
		
		//Members
		uint uintStepNum;
		GEN genPastPlayers[DEFINED_PLAYERS_PER_GEN];
		GEN genBestPlayers[DEFINED_BEST_PLAYER_NUM];
		uint uintGenSteps;
		const char* FileName = DEFINED_LOG_FILE_NAME;
		FILE* logFile;
		
		PLYR plyPlayer;
		MNSTR *pmstMonsters;
		
	public :
		//Default Constructor
		clsEntity();
		~clsEntity();
		
		//Members
		uchar uchrGenNum;
		uchar uchrPlayerNum;
		
		//Functions
		void start(void);
		void killMonster(uchar,uchar);
		void allocateMonsters(uchar);
		
		MNSTR getMonster(uchar);
		void setMonster(uchar, MNSTR);
		
		PLYR getPlayer(void);
		void setPlayer(LOC);
};
/**********************************************************************************************************************************************/
#endif

