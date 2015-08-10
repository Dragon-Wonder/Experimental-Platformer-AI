/**********************************************************************************************************************************************/
#include "tick.h"
/**********************************************************************************************************************************************/
//This will hold functions related to time.
/**********************************************************************************************************************************************/
void Tick::wait(void) {
	static ulong ulngSleepTime;
	/*
	Figure out how long the wait should be based on the our goal frames per second.
	
	The Sleep time will be in milliseconds and is how long each "frame" will appear on the
	the screen before the next the one is done.
	
	I did it this way so that if show map is on it doesn't update it as quickly as it can
	leading to the console being really hard to follow.
	The sleep allows it to look much better.
	
	The frames per second is also not going to be right be it doesn't account for
	all the other functions of the program and there is also the rounding. But
	it should be close enough to not matter
	*/
	if (ulngSleepTime == 0) {ulngSleepTime = (ulong) round(1000.0 / fps);}
	
	sleep(ulngSleepTime);
}
/**********************************************************************************************************************************************/
void Tick::sleep(ulong milliseconds) {
	/*Since sleep is usually an OS specific command I made this functions
	To work as a "sleep" but it doesn't function as a true sleep becuase the 
	CPU is still being used, but whatever. It "stops" the program for a bit
	which was its point so its gonna stay until I find something better.*/
	
	
	/* Code taken from http://c-for-dummies.com/blog/?p=69 */
	
	ulong pause;
	
	clock_t now, then;
	
	pause = milliseconds * (CLOCKS_PER_SEC/1000);
	
	now = then = clock();
	
	while ((now - then) < pause) {now = clock();}
}
/**********************************************************************************************************************************************/
//Default Constructor
Tick::Tick() : fps(Goal_FPS) {
}
/**********************************************************************************************************************************************/
