#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <time.h>
#include <cmath>
/**********************************************************************************************************************************************/
class Tick {
	private: 
		//Members
		uchar fps;
		uint Clock;
		
		//Functions
		void sleep(ulong);
		
	public:
		//Default Constructor
		Tick();
		~Tick();

		//Functions
		void wait(void); //Call this to activate the wait
		void resetClock(void);
		void decClock(void); //This is so the clock will decrement even when show map is false.
		uint getClockTime(void);
	
};
/**********************************************************************************************************************************************/
#endif
