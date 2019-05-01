#ifndef __CORE_HEADER__
#define __CORE_HEADER__
/*****************************************************************************/
#include "../version.h"
#include "config.h"
#include "tick.h"
#include "../game/map.h"
#include "../game/entity.h"
#include "../globals.h"
#include "../ui/screen.h"
#include "../ui/menu.h"
#include "../editor/editor.h"
/*****************************************************************************/
class clsCore {
  public:
    clsCore();
    ~clsCore();

    int start();

  private:
    SDL_Thread *exit_thread; /**< Pointer to the thread in memory. */
    clsScreen m_screen;
    clsMenu m_menu;
    clsEditor m_editor;

    static int exit_check(void *);

    void doGame(void);
    void doFirstGeneration(void);
    void doGrowthGeneration(void);
    void doSteadyGernation(void);
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class Core core.h "src/core/core.h"
/// @brief This class holds the core of the program. Almost all of the code
///        is run through this class instead of run in main.cpp.
/////////////////////////////////////////////////
/*****************************************************************************/
#endif // __CORE_HEADER__
