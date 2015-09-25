#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/*****************************************************************************/
#include <time.h>
#include <math.h>
/*****************************************************************************/
#include "main.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsTick tick.h "source/tick.h"
/// @brief This class will hold all of the values and functions related to
///        time.
/////////////////////////////////////////////////

class clsTick {
    public:
		//Default Constructor
		clsTick();

		//Functions
		void wait(void); //Call this to activate the wait
		void resetClock(void);
		void startTimer(void);

		void decClock(void); //This is so the clock will decrement even when show map is false.
		uint getClockTime(void);

    private:
		//Members
		/** The Frame per second we want to see. */
		uchar uchrFPS;

		/** How long (in milliseconds) the program should wait for. */
		ulong ulngSleepTime;

		/** The time the player has left to complete the map (in seconds). */
		uint uClock;

		/** The clock as a floating point, will be converted to uint. */
		float fClock;

		/** The start of the timer since the last pause. */
		clock_t timerStart;

		/** The start of the clock. */
		clock_t clockStart;

};
/*****************************************************************************/
#endif
