/*****************************************************************************/
/////////////////////////////////////////////////
/// @file       Main.cpp
/// @brief      Holds all the main functions.
/// @author     GamerMan7799
/// @author     xPUREx
/// @version    5.0.1
/// @date       2017
/// @copyright  Public Domain Unlicense.
/////////////////////////////////////////////////
/*****************************************************************************/
#include "version.h"
#include "main.h"
#include "config.h"
#include "tick.h"
#include "map.h"
#include "entity.h"
#include "globals.h"
#include "screen.h"
/*****************************************************************************/
/** This is the namespace that is meant to hold things that need to be referenced between
    different files */
namespace global {
  /** This will hold if this build is meant for debugging. While in debugging mode the
      program will print many more messages to the console. It is set to true if
      DEFINED_BUILD_MODE_PRIVATE is defined in version.h or during compiling. */
#ifndef DEFINED_BUILD_MODE_PRIVATE
  const bool blnDebugMode = false;
#else
  const bool blnDebugMode = true;
#endif // DEFINED_BUILD_MODE_PRIVATE


	bool blnError = false; /**< If there was an error; then the program will exit when it checks this. */

	//Call all of the classes as globals so they are the same everywhere.
	clsConfig cnfg; /**< Call Config Class on a global scope so it may be referenced in other classes */
	clsEntity enty; /**< Call Entity Class on a global scope so it may be referenced in other classes */
	clsMap mymap; /**< Call Map Class on a global scope so it may be referenced in other classes */
	clsTick tick; /**< Call Tick Class on a global scope so it may be referenced in other classes */

  /** This namespace holds values based on the physics that are used in the program */
  namespace physics {
    const float fGravityAcceleration = 9.81*defined::kPicSize; /** The acceleration downward due to gravity.
                                                   in pixels / sec<sup>2</sup> */
    const float fFriction = 0.089; /** Velocity will be decreased by this number times
                                         current velocity. */
    const float fMaxVelocity = 200; /** The max velocity (pixels / sec) the player can reach. */
    const float fMonsMaxVelocity = 44.25; /** The max velocity (pixels / sec) the monster can reach. */
    const float fIncVelocity = 32; /** How much velocity (pixels / sec) increments by for an input. */
    const float fRecoil = -0.595; /** When the player or monster runs into a wall their velocity will
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


	global::cnfg.Check(); //Load the config file's values
	if (global::blnError) { printf("\nThere was an error!\n"); return 1; }

  //Put all of the config values into CnfgValues for easier reference
  Configures CnfgValues;
  CnfgValues = global::cnfg.getvalues();

  clsScreen Screen; //create the screen, here so it is always called
  if (!CnfgValues.blnShowMap) { Screen.~clsScreen(); } //deconstruct the screen if we aren't updating the screen.
  //check if there was an error creating the SDL window
  //exit program if there was
  if (global::blnError) {printf("\nThere was an error!\n"); return 1;}
#if DEFINED_BUILD_HUMAN != 1
	//Seed rand as defined in the config options.

	srand(CnfgValues.uintSeed + ( CnfgValues.blnAppendTime ? time(NULL) : 0 ) );
  //Load the base map by locating the monster and player starting locations
	global::mymap.load();
	if (global::blnDebugMode) { printf("Map loaded\n"); }
  //Check if there was an error loading map (usually with monster allocation)
  //exit program if there was.
	if (global::blnError) { printf("\nThere was an error!\n"); return 1; }
	//Place base map back onto map
	global::mymap.restart();
	if (global::blnDebugMode) { printf("Map Restarted\n"); }
  //Check if there was an error restarting map (usually with monsters)
  //exit program if there was.
	if (global::blnError) { printf("\nThere was an error!\n"); return 1; }
  //Starts the main part of the program
	//that will loop through each generation
	char chrPlayerStatus = 0;
	//First Generation
	for (uint i = 0; i < CnfgValues.uintPlayersPerGen; ++i) {
		global::enty.newplayer();
		for (uint step = 0; step < CnfgValues.uintFirstGen; ++step) {
      chrPlayerStatus = global::enty.doPlayerStep(step, stageFirst);
			if (chrPlayerStatus != statusLiving) {
        if (CnfgValues.blnShowMap) {Screen.playerDeath();}
        step = CnfgValues.uintMaxPlayerSteps; //Forces for loop to stop
        global::mymap.restart();
      } else {if (CnfgValues.blnShowMap) {Screen.update();}}
		} //End for steps
		if (CnfgValues.blnShowMap) {Screen.update();}
	}//end for first gen
	global::enty.doNextGeneration(stageFirst);

	//Growth Generation
	while (global::enty.uintGenSteps + CnfgValues.uintGenIncrease <= CnfgValues.uintMaxPlayerSteps) {
		for (uint i = 0; i < CnfgValues.uintPlayersPerGen; ++i) {
			global::enty.newplayer();
			for (uint step = 0; step < global::enty.uintGenSteps + CnfgValues.uintGenIncrease; ++step) {
        chrPlayerStatus = global::enty.doPlayerStep(step, stageGrowth);
        if (chrPlayerStatus != statusLiving) {
          if (CnfgValues.blnShowMap) {Screen.playerDeath();}
          step = CnfgValues.uintMaxPlayerSteps; //Forces for loop to stop
          global::mymap.restart();
        } else {if (CnfgValues.blnShowMap) {Screen.update();}}
			}//end for steps
			if (CnfgValues.blnShowMap) {Screen.update();}
		} //end for players
		global::enty.doNextGeneration(stageGrowth);
	}//end while loop

	//Steady Generations
	for (uint i = 0; i < CnfgValues.uintGensPastGrowth; ++i) {
		for (uint j = 0; j < CnfgValues.uintPlayersPerGen; ++j) {
			global::enty.newplayer();
			for (uint step = 0; step < CnfgValues.uintMaxPlayerSteps; ++step) {
        chrPlayerStatus = global::enty.doPlayerStep(step, stageSteady);
        if (chrPlayerStatus != statusLiving) {
          if (CnfgValues.blnShowMap) {Screen.playerDeath();}
          step = CnfgValues.uintMaxPlayerSteps; //Forces for loop to stop
          global::mymap.restart();
        } else {if (CnfgValues.blnShowMap) {Screen.update();}}
			}//end for steps
			if (CnfgValues.blnShowMap) {Screen.update();}
		} //end for players
		global::enty.doNextGeneration(stageSteady);
	}//end for gens loop


	if (global::blnDebugMode) {printf("Generations finished.\n");}
#else
  //This is the human side of the game,
  //just for debugging stuff
  bool quit = false;
  SDL_Event event;
  char direction;
  char playerStatus;

  global::mymap.load();
  global::mymap.restart();
  while (!quit) {
    Screen.update();
    global::enty.getFitness();
    if (SDL_PollEvent( &event )) {
      if (event.type == SDL_QUIT) {quit = true;}
      else if (event.type == SDL_KEYDOWN) {
        //Key has been pressed figure out what to do
        switch (event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
          direction = dirUp;
          break;
        case SDLK_DOWN:
        case SDLK_s:
          direction = dirDown;
          break;
        case SDLK_RIGHT:
        case SDLK_d:
          direction = dirRight;
          break;
        case SDLK_LEFT:
        case SDLK_a:
          direction = dirLeft;
          break;
        case SDLK_q:
        case SDLK_ESCAPE:
          quit = true;
          break;
        case SDLK_r:
          global::mymap.restart();
          break;
        } //end switch
      } else if (event.type == SDL_KEYUP) {
        direction = dirNone;
      } //end if event
    } //end if event
    playerStatus = global::mymap.move(direction);
    if (playerStatus != statusLiving) {
      Screen.playerDeath();
      global::mymap.restart();
    }
  } //end while not quit
#endif
  Screen.~clsScreen();
	printf("\nDone\n");
	return 0;
} //end main
/*****************************************************************************/
