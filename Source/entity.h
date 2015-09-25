#ifndef __ENTITY__HEADER__
#define __ENTITY__HEADER__
/*****************************************************************************/
#include <cstdio>
#include <new>
/*****************************************************************************/
#include "main.h"
/*****************************************************************************/
/** @enum stage
  * Enum for what stage the program is in.
  */
enum stage {
    stageFirst = 0, /**< 0: The First Generation. */
    stageGrowth,    /**< 1: The Growth generations. */
    stageSteady     /**< 2: The Generations at max steps. */
};

/** @enum status
  * This is the status of the player. It is mostly used to record a player's death.
  */
enum status {
    statusLiving = 0,   /**< 0: Player is still alive. */
    deathInputs,        /**< 1: Player ran out of inputs. */
    deathMonster,       /**< 2: Player ran into monster. */
    deathClock,         /**< 3: Player ran out of time. */
    deathFall,          /**< 4: Player fell through hole. */
    deathDecay,         /**< 5: Death from hard mode & player fitness getting too low. */
    deathStupid,        /**< 6: Death from hard mode & player going too far left. */
    deathGeneric,       /**< 7: Generic death (catch in case player died without death being recorded). */
    deathError          /**< 8: an error happened and the player had to die, (usually only if
                                the player got stuck in the wall or something). */
};
/** @enum state
  * The state of the player. Will be used later on to change the image being used.
  * (to show player facing left or jumping) but currently does nothing.
  */
enum state {
    stateRest = 0,  /**< 0: Doing nothing */
    stateJump,      /**< 1: Jumping */
    stateLeft,      /**< 2: Moving / Facing left */
    stateRight,     /**< 3: Moving / Facing right */
    stateDuck,      /**< 4: Ducking */
    stateFalling,   /**< 5: Falling */
    stateDead       /**< 6: Dead */
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsEntity entity.h "source/entity.h"
/// @brief This class will hold all of the values and functions related to the
///        entities on the map; these include (at the moment) the player, and the
///        monsters.
/////////////////////////////////////////////////

class clsEntity {
	//private :
	public:
		//Functions
		void getFitness(void);
		void getBest(void);
		void nextplayer(char);
		//Members

		/** The current step number of the player. */
		uint uintStepNum;

		/** The array which holds all of the past players. */
		GEN genPastPlayers[DEFINED_PLAYERS_PER_GEN];

		/** The array which holds all of the BEST players from a generation. */
		GEN genBestPlayers[DEFINED_BEST_PLAYER_NUM];

		/** The number of steps the current generation has. */
		uint uintGenSteps;

		/** The log file name. */
		const char* FileName = DEFINED_LOG_FILE_NAME;

		/** Pointer reference to the log file open in memory. */
        FILE* logFile;

        /** The current player. */
		PLYR plyPlayer;

		/** Pointer to the monsters, this is made into a dynamic array. */
		MNSTR *pmstMonsters;
	//public :
		//Default Constructor
		clsEntity();
		~clsEntity();
		//Members
		/** The current generation number. */
		uchar uchrGenNum;

		/** The current player number for that generation. */
		uchar uchrPlayerNum;

		//Functions
		void killMonster(LOC);
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
/*****************************************************************************/
#endif

