#ifndef __GLOBAL__HEADER__
#define __GLOBAL__HEADER__
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file globals.h
/// @brief This file holds all of the references to variables declared in the
///        Global namespace. See Main.cpp for more details
/////////////////////////////////////////////////
/*****************************************************************************/
//Make some prototype classes
class clsConfig;
class clsEntity;
class clsMap;
class clsTick;
/*****************************************************************************/
//Globals
namespace global {
	extern const bool blnDebugMode; //Holds if in debug mode or not. Causes more messages to appear in the console
	extern bool blnError; //if there was an error; then the program will exit when it checks this.

	//Call all of the classes as globals so they are the same everywhere.
	extern clsConfig cnfg;
	extern clsEntity enty;
	extern clsMap mymap;
	extern clsTick tick;
	namespace physics { //Constants that are gonna be used for physics in later versions
    extern const float fGravityAcceleration;
    extern const float fFriction;
    extern const float fMaxVelocity;
    extern const float fMonsMaxVelocity;
    extern const float fIncVelocity; //How much velocity increments by for an input
    extern const float fRecoil; //Recoil for when hitting an object
	};
};
/*****************************************************************************/
#endif
