/*****************************************************************************/
/////////////////////////////////////////////////
/// @file       Main.cpp
/// @brief      Holds all the main functions.
/// @author     GamerMan7799
/// @author     xPUREx
/// @version    6.0.0-alpha.1
/// @date       2015-2019
/// @copyright  Public Domain Unlicense.
/////////////////////////////////////////////////
/*****************************************************************************/
#include "core/core.h"
#include "globals.h"
/*****************************************************************************/
/** This is the namespace that is meant to hold things that need to be
    referenced between different files */
namespace global {
  /** This will hold if this build is meant for debugging. While in debugging
      mode the program will print many more messages to the console.
      It is set to true if DEFINED_BUILD_MODE_PRIVATE is defined in
      version.h or during compiling. */
#ifndef DEFINED_BUILD_MODE_PRIVATE
  const bool blnDebugMode = false;
#else
  const bool blnDebugMode = true;
#endif // DEFINED_BUILD_MODE_PRIVATE


  bool blnError = false; /**< If there was an error; then the program will exit when it checks this. */
  bool blnQuit = false; /**< If the program should quit or not. */

	//Call all of the classes as globals so they are the same everywhere.
	clsConfig cnfg; /**< Call Config Class on a global scope so it may be referenced in other classes */
	clsEntity enty; /**< Call Entity Class on a global scope so it may be referenced in other classes */
	clsMap mymap; /**< Call Map Class on a global scope so it may be referenced in other classes */
	clsTick tick; /**< Call Tick Class on a global scope so it may be referenced in other classes */

  /** This namespace holds values based on the physics that are used in the
      program */
  namespace physics {
    const float fGravityAcceleration = 9.81 * defined::kPicSize; /**< The acceleration downward due to gravity.
                                                   in pixels / sec<sup>2</sup> */
    const float fFriction = 0.089; /**< Velocity will be decreased by this number times
                                         current velocity. */
    const float fMaxVelocity = 200; /**< The max velocity (pixels / sec) the player can reach. */
    const float fMonsMaxVelocity = 44.25; /**< The max velocity (pixels / sec) the monster can reach. */
    const float fIncVelocity = 32; /**< How much velocity (pixels / sec) increments by for an input. */
    const float fRecoil = -0.595; /**< When the player or monster runs into a wall their velocity will
                                        be multiplied by this value. The negative means that they will bounce
                                        off the wall. Setting it to 0 will cause them to come to a complete stop. */
	}; //end namespace physics
}; //end namespace Global
/*****************************************************************************/
int main(int argc, char *argv[]) {
  /////////////////////////////////////////////////
  /// @brief Holds the main function.
  ///
  /// @param argc = Something required by SDL
  /// @param argv = Something required by SDL
  /// @return 1 / 0: 1 Means exiting with an error. 0 means exiting as program
  ///                  has completed its run.
  ///
  /////////////////////////////////////////////////

  if ( DEFINED_VER_STATUS_SHORT == "a" ) {
    printf("This version is an Alpha, meaning that we cannot be sure that it works correctly. ");
    printf("Any features that you see may or may not be changed/removed later in the developmental cycle.\n\n");
  } else if ( DEFINED_VER_STATUS_SHORT == "b" ) {
    printf("This version is a Beta, meaning that while it should work very closely to how we\nintend it to work, ");
    printf("there may still be some bugs that can cause the program to crash.\n\n");
  } else if ( DEFINED_VER_STATUS_SHORT == "rc" ) {
    printf("This version is a Release Candidate, meaning that it is only a few items away from being an Official ");
    printf("Release. There shouldn't be very many bugs but let us know if you find any.\n\n");
  } else if ( DEFINED_VER_STATUS_SHORT == "r" ) {
    printf("This version is the official release for the game. There should not be any bugs, so if you find any ");
    printf("please let us know right away so we can work to fix them.\n\n");
  } else {
    printf("This version is not properly labeled. There is a mistake in the version.h file.\n\n");
  }

  clsCore core;
	return core.start();
} //end main
/*****************************************************************************/
