#ifndef __ENTITY__HEADER__
#define __ENTITY__HEADER__
/**********************************************************************************************************************************************/
#include <cstdio>
#include <new>
/**********************************************************************************************************************************************/
#include "main.h"
/**********************************************************************************************************************************************/
enum stage { //This is for what stage of the generation the program is in
    stageFirst = 0,
    stageGrowth, //1
    stageSteady
};

enum status { //Status of the player including deaths
    statusLiving = 0, //Player is still alive
    deathInputs, //1, Player ran out of inputs
    deathMonster, //2, Player ran into monster
    deathClock, //3, Player ran out of time
    deathFall, //4, Player fell through hole
    deathDecay, //5, death from hard mode & player fitness getting too low.
    deathStupid, //6, death from hard mode & player going too far left.
    deathGeneric, //7, Generic death
    deathError //8, and error happened and the player had to die
};

enum state { //State of Player or monster (walking, jumping, death, attacking, etc...)
    stateRest = 0,
    stateJump,//1
    stateLeft,//2
    stateRight,//3
    stateDuck,//4
    stateFalling,//5
    stateDead //6
};
/**********************************************************************************************************************************************/
class clsEntity {
	//private :
	public:
		//Functions
		void getFitness(void);
		void getBest(void);
		void nextplayer(char);
		//Members
		uint uintStepNum;
		GEN genPastPlayers[DEFINED_PLAYERS_PER_GEN];
		GEN genBestPlayers[DEFINED_BEST_PLAYER_NUM];
		uint uintGenSteps;
		const char* FileName = DEFINED_LOG_FILE_NAME;
        FILE* logFile;
		PLYR plyPlayer;
		MNSTR *pmstMonsters;
	//public :
		//Default Constructor
		clsEntity();
		~clsEntity();
		//Members
		uchar uchrGenNum;
		uchar uchrPlayerNum;
		//Functions
		void killMonster(uchar,uchar);
		void allocateMonsters(uchar);
		MNSTR getMonster(uchar);
		void setMonster(uchar, MNSTR);
		PLYR getPlayer(void);
		BPLYR getPlayerBase(void);
		char getPlayerState(void);
		void setPlayerState(char);
		void setPlayer(BPLYR);
		char doPlayerStep(uint, char);
		void doNextGeneration(char);
		void newplayer(void);
};
/**********************************************************************************************************************************************/
#endif

