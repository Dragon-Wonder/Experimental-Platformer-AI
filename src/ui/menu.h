#ifndef __MENU__HEADER__
#define __MENU__HEADER__
/*****************************************************************************/
#include "screen.h"
#include "../globals.h"
/*****************************************************************************/
#include <string.h>
/*****************************************************************************/
/// @addtogroup Structs
/// @{
/** @struct stcButton
    This Structure holds values related to the menu's buttons */
struct stcButton {
  char type;          /**< The type of button it is */
  std::string text;   /**< Text on the button */
  SDL_Rect cords;     /**< The SDL Coordinates of the button */
};

/// @addtogroup TypeDefs
/// @{
typedef struct stcButton button;
/// @}
/*****************************************************************************/
class clsMenu {
  public:
    clsMenu();
    ~clsMenu();

    char MainMenu(void);
    void AboutMenu(void);
    void OptionsMenu(void);
    char GameMenu(void);
    char promptUser(char, std::string);
    //stats doLevelUp(stats);

  protected:

  private:
    stcWinAtt* m_window; /**< Pointer to the main screen */
    bool clickcheck(LOC,SDL_Rect);
    //TTF_Font *m_font;
    //SDL_Texture* MakeMessage(std::string, SDL_Color);
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsMenu menu.h "src/ui/menu.h"
/// @brief This class holds all of the different menus.
///        It is done this way as some menus need to be referenced from different
///        files, and this is easier then trying to copy and maintain the functions
///        in several different locations.
/////////////////////////////////////////////////
/*****************************************************************************/
#endif // __MENU__HEADER__
