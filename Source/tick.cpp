/**********************************************************************************************************************************************/
#include "tick.h"
#include "globals.h"
/**********************************************************************************************************************************************/
//This will hold functions related to time.
/**********************************************************************************************************************************************/
//Default Constructor
clsTick::clsTick() {
	if (Global::blnDebugMode) {printf("Tick Constructor called.\n");}
	uchrFPS = DEFINED_GOAL_FPS;

	/*
	Figure out how long the wait should be based on the our goal frames per second.

	The Sleep time will be in milliseconds and is how long each "frame" will appear on the
	the screen before the next the one is done.

	I did it this way so that if show map is on it doesn't update it as quickly as it can
	leading to the console being really hard to follow.
	The sleep allows it to look much better.

	The frames per second is also not going to be right because it doesn't account for
	all the other functions of the program and there is also the rounding. But
	it should be close enough to not matter.
	*/

	ulngSleepTime = (ulong) round(1000.0 / uchrFPS);
	if (Global::blnDebugMode) {printf("ulngSleepTime = %lu\n", ulngSleepTime);}
	if(clsTick::uClock == 0) {clsTick::resetClock();}
}
/**********************************************************************************************************************************************/
clsTick::~clsTick() {
	if(Global::blnDebugMode) {printf("Tick Destructor called.\n");}
}
/**********************************************************************************************************************************************/
void clsTick::wait(void) {

	/*Since sleep is usually an OS specific command I made this functions
	To work as a "sleep" but it doesn't function as a true sleep because the
	CPU is still being used, but whatever. It "stops" the program for a bit
	which was its point so its gonna stay until I find something better.*/


	/* Code taken from http://c-for-dummies.com/blog/?p=69 */

	ulong pause;
	clock_t now, then;

	pause = ulngSleepTime * (CLOCKS_PER_SEC/1000);

	now = then = clock();
	if (Global::blnDebugMode) {printf("Wait started for %lu milliseconds.\n", ulngSleepTime);}


    /* TODO (Patrick.Rye#9#): Remove if statement when I figure out issue with time being too long */
	if (ulngSleepTime < 1500) {
		while ((now - then) < pause) {now = clock();}
	}
	if (Global::blnDebugMode) {printf("Wait ended.\n");}
}
/**********************************************************************************************************************************************/
void clsTick::resetClock(void) {
	uClock = DEFINED_TICK_LIMIT;
}
/**********************************************************************************************************************************************/
void clsTick::decClock(void) {
	if (uClock != 0) {uClock--;}
}
/**********************************************************************************************************************************************/
uint clsTick::getClockTime(void) {
	return uClock;
}
/**********************************************************************************************************************************************/
