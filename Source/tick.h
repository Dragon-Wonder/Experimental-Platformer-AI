#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/*****************************************************************************/
#include <time.h>
#include <math.h>
/*****************************************************************************/
#include "globals.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsTick tick.h "source/tick.h"
/// @brief This class will hold all of the values and functions related to
///        time.
/////////////////////////////////////////////////

class clsTick {
  public:
    clsTick();

    void wait(void); //Call this to activate the wait
    void resetClock(void);
    void startTimer(void);
    void decClock(void); //This is so the clock will decrement even when show map is false.
    uint getClockTime(void);

  private:
    uchar uchrFPS; /**< The Frame per second we want to see. */
    ulong ulngSleepTime; /**< How long (in milliseconds) the program should wait for. */
    uint uClock; /**< The time the player has left to complete the map (in seconds). */
    float fClock; /**< The clock as a floating point, will be converted to uint. */
    clock_t timerStart; /**< The start of the timer since the last pause. */
    clock_t clockStart; /**< The start of the clock. */
};
/*****************************************************************************/
#endif
