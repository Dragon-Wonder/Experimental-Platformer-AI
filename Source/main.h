#ifndef __MAIN__HEADER__
#define __MAIN__HEADER__
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file Main.h
/// @brief This is the main header, and defines many values, and structures that
///        are used a lot of other places in the code. This header keeps them all
///        centrally referenced.
/////////////////////////////////////////////////
/*****************************************************************************/
/** This defines the Map height, unless a map you are trying to load is more than
    this it should be kept at 14 */
#define DEFINED_MAP_HEIGHT 14

/** This defines the Map width, unless a map you are trying to load is more than
    this it should be kept at 217 */
#define DEFINED_MAP_WIDTH 217
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup ChangeableDefines Easily Changeable Defines
/// @brief These are defines that can be changed easily and without much issues.
/// @{
/////////////////////////////////////////////////


/** What the target FPS is for the program, only effects things when show map is true
    See tick.cpp for what it does */
#define DEFINED_GOAL_FPS 30

/** The most steps a single player can take. */
#define DEFINED_MAX_PLAYER_STEPS 1000

/** The number of players in each generation. */
#define DEFINED_PLAYERS_PER_GEN 40

/** The max number of times a player can jump before hitting the ground again. */
#define DEFINED_MAX_JUMP_COUNT 2

/** The points the player gets for collecting a coin. */
#define DEFINED_COIN_WORTH 100

/** The points the player gets for killing a monster. */
#define DEFINED_MONS_KILL_POINTS 500

/** How many seconds to complete the level, before player is killed */
#define DEFINED_TICK_LIMIT 300

/** How many best players are called at the end of each generation. */
#define DEFINED_BEST_PLAYER_NUM 10

/** How many different map tiles there are (used to make the array for clipping). */
#define DEFINED_NUM_MAP_TILES 11

/** Size of each tile in pixels. */
#define DEFINED_PIC_SIZE 24

/** Setting this to 1 will switch the program from being AI based to being Human based.
    It is useful to debugging. */
#define DEFINED_BUILD_HUMAN 1

/** The name of the config file that will be made / read from. */
#define DEFINED_CONFIG_FILE_NAME "Config.ini"

/** The name of the log file that will have player actions written to. */
#define DEFINED_LOG_FILE_NAME "Player.log"

/// @}
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup ConfigReturns Returns for the Config
/// @brief These are defines that just allow returns from clsConfig::verisonCheck
///        to be more human readable.
/// @{
/////////////////////////////////////////////////

/** Return to make a new config file. */
#define NEWCONFIG 'N'

/** Return to use the current config file. */
#define USECONFIG 'U'

/** Return to prompt the user if they should use / make new config file. */
#define PROMPTUSER 'P'
/// @}
/*****************************************************************************/
//Structures for varies uses

/** @struct stcLoc
    Holds the location of the Monster / Player in terms of pixels.
    It is its own structure so that I can pass these values together through functions */
struct stcLoc {
	unsigned int x; /**< X direction value. */
	unsigned int y; /**< Y direction value. */
}; //prefix = loc

/** @struct stcVel
    Holds the velocity of the Monster / Player in terms of pixels / second.
    It is its own structure so that I can pass these values together through functions */
struct stcVel {
    float x; /**< X direction value. */
    float y; /**< Y direction value. */
}; //prefix = vel

/** @struct stcGeneration
    Holds a single player's values that are needed when going to the next generation
    It is different from stcPlayer because I only need the player's fitness and inputs. */
struct stcGeneration {
	float fitness; /**< The fitness of the player. */
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS]; /**< All of the player's inputs. */
}; //prefix = gen

/** @struct stcPlayer
    Holds all of the values needed to process the current player. */
struct stcPlayer {
	struct stcLoc location; /**< Player's location. */
	struct stcVel vel; /**< Player's velocity. */
	float fitness; /**< Player's fitness. */
	unsigned int score; /**< Player's score (gained by collecting coins and killing monsters
                             it will effect their fitness. */
	unsigned char direction[DEFINED_MAX_PLAYER_STEPS]; /**< All of the player's inputs. */
	unsigned char state; /**< Player's state (examples: left, right, jumping, etc...). Currently does nothing. */
}; //prefix = ply

/** @struct stcBasePlayer
    Holds values for a skeleton "base" of a player, this is mostly used
    when passing these values through functions so that it is easier */
struct stcBasePlayer {
	struct stcLoc location; /**< Location. */
	struct stcVel vel; /** Velocity. */
}; //prefix = bply

/** @struct stcMonster
    Holds all of the values needed for the monsters */
struct stcMonster {
	struct stcLoc location; /**< Monster's location. */
	struct stcVel vel; /**< Monster's velocity. */
	bool living; /**< Whether or not the monster is alive. Will stop updating if false. */
	unsigned char state; /**< Monster's state (examples: left, right, jumping, etc...). Currently does nothing. */
}; //prefix = mst

/** @enum dir
  * The different directions that are possible for the player / monster in
  * an enum for easier reference,
  */

enum dir {
	dirNone = 0, /**< Not moving / no input. */
	dirUp, /**< Jumping. */
	dirLeft, /**< Moving to the left. */
	dirRight, /**< Moving to the right. */
	dirDown /**< Crouching / dropping to the ground if in air. */
};

typedef struct stcGeneration GEN; /**< Change structure typedef for easier reference. */
typedef struct stcPlayer PLYR; /**< Change structure typedef for easier reference. */
typedef struct stcMonster MNSTR; /**< Change structure typedef for easier reference. */
typedef struct stcLoc LOC; /**< Change structure typedef for easier reference. */
typedef struct stcVel VEL; /**< Change structure typedef for easier reference. */
typedef struct stcBasePlayer BPLYR; /**< Change structure typedef for easier reference. */

//Ahh laziness at its finest
typedef unsigned char uchar; /**< Change unsigned char to uchar because I'm lazy. */
typedef unsigned int uint; /**< Change unsigned int to uint because I'm lazy. */
typedef unsigned long ulong; /**< Change unsigned long to ulong because I'm lazy. */
/*****************************************************************************/
#endif
