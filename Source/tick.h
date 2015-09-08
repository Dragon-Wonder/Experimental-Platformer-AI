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
		//Default Constructor
		clsTick();
		~clsTick();

		//Functions
		void wait(void); //Call this to activate the wait
		void resetClock(void);
		void startTimer(void);

		void decClock(void); //This is so the clock will decrement even when show map is false.
		uint getClockTime(void);

    private:
		//Members
		uchar uchrFPS;
		ulong ulngSleepTime;
		uint uClock;
		float fClock; //The clock as a floating point, will be converted to uint
		clock_t timerStart; //The start of the timer since the last pause
		clock_t clockStart; //the start of the clock

};
/**********************************************************************************************************************************************/
#endif
