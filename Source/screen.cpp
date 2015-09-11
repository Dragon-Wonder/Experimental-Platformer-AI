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
        if (Global::Cnfg.getvalues(cnfgScreenWidth) == 0) {width = 35 * pic_size;}
        else {width = Global::Cnfg.getvalues(cnfgScreenWidth);}
        if (Global::Cnfg.getvalues(cnfgScreenHeight) == 0) {height = DEFINED_MAP_HEIGHT * pic_size;}
        else {height = Global::Cnfg.getvalues(cnfgScreenHeight);}

        //Set all the booleans to false
        blnloaded.blnWindow = false;
        blnloaded.blnRenderer = false;
        blnloaded.blnMapTiles = blnloaded.blnErrortex = false;
        bln_SDL_started = false;

        set_clips();

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
            blnloaded.blnWindow = true;
            if (Global::blnDebugMode) {printf("Window creation successful\n");}
        }

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (ren == nullptr) {
            printf("SDL Failed to create renderer.\n");
            error();
            return;
        } else {
            blnloaded.blnRenderer = true;
            if (Global::blnDebugMode) {printf("Renderer creation successful\n");}
        }

        loadTextures();
        if (bln_SDL_started == false) {return;}

        MessageFont = TTF_OpenFont(DEFINED_MESSAGE_FONT,16); //Opens font and sets size
        if (MessageFont == nullptr) {
            printf("Font failed to load, messages will not appear.");
            blnloaded.blnMessageFont = false;
        } else {
            if(Global::blnDebugMode) {printf("Message font created\n");}
           blnloaded.blnMessageFont = true;
        }

        colors.Black = {0, 0, 0, 0}; //Make the color black for fonts
        colors.White = {255, 255, 255, 0}; //Make the color white for fonts

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
    //clear renderer
    SDL_RenderClear(ren);
    //copy sky to cover entire screen.
    SDL_RenderCopy(ren,textures.maptiles,&clips[tileSpace],NULL);
    SDL_Rect dst;
    //Set the pic size
    dst.w = dst.h = pic_size;

    BPLYR tempPlayer = Global::Enty.getPlayerBase();
    stcLoc offset; //how much the screen is offset by

    offset.x = (uint) (round( tempPlayer.location.x / width ) * width);
    offset.y = (uint) (round( tempPlayer.location.y / height) * height);

    //Place the map parts where they belong.
    for (uint y = 0; (y < DEFINED_MAP_HEIGHT); y++) {
        for (uint x = 0; (x < DEFINED_MAP_WIDTH); x++) {
            //update where we're trying to put the texture.
            dst.x = (x * pic_size) - (offset.x);
            dst.y = (y * pic_size) - (offset.y);

            //Load the map.
            switch( Global::Map.getMapCell(x,y) ) { //Use this to make sure we aren't try to load a non-existing part
            case tileSpace:
            case tileCoin:
            case tilePole:
            case tileWall:
                SDL_RenderCopy(ren, textures.maptiles, &clips[Global::Map.getMapCell(x,y)], &dst);
                break;
            default:
                //Don't know what this is so display an error texture.
                SDL_RenderCopy(ren, textures.errortex, NULL, &dst);
                break;
            } //end switch
        } //end for x
    } //end for y

    //Now place the player and monsters
    MNSTR tempMonster;
    for (uchar i = 0; i < Global::Map.numMonsters; i++) {
        tempMonster = Global::Enty.getMonster(i);
        if (tempMonster.living) {
            dst.x = (tempMonster.location.x - offset.x);
            dst.y = (tempMonster.location.y - offset.y);
            SDL_RenderCopy(ren, textures.maptiles, &clips[tileMonster], &dst);
        } //end if living
    } // end for monsters
    dst.x = tempPlayer.location.x - offset.x; //Fix this later, doesn't account for screen shifts but I want to work on getting velocity working
    dst.y = tempPlayer.location.y - offset.y;
    SDL_RenderCopy(ren, textures.maptiles, &clips[tilePlayer], &dst);

    //Write messages only if Message font is loaded.
    if (blnloaded.blnMessageFont) {
        writemessage();
    }
    //show renderer
    SDL_RenderPresent(ren);
    Global::Tick.wait();
}
/**********************************************************************************************************************************************************************/
void clsScreen::cleanup(void) {
    if (blnloaded.blnMapTiles) {
        SDL_DestroyTexture(textures.maptiles);
        blnloaded.blnMapTiles = false;
        if (Global::blnDebugMode) {printf("Tiles texture destroyed\n");}
    }

    if (blnloaded.blnErrortex) {
        SDL_DestroyTexture(textures.errortex);
        blnloaded.blnErrortex = false;
        if (Global::blnDebugMode) {printf("Error texture destroyed\n");}
    }

    if (blnloaded.blnMessageFont) {
        TTF_CloseFont(MessageFont);
        blnloaded.blnMessageFont = false;
        if (Global::blnDebugMode) {printf("Message font destroyed\n");}
    }

    if (blnloaded.blnMessage) {
        SDL_DestroyTexture(textures.texmessage);
        blnloaded.blnMessage = false;
        if (Global::blnDebugMode) {printf("Message texture destroyed\n");}
    }

	if (blnloaded.blnRenderer) {
        SDL_DestroyRenderer(ren);
        blnloaded.blnRenderer = false;
        if (Global::blnDebugMode) {printf("Renderer destroyed\n");}
    }
	if (blnloaded.blnWindow) {
        SDL_DestroyWindow(win);
        blnloaded.blnWindow = false;
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
void clsScreen::loadTextures() {
    std::string path = DEFINED_DEFAULT_IMAGE_PATH;
    path += "tiles.png";

    //Load the error texture first.
    SDL_Surface* temp = IMG_ReadXPMFromArray(image_error_xpm);
    if (temp == nullptr) {
        printf("Failed to load embedded image.\n");
        error();
        return;
	} else {
	    if (Global::blnDebugMode) {printf("Error surface created.\n");}
    }

    textures.errortex = SDL_CreateTextureFromSurface(ren,temp);
	if (textures.errortex == nullptr) {
        printf("Failed to create texture.\n");
        error();
	} else {
	    if (Global::blnDebugMode) {printf("Surface to texture successful\n");}
	    blnloaded.blnErrortex = true;
    }

    //Now load the tiles
    temp = IMG_Load(path.c_str());
	if (temp == nullptr) {
        //File doesn't exist, replace the clips to be all 0,0
        //and set it to use the error texture instead.
        printf("Could not find tiles.png!\n");
        textures.maptiles = textures.errortex;
        for (uchar i = 0; i < DEFINED_NUM_MAP_TILES; i++) {
            clips[i].x = clips[i].y = 0;
        }
        blnloaded.blnMapTiles = true;
        return;
	} else {
	    if (Global::blnDebugMode) {printf("Tiles.png load successful\n");}
    }

	textures.maptiles = SDL_CreateTextureFromSurface(ren,temp);
	SDL_FreeSurface(temp);
	if (textures.maptiles == nullptr) {
        //Cannot make texture; replace the clips to be all 0,0
        //and set it to use the error texture instead.
        printf("Tiles could not be converted to texture.\n");
        textures.maptiles = textures.errortex;
        for (uchar i = 0; i < DEFINED_NUM_MAP_TILES; i++) {
            clips[i].x = clips[i].y = 0;
        }
        blnloaded.blnMapTiles = true;
        return;
	} else {
	    if (Global::blnDebugMode) {printf("Tiles converted to texture successful\n");}
	    blnloaded.blnMapTiles = true;
    }
}
/**********************************************************************************************************************************************************************/
void clsScreen::playerDeath(void) {
	//Plays short death animation
	//only shows up if the Map::move returns DEAD
	//The games "pauses" for a second then the player will move up
	//3 spaces then down about 4 spaces (depending on starting point)
	//the whole thing happens in 5 frames.

    /* TODO (GamerMan7799#1#): Add tileDeadPlayer with its own image to better tell when the death animation is happening */
	BPLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayerBase();
	//show();
	for (uchar i = 0; i < DEFINED_GOAL_FPS; i++) {Global::Tick.wait();} //wait for a second.
	for (uchar i = 0; i < 5; i++) {
		if (i < 3) { tempPlayer.vel.y = -1 * Global::Physics::fMaxVelocity; }
        else { tempPlayer.vel.y = 0; }

        tempPlayer.location.x += tempPlayer.vel.x * (1 / DEFINED_GOAL_FPS);
        tempPlayer.location.y += tempPlayer.vel.y * (1 / DEFINED_GOAL_FPS);

        Global::Enty.setPlayer(tempPlayer);
		update();
	}
}
/**********************************************************************************************************************************************************************/
void clsScreen::writemessage(void) {
    //Now work on Making the messages that will appear on the screen

    //I imagine that I am doing terrible programing things
    //With how I'm switch between char's and strings but whatever

    //These are char arrays that will act as "Strings" for building the messages to appear
    char strClock[8];
    char strGenNum[5];
    char strPlayerNum[4];
    char strFitness[7];

    /* TODO (GamerMan7799#5#): Somehow detect if over wall and change color of text */
    std::string message;
    sprintf(strClock, "%8u", Global::Tick.getClockTime());

    SDL_Surface* surmessage = TTF_RenderText_Solid(MessageFont, strClock, colors.Black);
    if (surmessage == nullptr) {
        printf("Failed to make clock surface.\n");
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Surface Message successfully created\n");}
    }

    textures.texmessage = SDL_CreateTextureFromSurface(ren, surmessage);
    if (textures.texmessage == nullptr) {
        printf("Failed to convert message surface to texture.\n");
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Surface texture successfully created\n");}
        blnloaded.blnMessage = true;
    }

    SDL_Rect dst;
    SDL_QueryTexture(textures.texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.y = 0;
    dst.x = width - dst.w;
    SDL_RenderCopy(ren, textures.texmessage, NULL, &dst);
    if (Global::blnDebugMode) {printf("Clock placed.\n");}

    PLYR tempPlayer = Global::Enty.getPlayer();

    //Covert all the values into their char arrays
    int valuesscanned;

    //Make the status string
    //Also make sure that each values has at least 1 values scanned in to avoid an error
    message = "Generation: ";
    valuesscanned = sprintf(strGenNum, "%2u", Global::Enty.uchrGenNum);
    if (valuesscanned >= 1) {
            strGenNum[4] = '\0'; //Make sure that the char is null terminated or the program crashes.
            message += std::string(strGenNum);
    }
    message += "     Player: ";
    valuesscanned = sprintf(strPlayerNum, "%3u", Global::Enty.uchrPlayerNum);
    if (valuesscanned >= 1) {
            strPlayerNum[3] = '\0'; //Make sure that the char is null terminated or the program crashes.
            message += std::string(strPlayerNum);
    }
    message += "     Fitness: ";
    valuesscanned = sprintf(strFitness, "%3.2f", tempPlayer.fitness);
    if (valuesscanned >= 1) {
            strFitness[6] = '\0'; //Make sure that the char is null terminated or the program crashes.
            message += std::string(strFitness);
    }
    if (Global::blnDebugMode) {printf("Status message made.\n");}

    surmessage = TTF_RenderText_Solid(MessageFont, message.c_str(),colors.White);
    if (surmessage == nullptr) {
        printf("Failed to make status surface.\n");
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Generation Status Message successfully created.\n");}
    }
    textures.texmessage = SDL_CreateTextureFromSurface(ren, surmessage);
    if (textures.texmessage == nullptr) {
        printf("Failed to convert message surface to texture.\n");
        blnloaded.blnMessage = false;
        error();
        return;
    } else {
        if (Global::blnDebugMode) {printf("Surface texture successfully created\n");}
        blnloaded.blnMessage = true;
    }

    SDL_QueryTexture(textures.texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.x = (int)(width - dst.w);
    dst.y = height - 30;

    SDL_RenderCopy(ren, textures.texmessage, NULL, &dst);
    SDL_FreeSurface(surmessage);
}
/**********************************************************************************************************************************************************************/
void clsScreen::set_clips() {
    //Set all the locations of the specific tiles in the tiles.png
     //Since all of the sizes are the same we will do this all together to save space
     for (uchar i = 0; i < DEFINED_NUM_MAP_TILES; i++) {
        clips[i].w = clips[i].h = pic_size;
     }

    /*     The Picture Coordinates (x,y)
     *     we multiply this by the pic size to get the clip
     *     +-----+-----+-----+
     *     |(0,0)|(1,0)|(2,0)|
     *     +-----+-----+-----+
     *     |(0,1)|(1,1)|(2,1)|
     *     +-----+-----+-----+
     */

     //First Row (Space, Wall, Player)
     clips[tileSpace].x = 0 * pic_size;
     clips[tileSpace].y = 0 * pic_size;

     clips[tileWall].x = 1 * pic_size;
     clips[tileWall].y = 0 * pic_size;

     clips[tilePlayer].x = 2 * pic_size;
     clips[tilePlayer].y = 0 * pic_size;

     //Second Row (Pole, Monster, Coin)
     clips[tilePole].x = 0 * pic_size;
     clips[tilePole].y = 1 * pic_size;

     clips[tileMonster].x = 1 * pic_size;
     clips[tileMonster].y = 1 * pic_size;

     clips[tileCoin].x = 2 * pic_size;
     clips[tileCoin].y = 1 * pic_size;

}
/**********************************************************************************************************************************************************************/
