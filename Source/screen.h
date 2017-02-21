#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/*****************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdio>
/*****************************************************************************/
#include "main.h"
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
  SDL_Texture *texmessage; /**< Pointer to the texture for the messages in memory. */
};

typedef struct stcLoaded Loaded;
typedef struct stcColors clrs;
typedef struct stcTextures TEX;
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsScreen screen.h "source/screen.h"
/// @brief This class will hold all of the values and functions related to the
///        the SDL screen..
/////////////////////////////////////////////////

class clsScreen {
  public:
    clsScreen();
    ~clsScreen();

    void update(void);
    void cleanup(void);
    void playerDeath(void);

    /** If SDL is currently running without issue. */
    bool bln_SDL_started;

  private:
    /** All the textures. */
    TEX textures;

    /** Pointer to the window in memory. */
    SDL_Window *win;

    /** Pointer to the renderer in memory. */
    SDL_Renderer *ren;

    /** Screen width in pixels. */
    uint width;

    /** Screen height in pixels. */
    uint height;

    /** Picture size in pixels */
    uint pic_size;

    /** All the booleans. */
    Loaded blnloaded;

    /** SDL Rectangles for the location of each of the tiles in tiles.png */
    SDL_Rect clips[defined::kNumMapTiles];

    /** Colors. */
    clrs colors;

    /** Message font. */
    TTF_Font *MessageFont;

    void loadTextures(void);
    void writemessage(void);
    void set_clips(void);
    void error(void);
};
/*****************************************************************************/
#endif // __MY_SCREEN_HEADER__
