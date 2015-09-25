/*****************************************************************************/
#include <cstdio>
#include <cstdlib>
/*****************************************************************************/
#include "tick.h"
#include "globals.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file tick.cpp
/// @brief Holds all of the functions for the Tick Class
/////////////////////////////////////////////////
/*****************************************************************************/
clsTick::clsTick() {
    /////////////////////////////////////////////////
    /// @brief 	Figure out how long the wait should be based on the our goal frames per second.
    ///
	///         The Sleep time will be in milliseconds and is how long each "frame" will appear on the
	///         the screen before the next the one is done.
    ///
	///         I did it this way so that if show map is on it doesn't update it as quickly as it can
	///         leading to the console being really hard to follow.
	///         The sleep allows it to look much better.
    ///
	///         The program will also keep track of how many ticks have passed since the last time it paused;
	///         and it will only pause for however much longer it needs to meet the FPS goal.
    /////////////////////////////////////////////////




	if (Global::blnDebugMode) {printf("Tick Constructor called.\n");}
	uchrFPS = DEFINED_GOAL_FPS;

	ulngSleepTime = (ulong) round(1000.0 / uchrFPS);
	timerStart = clock();
	if (Global::blnDebugMode) {printf("ulngSleepTime = %lu\n", ulngSleepTime);}
	uClock = DEFINED_TICK_LIMIT;
}
/*****************************************************************************/
void clsTick::wait(void) {
    /////////////////////////////////////////////////
    /// @brief 	Since sleep is usually an OS specific command I made this function
	///         to work as a "sleep" but it doesn't function as a true sleep because the
	///         CPU is still being used, but whatever. It "stops" the program for a bit
	///         which was its point so its gonna stay until I find something better.
    /// @return void
    /////////////////////////////////////////////////



	ulong pause;
	clock_t now;

	pause = ulngSleepTime * (CLOCKS_PER_SEC/1000);
	now = clock();

    if ((uint)abs(now - timerStart) < pause) {
        while ( (uint)abs(now - timerStart) < pause) {now = clock();}
    }
	startTimer();
}
/*****************************************************************************/
void clsTick::resetClock(void) {
    /////////////////////////////////////////////////
    /// @brief Sets uClock and fClock back to DEFINED_TICK_LIMIT
    /// @return void
    /////////////////////////////////////////////////

	uClock = DEFINED_TICK_LIMIT;
	fClock = DEFINED_TICK_LIMIT;
	clockStart = clock();
}
/*****************************************************************************/
void clsTick::decClock(void) {
    /////////////////////////////////////////////////
    /// @brief Decrements the clock by however much time has passed.
    /// @return void
    /////////////////////////////////////////////////

    fClock = DEFINED_TICK_LIMIT - (clock() - clockStart) / CLOCKS_PER_SEC;

    uClock = (fClock >= 0.00) ? (uint) roundf(fClock) : 0;
}
/*****************************************************************************/
uint clsTick::getClockTime(void) {
    /////////////////////////////////////////////////
    /// @brief Gets the clock amount.
    /// @return uClock
    /////////////////////////////////////////////////

	return uClock;
}
/*****************************************************************************/
void clsTick::startTimer(void) {
    /////////////////////////////////////////////////
    /// @brief Sets timerStart to now.
    /// @return void
    /////////////////////////////////////////////////

    timerStart = clock();
}
/*****************************************************************************/
