#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include <time.h>
#include <cmath>
/**********************************************************************************************************************************************/
#include "main.h"
/**********************************************************************************************************************************************/
class clsTick {
    public:
	//private:
		//Members
		uchar uchrFPS;
		ulong ulngSleepTime;
		uint uClock;
		clock_t timerStart;

	//public:
		//Default Constructor
		clsTick();
		~clsTick();

		//Functions
		void wait(void); //Call this to activate the wait
		void resetClock(void);
		void startTimer(void);

		void decClock(void); //This is so the clock will decrement even when show map is false.
		uint getClockTime(void);
};
/**********************************************************************************************************************************************/
#endif
