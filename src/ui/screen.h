#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/*****************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <string>
#include <cstdio>
/*****************************************************************************/
#include "../globals.h"
/*****************************************************************************/
/** @def DEFINED_DEFAULT_IMAGE_PATH
  All images are stored in a folder called "Images" however, because Linux,
  and Windows handle file paths differently this macro will be set based on
  the OS. */
/** @def DEFINED_MESSAGE_FONT
  Much like DEFINED_DEFAULT_IMAGE_PATH we have to set this differently depending
  on if the program is being run on a Windows computer or Linux. I dislike Apple
  so we don't care about that. */

#if defined(_WIN32) || defined(_WIN64)
  #define DEFINED_DEFAULT_IMAGE_PATH ".\\Images\\"
  #define DEFINED_MESSAGE_FONT "C:\\Windows\\Fonts\\GARA.ttf"
#elif defined(__unix__) || defined(__linux__)
  #define DEFINED_DEFAULT_IMAGE_PATH "./Images/"
  #define DEFINED_MESSAGE_FONT "/usr/share/fonts/truetype/freefont/FreeMono.ttf"
#elif defined(__CYGWIN__)
  #define DEFINED_DEFAULT_IMAGE_PATH "./Images/"
  #define DEFINED_MESSAGE_FONT "C:/Windows/Fonts/GARA.ttf"
#else
  #define DEFINED_DEFAULT_IMAGE_PATH "OS NOT SUPPORTED!"
  #define DEFINED_MESSAGE_FONT "OS NOT SUPPORTED!"
#endif // defined OS
/*****************************************************************************/
/** @struct stcLoaded
  Holds booleans for if SDL stuff is loaded or not. */

struct stcLoaded {
  bool blnWindow;     /**< A boolean for the window. */
  bool blnRenderer;   /**< A boolean for the renderer. */
  bool blnMapTiles;   /**< A boolean for the map tiles. */
  bool blnErrortex;   /**< A boolean for the error texture. */
  bool blnMessage;    /**< A boolean for the message texture. */
  bool blnMessageFont;/**< A boolean for the Message font. */
  bool blnTools;      /**< A boolean for the tools. */
  bool blnToolFrame;  /**< A boolean for the toolframe. */
};

/** @struct stcColors
  SDL colors for the messages. */
struct stcColors {
  SDL_Color Black; /**< The color black. */
  SDL_Color White; /**< The color white. */
};

/** @struct stcTextures
  All the textures loaded; held in a structure for easier reference */
struct stcTextures {
  SDL_Texture *maptiles; /**< Pointer to "tiles.png" in memory. */
  SDL_Texture *errortex; /**< Pointer to the error texture (its embedded) in memory. */
  SDL_Texture *tooltex; /**< Pointer to the tools texture in memory */
  SDL_Texture *toolframetex; /** Pointer to the tool frame texture */
  SDL_Texture *texmessage; /**< Pointer to the texture for the messages in memory. */
};

/// @addtogroup TypeDefs
/// @{
typedef struct stcLoaded Loaded;
typedef struct stcColors clrs;
typedef struct stcTextures TEX;
/// @}

/** @struct stcWinAtt
    Holds memory references to various window attritues so it can be passed
    between functions. */
struct stcWinAtt {
  SDL_Window *win;    /**< Pointer to the window in memory. */
  SDL_Renderer *ren;  /**< Pointer to the renderer in memory. */
  uint width;         /**< Screen width in pixels. */
  uint height;        /**< Screen height in pixels. */
  TTF_Font *font;     /**< Screen height in pixels. */
  uint pic_size;      /**< Picture size in pixels */
  clrs colors;        /**< Colors. */
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsScreen screen.h "src/ui/screen.h"
/// @brief This class will hold all of the values and functions related to the
///        the SDL screen..
/////////////////////////////////////////////////

class clsScreen {
  public:
    clsScreen();
    ~clsScreen();

    void start(void);
    void update(void);
    void cleanup(void);
    void playerDeath(void);
    TEX* get_Textures(void);
    stcWinAtt* getWinAtt(void);
    SDL_Rect getMapClips(uchar);
    SDL_Rect getToolClips(uchar);
    void showErrors(void);

    static bool bln_SDL_started; /**< If SDL is currently running without issue. */

  private:
    static stcWinAtt window;
    static TEX textures; /**< All the textures. */
    static Loaded blnloaded; /**< All the booleans. */
    static SDL_Rect map_clips[defined::kNumMapTiles]; /**< SDL Rectangles for the location of each of the tiles in tiles.png */
    static SDL_Rect tool_clips[defined::kNumTools]; /**< SDL Rectangles for the location of each of the tiles in image_tools.xpm */

    void loadTextures(void);
    void writemessage(void);
    void set_clips(void);
    void error(void);
};
/*****************************************************************************/
#endif // __MY_SCREEN_HEADER__
