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

	The program will also keep track of how many ticks have passed since the last time it paused;
	and it will only pause for however much longer it needs to meet the FPS goal.
	*/

	ulngSleepTime = (ulong) round(1000.0 / uchrFPS);
	timerStart = clock();
	if (Global::blnDebugMode) {printf("ulngSleepTime = %lu\n", ulngSleepTime);}
	uClock = DEFINED_TICK_LIMIT;
}
/**********************************************************************************************************************************************/
clsTick::~clsTick() {
	if(Global::blnDebugMode) {printf("Tick Destructor called.\n");}
}
/**********************************************************************************************************************************************/
void clsTick::wait(void) {

	/*Since sleep is usually an OS specific command I made this function
	To work as a "sleep" but it doesn't function as a true sleep because the
	CPU is still being used, but whatever. It "stops" the program for a bit
	which was its point so its gonna stay until I find something better.*/

	ulong pause;
	clock_t now;

	pause = ulngSleepTime * (CLOCKS_PER_SEC/1000);
	now = clock();

    if ((uint)abs(now - timerStart) < pause) {
        while ( (uint)abs(now - timerStart) < pause) {now = clock();}
    }
	startTimer();
}
/**********************************************************************************************************************************************/
void clsTick::resetClock(void) {
	uClock = DEFINED_TICK_LIMIT;
	fClock = DEFINED_TICK_LIMIT;
	clockStart = clock();
}
/**********************************************************************************************************************************************/
void clsTick::decClock(void) {
    fClock = DEFINED_TICK_LIMIT - (clock() - clockStart) / CLOCKS_PER_SEC;

    uClock = (fClock >= 0.00) ? (uint) roundf(fClock) : 0;
}
/**********************************************************************************************************************************************/
uint clsTick::getClockTime(void) {
	return uClock;
}
/**********************************************************************************************************************************************/
void clsTick::startTimer(void) {
    timerStart = clock();
}
/**********************************************************************************************************************************************/
