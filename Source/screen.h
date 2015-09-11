#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/**********************************************************************************************************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdio>
/**********************************************************************************************************************************************/
#include "main.h"
/**********************************************************************************************************************************************************************/
#if defined(_WIN32) ||defined(_WIN64)
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
/**********************************************************************************************************************************************************************/
struct stcLoaded { //Holds bools for if stuff is loaded or not
    bool blnWindow;
    bool blnRenderer;
    bool blnMapTiles;
    bool blnErrortex;
    bool blnMessage;
    bool blnMessageFont;
};

struct stcColors {
    SDL_Color Black;
    SDL_Color White;
};

struct stcTextures {
    SDL_Texture *maptiles;
    SDL_Texture *errortex;
    SDL_Texture *texmessage;
};

typedef struct stcLoaded Loaded;
typedef struct stcColors clrs;
typedef struct stcTextures TEX;
/**********************************************************************************************************************************************************************/
class clsScreen {
    public:
        /** Default constructor */
        clsScreen(void);
        /** Default destructor */
        ~clsScreen();

        void update(void);
        void cleanup(void);
        void playerDeath(void);

        bool bln_SDL_started;


    private:
        TEX textures;

        SDL_Window *win;
        SDL_Renderer *ren;

        uint width;
        uint height;

        //All the pictures should be the same size,
        //define that size here.
        const uint pic_size = DEFINED_PIC_SIZE;

        //Keeps track of which parts have been loaded
        //so when ending only the ones that are open
        //are closed
        Loaded blnloaded;

        void loadTextures(void);
        SDL_Rect clips[DEFINED_NUM_MAP_TILES];

        void set_clips(void);
        void error(void);

        clrs colors;

        TTF_Font *MessageFont;
        void writemessage(void);
        //SDL_Rect detectEdge(SDL_Rect, SDL_Rect);
};
/**********************************************************************************************************************************************************************/
#endif // __MY_SCREEN_HEADER__
