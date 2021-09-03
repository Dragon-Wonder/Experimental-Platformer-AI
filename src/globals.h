#ifndef __GLOBAL__HEADER__
#define __GLOBAL__HEADER__
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file globals.h
/// @brief This file holds all of the references to variables declared in the
///        Global namespace. See Main.cpp for more details
/////////////////////////////////////////////////
/*****************************************************************************/
#include <vector>
/*****************************************************************************/
//Make some prototype classes
class clsConfig;
class clsEntity;
class clsMap;
class clsTick;
/*****************************************************************************/
//Globals
namespace global {
	extern const bool blnDebugMode; // Holds if in debug mode or not.
                                  // Causes more messages to appear.
	extern bool blnError; // if there was an error
	extern bool blnQuit;  // if a quit has been called

	// Call all of the classes as globals so they are the same everywhere.
	// Likely really bad way to handle this, but I'll fix it later
	extern clsConfig cnfg;
	extern clsEntity enty;
	extern clsMap mymap;
	extern clsTick tick;
	namespace physics { // Constants that are gonna be used for
                      // physics in later versions
    extern const float fGravityAcceleration;
    extern const float fFriction;
    extern const float fMaxVelocity;
    extern const float fMonsMaxVelocity;
    extern const float fIncVelocity; // How much velocity increments
                                     // by for an input
    extern const float fRecoil; //Recoil for when hitting an object
	};
};

/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup ChangeableDefines Easily Changeable Defines
/// @brief These are defines that can change key aspects of the program, but
///        shouldn't need to be changed often.
/// @{
/////////////////////////////////////////////////

namespace defined {
  constexpr unsigned char kGoalFPS = 30;
    /**< What the target FPS is for the program, only effects things when show map is true
    See tick.cpp for what it does */
  constexpr unsigned char kMaxJumpCount = 2;
    /**< The max number of times a player can jump before hitting the ground again. */
  constexpr unsigned int kCoinWorth = 100;
    /**< The points the player gets for collecting a coin. */
  constexpr unsigned int kMonsterKillWorth = 500;
    /**< The points the player gets for killing a monster. */
  constexpr unsigned int kTickLimit = 300;
    /**< How many seconds to complete the level, before player is killed */
  constexpr unsigned char kNumMapTiles = 11;
    /**< How many different map tiles there are (used to make the array for clipping). */
  constexpr unsigned char kPicSize = 24;
    /**< Size of each tile in pixels. */
  constexpr char* kConfigFileName = "Config.ini";
    /**< The name of the config file that will be made / read from. */
  constexpr char* kLogFileName = "player.log";
    /**< The name of the log file that will have player actions written to. */
  constexpr unsigned char kMapHeight = 14;
    /**< This defines the Map height, unless a map you are trying to load is more than
          this it should be kept at 14 */
  constexpr unsigned char kMapWidth = 217;
    /**< This defines the Map width, unless a map you are trying to load is more than
      this it should be kept at 217 */
  constexpr unsigned char kNumTools = 4;
    /**< Number of tools in image_toos.xpm */
}
/// @}
/*****************************************************************************/
/// @addtogroup TypeDefs
/// @{
typedef std::vector<char> VectorSteps;
/// @}
/*****************************************************************************/
//Structures for varies uses
/** @struct stcLoc
    Holds the location of the Monster / Player in terms of pixels.
    It is its own structure so that I can pass these values together through functions */
struct stcLoc {
	int x; /**< X direction value. */
	int y; /**< Y direction value. */
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
	VectorSteps steps; /**< All of the player's inputs. */
}; //prefix = gen

/** @struct stcPlayer
    Holds all of the values needed to process the current player. */
struct stcPlayer {
	struct stcLoc location; /**< Player's location. */
	struct stcVel vel; /**< Player's velocity. */
	float fitness; /**< Player's fitness. */
	unsigned int score; /**< Player's score (gained by collecting coins and killing monsters
                             it will effect their fitness. */
	VectorSteps direction; /**< All of the player's inputs. */
	unsigned char state; /**< Player's state (examples: left, right, jumping, etc...). Currently does nothing. */
}; //prefix = ply

/** @struct stcBasePlayer
    Holds values for a skeleton "base" of a player, this is mostly used
    when passing these values through functions so that it is easier */
struct stcBasePlayer {
	struct stcLoc location; /**< Location. */
	struct stcVel vel; /**< Velocity. */
}; //prefix = bply

/** @struct stcMonster
    Holds all of the values needed for the monsters */
struct stcMonster {
	struct stcLoc location; /**< Monster's location. */
	struct stcVel vel; /**< Monster's velocity. */
	bool living; /**< Whether or not the monster is alive. Will stop updating if false. */
	unsigned char state; /**< Monster's state (examples: left, right, jumping, etc...). Currently does nothing. */
}; //prefix = mst
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup enum Enumerators
/// @brief These are all the different enums that are used throughout the code.
/// @{
/////////////////////////////////////////////////

/** Holds all the different tiles that are in tiles.png. */
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

/** Holds the different menu selections. */
enum enumMenu {
  menuQuit = 0,
  menuNew,
  menuEditor,
  menuAbout,
  menuOptions,
  menuError,
  menuSave
};

/** Holds the different prompt types */
enum promptTypes {
  promptYesNo = 0,
  promptOkay,
  promptNew,
  promptPrompt,
  promptUse
};

/** Holds the different returns from a prompt*/
enum promptReturns {
  returnYes = 0,
  returnNo,
  returnError,
  returnMaybe
};

/** Holds the different tools */
enum enumTools {
  toolQuit = 0,
  toolSave,
  toolLeft,
  toolRght
};
// end enum groups
/// @}
/*****************************************************************************/
/// @addtogroup TypeDefs
/// @{
typedef struct stcGeneration GEN; /**< Change structure typedef for easier reference. */
typedef struct stcPlayer PLYR; /**< Change structure typedef for easier reference. */
typedef struct stcMonster MNSTR; /**< Change structure typedef for easier reference. */
typedef struct stcLoc LOC; /**< Change structure typedef for easier reference. */
typedef struct stcVel VEL; /**< Change structure typedef for easier reference. */
typedef struct stcBasePlayer BPLYR; /**< Change structure typedef for easier reference. */
typedef std::vector<MNSTR> VectorMonsters; /**< Define a vector for the monsters */
typedef std::vector<GEN> VectorGeneration; /**< Define a vector for the generation */
typedef struct stcLoc OFFST; /**< Define a map offset */

//Ahh laziness at its finest
typedef unsigned char uchar; /**< Change unsigned char to uchar because I'm lazy. */
typedef unsigned int uint; /**< Change unsigned int to uint because I'm lazy. */
typedef unsigned long ulong; /**< Change unsigned long to ulong because I'm lazy. */
/// @}
/*****************************************************************************/
#endif
