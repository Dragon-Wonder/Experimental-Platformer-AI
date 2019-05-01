#ifndef __EDITOR__HEADER__
#define __EDITOR__HEADER__
/*****************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstring>
#include <string>
#include <cstdio>
/*****************************************************************************/
#include "../globals.h"
#include "../ui/screen.h"
#include "../ui/menu.h"
#include "../game/map.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsEditor editor.h "src/editor/editor.h"
/// @brief This class will handle the editor
/////////////////////////////////////////////////

class clsEditor {
public:
  clsEditor();
  ~clsEditor();

  void start(*clsScreen);

private:

  void make_buttons(void);
  void check_events(SDL_Event*);
  void draw(void);
  void save(void);
  void load(void);
  void newmap(void);

  clsScreen m_screen;

};
/*****************************************************************************/
#endif
