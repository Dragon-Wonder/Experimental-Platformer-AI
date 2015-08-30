/**********************************************************************************************************************************************************************/
#include "screen.h"
#include "map.h"
#include "config.h"
#include "globals.h"
/**********************************************************************************************************************************************************************/
/* TODO (GamerMan7799#5#): Get better images for the game. (Currently just using placeholders)
                           Consider hiring someone? */
/**********************************************************************************************************************************************************************/
clsScreen::clsScreen() {
    //width = Global::Config.values.uintScreenWidth;
    //height = Global::Config.values.uintScreenHeight;
    if (Global::Cnfg.getvalues(cnfgShowMap) == 1) { //if not showing the map don't bother trying to load any of the images
                                                    //useful so if show map is disabled you don't need the images folder.
        if (Global::Cnfg.getvalues(cnfgScreenWidth) == 0) {width = 30*pic_size;}
        else {width = Global::Cnfg.getvalues(cnfgScreenWidth);}
        if (Global::Cnfg.getvalues(cnfgScreenHeight) == 0) {height = 14*pic_size;}
        else {height = Global::Cnfg.getvalues(cnfgScreenHeight);}

        blnWindow = false;
        blnRenderer = false;
        blnSky = blnPlayer = blnMonster = blnWall = blnPole = blnCoin = blnErrortex = false;
        bln_SDL_started = false;

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            bln_SDL_started = false;
            error();
            return;
        } else {
            bln_SDL_started = true;
            if (Global::blnDebugMode) {printf("SDL init successful\n");}
        }

        win = SDL_CreateWindow("Experimental Platformer AI",100, 100, width, height, SDL_WINDOW_SHOWN);
        if (win == nullptr) {
            printf("SDL Failed to create window.\n");
            cleanup();
            error();
            bln_SDL_started = false;
            return;
        } else {
            blnWindow = true;
            if (Global::blnDebugMode) {printf("Window creation successful\n");}
        }

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (ren == nullptr) {
            printf("SDL Failed to create renderer.\n");
            cleanup();
            error();
            bln_SDL_started = false;
            return;
        } else {
            blnRenderer = true;
            if (Global::blnDebugMode) {printf("Renderer creation successful\n");}
        }

        std::string path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "sky.bmp";

        sky = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnSky = true;
            if (Global::blnDebugMode) {printf("Sky loading successful\n");}
        }

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "player.bmp";

        player = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnPlayer = true;
            if (Global::blnDebugMode) {printf("Player loading successful\n");}
        }

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "wall.bmp";

        wall = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnWall = true;
            if (Global::blnDebugMode) {printf("Wall loading successful\n");}
        }

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "coin.bmp";

        coin = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnCoin = true;
            if (Global::blnDebugMode) {printf("Coin loading successful\n");}
        }

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "pole.bmp";

        pole = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnPole = true;
            if (Global::blnDebugMode) {printf("Pole loading successful\n");}
        }

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "monster.bmp";

        monster = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnMonster = true;
            if (Global::blnDebugMode) {printf("Monster loading successful\n");}
        }

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "error.bmp";

        errortex = loadIMG(path);
        if (bln_SDL_started == false) {return;}
        else {
            blnErrortex = true;
            if (Global::blnDebugMode) {printf("Error loading successful\n");}
        }

        update();
    } //end if blnShowMap
}
/**********************************************************************************************************************************************************************/
clsScreen::~clsScreen() {
    if (Global::Cnfg.getvalues(cnfgShowMap) == 1) { //if nothing was really loaded then don't need to clean anything up
        cleanup();
        SDL_Quit();
        if (Global::blnDebugMode) {printf("SDL quit\n");}
    } //end if show map
}
/**********************************************************************************************************************************************************************/
void clsScreen::update(void) {
    PLYR tempPlayer = Global::Enty.getPlayer();

    /* TODO (GamerMan7799#4#): Add the Clock to the screen */

    uint Max_Height, Max_Width; //Values for how far on the map the screen should render
    Max_Height = (uint) (height/pic_size);
    Max_Width = (uint) (width/pic_size);

    //clear renderer
    SDL_RenderClear(ren);

    //copy sky to cover entire screen.
    SDL_RenderCopy(ren,sky,NULL,NULL);

    SDL_Rect dst;

    //Start updating texture placements
    for (uint y = (tempPlayer.location.y - 2); (y < (tempPlayer.location.y + Max_Height - 2)) && (y < DEFINED_MAP_HEIGHT); y++) {
        for (uint x = (tempPlayer.location.x - 5); (x < (tempPlayer.location.x + Max_Width - 5)) && (x < DEFINED_MAP_WIDTH); x++) {
            //update where we're trying to put the texture.
            dst.x = (x - tempPlayer.location.x + 5) * pic_size;
            dst.y = /*(y - tempPlayer.location.y + 10)*/ y * pic_size;
            //Query a texture to get its width and height
            //Since all textures are the same it doesn't matter which one we use
            SDL_QueryTexture(coin,NULL,NULL, &dst.w, &dst.h);

            switch(Global::Map.getMapCell(x,y)) {
            case tileSpace:
                SDL_RenderCopy(ren,sky,NULL, &dst);
                break;
            case tileCoin:
                SDL_RenderCopy(ren,coin,NULL, &dst);
                break;
            case tileMonster:
                SDL_RenderCopy(ren, monster, NULL, &dst);
                break;
            case tilePlayer:
                SDL_RenderCopy(ren, player, NULL, &dst);
                break;
            case tilePole:
                SDL_RenderCopy(ren, pole, NULL, &dst);
                break;
            case tileWall:
                SDL_RenderCopy(ren, wall, NULL, &dst);
                break;
            default:
                //Don't know what this is so display an error texture.
                SDL_RenderCopy(ren, errortex, NULL, &dst);
                break;
            } //end switch
        } //end for x
    } //end for y

    //show renderer
    SDL_RenderPresent(ren);
    Global::Tick.wait();
}
/**********************************************************************************************************************************************************************/
void clsScreen::cleanup(void) {
    if (blnPlayer) {
        SDL_DestroyTexture(player);
        blnPlayer = false;
        if (Global::blnDebugMode) {printf("Player texture destroyed\n");}
    }
	if (blnSky) {
        SDL_DestroyTexture(sky);
        blnSky = false;
        if (Global::blnDebugMode) {printf("Sky texture destroyed\n");}
    }

    if (blnCoin) {
        SDL_DestroyTexture(coin);
        blnCoin = false;
        if (Global::blnDebugMode) {printf("Coin texture destroyed\n");}
    }

     if (blnWall) {
        SDL_DestroyTexture(wall);
        blnWall = false;
        if (Global::blnDebugMode) {printf("Wall texture destroyed\n");}
    }

    if (blnPole) {
        SDL_DestroyTexture(pole);
        blnPole = false;
        if (Global::blnDebugMode) {printf("Pole texture destroyed\n");}
    }

    if (blnMonster) {
        SDL_DestroyTexture(monster);
        blnMonster = false;
        if (Global::blnDebugMode) {printf("Monster texture destroyed\n");}
    }

    if (blnErrortex) {
        SDL_DestroyTexture(errortex);
        blnErrortex = false;
        if (Global::blnDebugMode) {printf("Error texture destroyed\n");}
    }

	if (blnRenderer) {
        SDL_DestroyRenderer(ren);
        blnRenderer = false;
        if (Global::blnDebugMode) {printf("Renderer destroyed\n");}
    }
	if (blnWindow) {
        SDL_DestroyWindow(win);
        blnWindow = false;
        if (Global::blnDebugMode) {printf("Window destroyed\n");}
    }
}
/**********************************************************************************************************************************************************************/
void clsScreen::error(void) {
    Global::blnError = true;
    printf("SDL error: %s\n", SDL_GetError());
	getchar();
}
/**********************************************************************************************************************************************************************/
SDL_Texture* clsScreen::loadIMG(std::string filename) {
    SDL_Surface* temp = SDL_LoadBMP(filename.c_str());

	if (temp == nullptr) {
        printf("Failed to load img.\n");
        cleanup();
        error();
        bln_SDL_started = false;
        return nullptr;
	} else {
	    if (Global::blnDebugMode) {printf("img to surface successful\n");}
    }

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,temp);
	SDL_FreeSurface(temp);
	if (tex == nullptr) {
        printf("Failed to create texture.\n");
        cleanup();
        error();
        bln_SDL_started = false;
	} else {
	    if (Global::blnDebugMode) {printf("Surface to texture successful\n");}
    }

	return tex;
}
/**********************************************************************************************************************************************************************/
void clsScreen::playerDeath(void) {
	//Plays short death animation
	//only shows up if the Map::move returns DEAD
	//The games "pauses" for a second then the player will move up
	//3 spaces then down about 4 spaces (depending on starting point)
	//the whole thing happens in 5 frames.

	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();
	//show();
	for (uchar i = 0; i < DEFINED_GOAL_FPS; i++) {Global::Tick.wait();} //wait for a second.
	uint tempy = tempPlayer.location.y;
	for (uchar i = 0; i < 5; i++) {
		if (i < 3) {
			if (tempy != DEFINED_MAP_HEIGHT) { tempy--;}
		} else {
			if (tempy < DEFINED_MAP_HEIGHT - 2) {tempy+=2;}
		}
        Global::Map.setMapCell(tempPlayer.location.x, tempPlayer.location.y,tileSpace);
        Global::Map.setMapCell(tempPlayer.location.x, tempy, tilePlayer);

        tempPlayer.location.y = tempy;
		update();
	}
}
/**********************************************************************************************************************************************************************/
