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
/** Holds values that is used for the paint brush. */
struct stcPaintBrush {
  uchar CurrentTile;  /**< Holds current tile that is selected. */
  uchar ToolMode;     /**< Holds the tool that is selected. Currently does
                           nothing, but will later have tools like bucket, line, etc... */
};

/** Holds values for the buttons. */
struct stcToolButton {
  SDL_Rect box;       /**< Holds the "box" which is where the button appears on screen. */
  SDL_Rect *clip;     /**< Holds the "clip" which is where the button appears on tile.png. */
  uchar buttontype;   /**< Holds what type of button it is, aka the tile. */
};

/// @addtogroup TypeDefs
/// @{
typedef struct stcPaintBrush BRUSH;
typedef struct stcToolButton BTTN;
/// @}
/*****************************************************************************/
/** Holds the different tools available. Currently only pencil works. */
enum tools {
    toolPencil = 0,
    toolBucket,
    toolLine,
};

/** Holds the different prompts types to the user. */
enum prompttype {
    promptYesNo = 0,
    promptOkay
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsEditor editor.h "src/editor/editor.h"
/// @brief This class will handle the editor
/////////////////////////////////////////////////

class clsEditor {
public:
  clsEditor();
  ~clsEditor();

  void start(void);

private:

  void make_buttons(void);
  void check_events(SDL_Event*);
  void toolbar_draw(void);
  void map_draw(void);
  void update(void);
  void save(void);
  void load(void);
  void newmap(void);
  char promptuser(uchar, std::string)

  stcWinAtt* m_window;
  TEX m_textures;
  SDL_Rect map_clips[defined::kNumMapTiles];
  SDL_Rect tool_clips[defined::kNumTools];
  BURSH paintbrush;
  BTTN tilebuttons[defined::kNumMapTiles];
  uint button_xplaces[deinfed::kNumMapTiles];
  BTTN menubuttons[defined::kNumTools];
};
/*****************************************************************************/
#endif
