#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/**********************************************************************************************************************************************************************/
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_TTF.h>
#include <string>
#include <cstdio>
/**********************************************************************************************************************************************/
#include "main.h"
/**********************************************************************************************************************************************************************/
#if defined(_WIN32) ||defined(_WIN64)
    #define DEFINED_DEFAULT_IMAGE_PATH ".\\Images\\"
#elif defined(__unix__) || defined(__CYGWIN__) || defined(__linux__)
    #define DEFINED_DEFAULT_IMAGE_PATH "./Images/"
#else
    #define DEFINED_DEFAULT_IMAGE_PATH "OS NOT SUPPORTED!"
#endif // defined OS
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
        SDL_Texture *player;
        SDL_Texture *monster;
        SDL_Texture *wall;
        SDL_Texture *sky;
        SDL_Texture *pole;
        SDL_Texture *coin;
        SDL_Texture *errortex;

        SDL_Window *win;
        SDL_Renderer *ren;

        uint width;
        uint height;
        //All the pictures should be the same size,
        //define that size here.
        const uint pic_size = 24;

        //Keeps track of which parts have been loaded
        //so when ending only the ones that are open
        //are closed
        bool blnWindow;
        bool blnRenderer;
        bool blnSky;
        bool blnPlayer;
        bool blnMonster;
        bool blnWall;
        bool blnPole;
        bool blnCoin;
        bool blnErrortex;

        SDL_Texture* loadIMG(std::string);
        void error(void);
};
/**********************************************************************************************************************************************************************/
#endif // __MY_SCREEN_HEADER__
