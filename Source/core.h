#ifndef __CORE_HEADER__
#define __CORE_HEADER__
/*****************************************************************************/
#include "version.h"
#include "config.h"
#include "tick.h"
#include "map.h"
#include "entity.h"
#include "globals.h"
#include "screen.h"
/*****************************************************************************/
class clsCore {
  public:
    clsCore();
    ~clsCore();

    int start();

  private:
    SDL_Thread *exit_thread; /**< Pointer to the thread in memory. */
    clsScreen Screen;

    static int exit_check(void *);
    void doFirstGeneration(void);
    void doGrowthGeneration(void);
    void doSteadyGernation(void);
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class Core core.h "src/core.h"
/// @brief This class holds the core of the program. Almost all of the code
///        is run through this class instead of run in main.cpp.
/////////////////////////////////////////////////
/*****************************************************************************/
#endif // __CORE_HEADER__
