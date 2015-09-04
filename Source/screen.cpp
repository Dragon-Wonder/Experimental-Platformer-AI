/**********************************************************************************************************************************************************************/
#include "screen.h"
#include "map.h"
#include "config.h"
#include "globals.h"
#include "image_error.xpm"
/**********************************************************************************************************************************************************************/
/* TODO (GamerMan7799#5#): Get better images for the game. (Currently just using placeholders)
                           Consider hiring someone? */
/**********************************************************************************************************************************************************************/
clsScreen::clsScreen() {
    if (Global::Cnfg.getvalues(cnfgShowMap) == 1) { //if not showing the map don't bother trying to load any of the images
                                                    //useful so if show map is disabled you don't need the images folder.
        //Figure out screen size
        if (Global::Cnfg.getvalues(cnfgScreenWidth) == 0) {width = 35*pic_size;}
        else {width = Global::Cnfg.getvalues(cnfgScreenWidth);}
        if (Global::Cnfg.getvalues(cnfgScreenHeight) == 0) {height = DEFINED_MAP_HEIGHT*pic_size;}
        else {height = Global::Cnfg.getvalues(cnfgScreenHeight);}

        //Set all the booleans to false
        blnWindow = false;
        blnRenderer = false;
        blnSky = blnPlayer = blnMonster = blnWall = blnPole = blnCoin = blnErrortex = false;
        bln_SDL_started = false;

        //Start SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            error();
            return;
        } else {
            bln_SDL_started = true;
            if (Global::blnDebugMode) {printf("SDL init successful\n");}
        }

        //Start TTF
        if (TTF_Init() != 0) {
            error();
            return;
        } else {
            if (Global::blnDebugMode) {printf("TTF init successful\n");}
        }

        //Start Image (with only png)
        if (!(IMG_Init( IMG_INIT_PNG )) & IMG_INIT_PNG) {
            error();
            return;
        } else {
            if (Global::blnDebugMode) {printf("IMG init successful\n");}
        }

        win = SDL_CreateWindow("Experimental Platformer AI",100, 100, width, height, SDL_WINDOW_SHOWN);
        if (win == nullptr) {
            printf("SDL Failed to create window.\n");
            error();
            return;
        } else {
            blnWindow = true;
            if (Global::blnDebugMode) {printf("Window creation successful\n");}
        }

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (ren == nullptr) {
            printf("SDL Failed to create renderer.\n");
            error();
            return;
        } else {
            blnRenderer = true;
            if (Global::blnDebugMode) {printf("Renderer creation successful\n");}
        }



        errortex = loadERROR();
        if (bln_SDL_started == false) {return;}
        else {
            blnErrortex = true;
            if (Global::blnDebugMode) {printf("Error loading successful\n");}
        }

        std::string path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "sky.png";
        sky = loadIMG(path);
        blnSky = true;
        if (Global::blnDebugMode) {printf("Sky loading successful\n");}

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "player.png";
        player = loadIMG(path);
        blnPlayer = true;
        if (Global::blnDebugMode) {printf("Player loading successful\n");}


        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "wall.png";
        wall = loadIMG(path);
        blnWall = true;
        if (Global::blnDebugMode) {printf("Wall loading successful\n");}


        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "coin.png";
        coin = loadIMG(path);
        blnCoin = true;
        if (Global::blnDebugMode) {printf("Coin loading successful\n");}

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "pole.png";
        pole = loadIMG(path);
        blnPole = true;
        if (Global::blnDebugMode) {printf("Pole loading successful\n");}

        path = DEFINED_DEFAULT_IMAGE_PATH;
        path += "monster.png";
        monster = loadIMG(path);
        blnMonster = true;
        if (Global::blnDebugMode) {printf("Monster loading successful\n");}

        MessageFont = TTF_OpenFont(DEFINED_MESSAGE_FONT,16); //Opens font and sets size
        if (MessageFont == nullptr) {
            printf("Font failed to load, messages will not appear.");
            blnMessageFont = false;
        } else {
            if(Global::blnDebugMode) {printf("Message font created\n");}
            blnMessageFont = true;
        }

        Black = {0, 0, 0, 0}; //Make the color black for fonts
        White = {255, 255, 255, 0}; //Make the color white for fonts

        update();
    } //end if blnShowMap
}
/**********************************************************************************************************************************************************************/
clsScreen::~clsScreen() {
    if (Global::Cnfg.getvalues(cnfgShowMap) == 1) { //if nothing was really loaded then don't need to clean anything up
        cleanup();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        if (Global::blnDebugMode) {printf("SDL quit\n");}
    } //end if show map
}
/**********************************************************************************************************************************************************************/
void clsScreen::update(void) {
    PLYR tempPlayer = Global::Enty.getPlayer();
    uint Max_Height, Max_Width; //Values for how far on the map the screen should render
    uint x_start; //place where x starts at
    Max_Height = (uint) (height/pic_size);
    Max_Width = (uint) (width/pic_size);

    //This will cause the screen to move in different segments at a time.
    x_start = (uint) (round (tempPlayer.location.x / Max_Width) ) * Max_Width;

    //clear renderer
    SDL_RenderClear(ren);

    //copy sky to cover entire screen.
    SDL_RenderCopy(ren,sky,NULL,NULL);

    SDL_Rect dst;

    //Start updating texture placements
    for (uint y = 0; (y < (Max_Height)) && (y < DEFINED_MAP_HEIGHT); y++) {
        for (uint x = x_start; (x < (x_start + Max_Width)) && (x < DEFINED_MAP_WIDTH); x++) {
            //update where we're trying to put the texture.
            dst.x = (x - x_start) * pic_size;
            dst.y = y * pic_size;
            //Query a texture to get its width and height
            //Since all the textures are the same we'll use the error one since the program would
            //not have gotten this far if that failed load.
            SDL_QueryTexture(errortex,NULL,NULL, &dst.w, &dst.h);

            switch(Global::Map.getMapCell(x,y)) {
            case tileSpace:
                //SDL_RenderCopy(ren,sky,NULL, &dst);
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

    //Write messages only if Message font is loaded.
    if (blnMessageFont) {
        writemessage();
    }
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

    if (blnMessageFont) {
        TTF_CloseFont(MessageFont);
        blnMessageFont = false;
        if (Global::blnDebugMode) {printf("Message font destroyed\n");}
    }

    if (blnMessage) {
        SDL_DestroyTexture(texmessage);
        blnMessage = false;
        if (Global::blnDebugMode) {printf("Message texture destroyed\n");}
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
    cleanup();
    Global::blnError = true;
    printf("SDL error: %s\n", SDL_GetError());
    printf("TTF error: %s\n", TTF_GetError());
    printf("IMG error: %s\n", IMG_GetError());
    bln_SDL_started = false;
	getchar();
}
/**********************************************************************************************************************************************************************/
SDL_Texture* clsScreen::loadIMG(std::string filename) {
    SDL_Surface* temp = IMG_Load(filename.c_str());

	if (temp == nullptr) {
        printf("Failed to load img.\n");
        return errortex;
	} else {
	    if (Global::blnDebugMode) {printf("img to surface successful\n");}
    }

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,temp);
	SDL_FreeSurface(temp);
	if (tex == nullptr) {
        printf("Failed to create texture.\n");
        tex = errortex;
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

    /* TODO (GamerMan7799#1#): Add tileDeadPlayer with its own image to better till when the death animation is happening */
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
void clsScreen::writemessage(void) {
    //Now work on Making the messages that will appear on the screen
    //Will require set up of Library SDL_TTF
    //Which I don't want to do right now so comment it all out.

    //I imagine that I am doing terrible programing things
    //With how I'm switch between char's and strings but whatever

    //These are char arrays that will act as "Strings" for building the messages to appear
    char strClock[8];
    char strGenNum[4];
    char strPlayerNum[3];
    char strFitness[6];

    /* TODO (GamerMan7799#5#): Somehow detect if over wall and change color of text */
    std::string message;
    sprintf(strClock, "%8u", Global::Tick.getClockTime());

    SDL_Surface* surmessage = TTF_RenderText_Solid(MessageFont, strClock, Black);
    if (surmessage == nullptr) {
        printf("Failed to make clock surface.\n");
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Surface Message successfully created\n");}
    }

    texmessage = SDL_CreateTextureFromSurface(ren, surmessage);
    if (texmessage == nullptr) {
        printf("Failed to convert message surface to texture.\n");
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Surface texture successfully created\n");}
        blnMessage = true;
    }

    SDL_Rect dst;
    SDL_QueryTexture(texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.y = 0;
    dst.x = width - dst.w;
    SDL_RenderCopy(ren, texmessage, NULL, &dst);
    if (Global::blnDebugMode) {printf("Clock placed.\n");}

    PLYR tempPlayer = Global::Enty.getPlayer();

    //Covert all the values into their char arrays
    int valuesscanned;
    /* FIXME (GamerMan7799#1#): Program crashes if -Os -O3 or -s are enabled in compiler. */
    //Make the status string
    //Also make sure that each values has at least 1 values scanned in to avoid an error
    message = "Generation: ";
    valuesscanned = sprintf(strGenNum, "%2u", Global::Enty.uchrGenNum);
    if (valuesscanned >= 1) {message += strGenNum;}
    message += "     Player: ";
    valuesscanned = sprintf(strPlayerNum, "%3u", Global::Enty.uchrPlayerNum);
    if (valuesscanned >= 1) {message += strPlayerNum;}
    message += "     Fitness: ";
    valuesscanned = sprintf(strFitness, "%3.2f", tempPlayer.fitness);
   if (valuesscanned >= 1) { message += strFitness;}
    if (Global::blnDebugMode) {printf("Message written.\n");}

    surmessage = TTF_RenderText_Solid(MessageFont, message.c_str(),White);
    if (surmessage == nullptr) {
        printf("Failed to make status surface.\n");
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Generation Status Message successfully created.\n");}
    }
    texmessage = SDL_CreateTextureFromSurface(ren, surmessage);
    if (texmessage == nullptr) {
        printf("Failed to convert message surface to texture.\n");
        blnMessage = false;
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Surface texture successfully created\n");}
        blnMessage = true;
    }

    SDL_QueryTexture(texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.x = (int)(width - dst.w);
    dst.y = height - 30;

    SDL_RenderCopy(ren, texmessage, NULL, &dst);
    SDL_FreeSurface(surmessage);
}
/**********************************************************************************************************************************************************************/
SDL_Texture* clsScreen::loadERROR(void) {
    SDL_Surface* temp = IMG_ReadXPMFromArray(image_error_xpm);

	if (temp == nullptr) {
        printf("Failed to load embedded image.\n");
        error();
        return nullptr;
	} else {
	    if (Global::blnDebugMode) {printf("Error surface created.\n");}
    }

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,temp);
	SDL_FreeSurface(temp);
	if (tex == nullptr) {
        printf("Failed to create texture.\n");
        error();
	} else {
	    if (Global::blnDebugMode) {printf("Surface to texture successful\n");}
    }

	return tex;
}
/**********************************************************************************************************************************************************************/
