#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <time.h>
#include <cmath>
/**********************************************************************************************************************************************/
class clsTick {
	private: 
		//Members
		uchar uchrFPS;
		uint uClock;
		ulong ulngSleepTime;
		
	public:
		//Default Constructor
		clsTick();
		~clsTick();

		//Functions
		void wait(void); //Call this to activate the wait
		void resetClock(void);
		void decClock(void); //This is so the clock will decrement even when show map is false.
		uint getClockTime(void);
};
/**********************************************************************************************************************************************/
#endif
