/*****************************************************************************/
#include "screen.h"
#include "map.h"
#include "config.h"
#include "entity.h"
#include "tick.h"
#include "image_error.xpm"
/*****************************************************************************/
/** \todo (GamerMan7799#5#): Get better images for the game. (Currently just using placeholders)
                           Consider hiring someone? */
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file screen.cpp
/// @brief Holds all of the functions for the Screen Class
/////////////////////////////////////////////////
/*****************************************************************************/
clsScreen::clsScreen() {
  /////////////////////////////////////////////////
  /// @brief The default constructor for the SDL screen
  ///        it will try start SDL, and create and window and a renderer,
  ///        then try to load the textures it will need, if any of these fail
  ///        it will set bln_SDL_Started to false and return void, when main in main.cpp
  ///        checks bln_SDL_Started and ends the entire program will it is false.
  ///        If, however, bln_SDL_Started is true it will continue on with the rest of the program.
  /////////////////////////////////////////////////

  pic_size = defined::kPicSize;

  if (global::cnfg.getvalues(cnfgShowMap) == 1) {
    //if not showing the map don't bother trying to load any of the images
    //useful so if show map is disabled you don't need the images folder.
    //Figure out screen size

    width = (global::cnfg.getvalues(cnfgScreenWidth) == 0) ?
        35 * pic_size : global::cnfg.getvalues(cnfgScreenWidth);
    height = (global::cnfg.getvalues(cnfgScreenHeight) == 0) ?
        defined::kMapHeight * pic_size :
        global::cnfg.getvalues(cnfgScreenHeight);

    //Set all the booleans to false
    blnloaded.blnWindow = blnloaded.blnRenderer = false;
    blnloaded.blnMapTiles = blnloaded.blnErrortex = false;
    bln_SDL_started = false;

    set_clips();

    //Start SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      error();
      return;
    } else {
      bln_SDL_started = true;
      if (global::blnDebugMode) { printf("SDL init successful\n"); }
    }

    //Start TTF
    if (TTF_Init() != 0) {
      error();
      return;
    } else {
      if (global::blnDebugMode) { printf("TTF init successful\n"); }
    }

    //Start Image (with only png)
    if (!(IMG_Init( IMG_INIT_PNG )) & IMG_INIT_PNG) {
      error();
      return;
    } else {
      if (global::blnDebugMode) { printf("IMG init successful\n"); }
    }

    win = SDL_CreateWindow("Experimental Platformer AI",100, 100,
                           width, height, SDL_WINDOW_SHOWN);
    ren = (win == nullptr) ? nullptr :
        SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                           SDL_RENDERER_PRESENTVSYNC);

    if (ren == nullptr) {
      printf("SDL Failed to create renderer.\n");
      error();
      return;
    } else {
      blnloaded.blnRenderer = true;
      if (global::blnDebugMode) { printf("Renderer creation successful\n"); }
    }

    loadTextures();
    if ( !bln_SDL_started ) { return; }

    MessageFont = TTF_OpenFont(DEFINED_MESSAGE_FONT,16); //Opens font and sets size
    if (MessageFont == nullptr) {
      printf("Font failed to load, messages will not appear.");
      blnloaded.blnMessageFont = false;
    } else {
      if(global::blnDebugMode) { printf("Message font created\n"); }
      blnloaded.blnMessageFont = true;
    }

    colors.Black = {0, 0, 0, 0}; //Make the color black for fonts
    colors.White = {255, 255, 255, 0}; //Make the color white for fonts

    update();
  } //end if blnShowMap
}
/*****************************************************************************/
clsScreen::~clsScreen() {
  /////////////////////////////////////////////////
  /// @brief This is the default deconstructor, it will just call
  ///        clsScreen::cleanup to ensure everything is cleared from memory,
  ///        and then quit SDL.
  /////////////////////////////////////////////////
  cleanup();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  if (global::blnDebugMode) { printf("SDL quit\n"); }
}
/*****************************************************************************/
void clsScreen::update(void) {
  /////////////////////////////////////////////////
  /// @brief Will update the SDL screen based on the map and player / monster locations.
  /// @return void
  /////////////////////////////////////////////////
  //clear renderer
  SDL_RenderClear(ren);
  //copy sky to cover entire screen.
  SDL_RenderCopy(ren,textures.maptiles,&clips[tileSpace],NULL);
  SDL_Rect dst;
  //Set the pic size
  dst.w = dst.h = pic_size;

  BPLYR tempPlayer = global::enty.getPlayerBase();
  stcLoc offset; //how much the screen is offset by

  offset.x = (uint) (round( tempPlayer.location.x / width ) * width);
  offset.y = (uint) (round( tempPlayer.location.y / height) * height);

  //Place the map parts where they belong.
  for (uint y = 0; (y < defined::kMapHeight); ++y) {
    for (uint x = 0; (x < defined::kMapWidth); ++x) {
      //update where we're trying to put the texture.
      dst.x = (x * pic_size) - (offset.x);
      dst.y = (y * pic_size) - (offset.y);

      //Load the map.
      switch( global::mymap.getMapCell(x,y) ) {
      //Use this to make sure we aren't try to load a non-existing part
      case tileSpace:
      case tileCoin:
      case tilePole:
      case tileBricksGray:
      case tileBricksGreen:
      case tileBricksLarge:
      case tileBricksOrange:
      case tileBricksRed:
      case tileBricksSmall:
        SDL_RenderCopy(ren, textures.maptiles,
                       &clips[global::mymap.getMapCell(x,y)], &dst);
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
  for (uchar i = 0; i < global::mymap.numMonsters; ++i) {
    tempMonster = global::enty.getMonster(i);
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
  if (blnloaded.blnMessageFont) { writemessage(); }
  //show renderer
  SDL_RenderPresent(ren);
  global::tick.wait();
}
/*****************************************************************************/
void clsScreen::cleanup(void) {
  /////////////////////////////////////////////////
  /// @brief This will attempt to delete Textures, and the Window / Renderer
  ///        from memory if their representative loaded boolean is true. Will delete
  ///        * Map tile textures
  ///        * Error textures
  ///        * Message Font
  ///        * Message texture
  ///        * Renderer
  ///        * Window
  ///
  /// @return void
  /////////////////////////////////////////////////
  if (blnloaded.blnMapTiles) {
    SDL_DestroyTexture(textures.maptiles);
    blnloaded.blnMapTiles = false;
    if (global::blnDebugMode) { printf("Tiles texture destroyed\n"); }
  }

  if (blnloaded.blnErrortex) {
    SDL_DestroyTexture(textures.errortex);
    blnloaded.blnErrortex = false;
    if (global::blnDebugMode) { printf("Error texture destroyed\n"); }
  }

  if (blnloaded.blnMessageFont) {
    TTF_CloseFont(MessageFont);
    blnloaded.blnMessageFont = false;
    if (global::blnDebugMode) { printf("Message font destroyed\n"); }
  }

  if (blnloaded.blnMessage) {
    SDL_DestroyTexture(textures.texmessage);
    blnloaded.blnMessage = false;
    if (global::blnDebugMode) { printf("Message texture destroyed\n"); }
  }

	if (blnloaded.blnRenderer) {
    SDL_DestroyRenderer(ren);
    blnloaded.blnRenderer = false;
    if (global::blnDebugMode) { printf("Renderer destroyed\n"); }
  }
	if (blnloaded.blnWindow) {
    SDL_DestroyWindow(win);
    blnloaded.blnWindow = false;
    if (global::blnDebugMode) { printf("Window destroyed\n"); }
  }
}
/*****************************************************************************/
void clsScreen::error(void) {
  /////////////////////////////////////////////////
  /// @brief Will print out the error generated by SDL if something goes wrong
  /// @return void
  /////////////////////////////////////////////////
  cleanup();
  global::blnError = true;
  printf("SDL error: %s\n", SDL_GetError());
  printf("TTF error: %s\n", TTF_GetError());
  printf("IMG error: %s\n", IMG_GetError());
  bln_SDL_started = false;
	getchar();
}
/*****************************************************************************/
void clsScreen::loadTextures() {
  /////////////////////////////////////////////////
  /// @brief Will load all the images that will be needed.
  /// @return void
  ////////////////////////////////////////////////

  std::string path = DEFINED_DEFAULT_IMAGE_PATH;
  path += "tiles.png";

  //Load the error texture first.
  SDL_Surface* temp = IMG_ReadXPMFromArray(image_error_xpm);
  textures.errortex = (temp == nullptr) ?
      nullptr : SDL_CreateTextureFromSurface(ren,temp);
	if (textures.errortex == nullptr) {
    printf("Failed to create texture.\n");
    error();
	} else {
    if (global::blnDebugMode) { printf("Surface to texture successful\n"); }
    blnloaded.blnErrortex = true;
  }

    //Now load the tiles
  temp = IMG_Load(path.c_str());
	textures.maptiles = (temp == nullptr) ?
      nullptr : SDL_CreateTextureFromSurface(ren,temp);
	SDL_FreeSurface(temp);
	if (textures.maptiles == nullptr) {
    //Cannot make texture; replace the clips to be all 0,0
    //and set it to use the error texture instead.
    printf("Tiles could not be converted to texture.\n");
    textures.maptiles = textures.errortex;
    for (uchar i = 0; i < defined::kNumMapTiles; ++i) { clips[i] = {0,0}; }
    blnloaded.blnMapTiles = true;
    return;
	} else {
    if (global::blnDebugMode) {printf("Tiles converted to texture successful\n");}
    blnloaded.blnMapTiles = true;
  }
}
/*****************************************************************************/
void clsScreen::playerDeath(void) {
  /////////////////////////////////////////////////
  /// @brief Plays short death animation
	///        only shows up if the Map::move returns DEAD
	///        The games "pauses" for a second then the player will move up
	///        3 spaces then down about 4 spaces (depending on starting point)
	///        the whole thing happens in 5 frames.
  /// @return void
  /////////////////////////////////////////////////

  /** \todo (GamerMan7799#1#): Add tileDeadPlayer with its own image to better tell when the death animation is happening */
	BPLYR tempPlayer;
	tempPlayer = global::enty.getPlayerBase();

	for (uchar i = 0; i < defined::kGoalFPS; ++i) { global::tick.wait(); } //wait for a second.
	for (uchar i = 0; i < 60; ++i) {
		if (i < 30) { tempPlayer.vel.y = -1.5 * global::physics::fMaxVelocity; }
    else { tempPlayer.vel.y = global::physics::fMaxVelocity; }

    tempPlayer.location.x += tempPlayer.vel.x * (1 / defined::kGoalFPS);
    tempPlayer.location.y += tempPlayer.vel.y * (1 / defined::kGoalFPS);

    global::enty.setPlayer(tempPlayer);
		update();
	}
}
/*****************************************************************************/
void clsScreen::writemessage(void) {
    /////////////////////////////////////////////////
    /// @brief Now work on Making the messages that will appear on the screen
    ///
    ///        I imagine that I am doing terrible programing things
    ///        With how I'm switch between char's and strings but whatever
    /// @return void
    /////////////////////////////////////////////////

    // These are char arrays that will act as "Strings" for building the messages to appear
    char strClock[8];
    char strGenNum[5];
    char strPlayerNum[4];
    char strFitness[7];

    /** \todo (GamerMan7799#5#): Somehow detect if over wall and change color of text */
    std::string message;
    sprintf(strClock, "%8u", global::tick.getClockTime());

    SDL_Surface* surmessage = TTF_RenderText_Solid(MessageFont,
                                                   strClock, colors.Black);
    textures.texmessage = (surmessage == nullptr) ?
        nullptr : SDL_CreateTextureFromSurface(ren, surmessage);
    if (textures.texmessage == nullptr) {
      printf("Failed to convert message surface to texture.\n");
      error();
      return;
    } else {
      if (global::blnDebugMode) {
          printf("Surface texture successfully created\n");
      }
      blnloaded.blnMessage = true;
    }

    SDL_Rect dst;
    SDL_QueryTexture(textures.texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.y = 0;
    dst.x = width - dst.w;
    SDL_RenderCopy(ren, textures.texmessage, NULL, &dst);
    if (global::blnDebugMode) { printf("Clock placed.\n"); }

    PLYR tempPlayer = global::enty.getPlayer();

    //Covert all the values into their char arrays
    int valuesscanned;

    //Make the status string
    //Also make sure that each values has at least 1 values scanned in to avoid an error
    message = "Generation: ";
    valuesscanned = sprintf(strGenNum, "%2u", global::enty.uchrGenNum);
    if (valuesscanned >= 1) {
      strGenNum[4] = '\0'; //Make sure that the char is null terminated or the program crashes.
      message += std::string(strGenNum);
    }
    message += "     Player: ";
    valuesscanned = sprintf(strPlayerNum, "%3u", global::enty.uchrPlayerNum);
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
    if (global::blnDebugMode) { printf("Status message made.\n"); }

    surmessage = TTF_RenderText_Solid(MessageFont, message.c_str(),
                                      colors.Black);
    textures.texmessage = (surmessage == nullptr) ?
        nullptr : SDL_CreateTextureFromSurface(ren, surmessage);
    if (textures.texmessage == nullptr) {
      printf("Failed to convert message surface to texture.\n");
      blnloaded.blnMessage = false;
      error();
      return;
    } else {
      if (global::blnDebugMode) {
          printf("Surface texture successfully created\n");
      }
      blnloaded.blnMessage = true;
    }

    SDL_QueryTexture(textures.texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.x = (int)(width - dst.w);
    dst.y = height - 30;

    SDL_RenderCopy(ren, textures.texmessage, NULL, &dst);
    SDL_FreeSurface(surmessage);
}
/*****************************************************************************/
void clsScreen::set_clips() {
  /////////////////////////////////////////////////
  /// @brief Set all the locations of the specific tiles in the tiles.png
  ///        Since all of the sizes are the same we will do this all together to save space
  /// @return void
  /////////////////////////////////////////////////

  /*     The Picture Coordinates (x,y)
   *     we multiply this by the pic size to get the clip
   *     +-----+-----+-----+
   *     |(0,0)|(1,0)|(2,0)|
   *     +-----+-----+-----+
   *     |(0,1)|(1,1)|(2,1)|
   *     +-----+-----+-----+
   *     |(0,2)|(1,2)|(2,2)|
   *     +-----+-----+-----+
   *     |(0,3)|(1,3)|(2,3)|
   *     +-----+-----+-----+
   */

   //First Row (Space, Wall, Player)
   clips[tileSpace] = {0 * pic_size, 0 * pic_size, pic_size, pic_size};
   clips[tileBricksLarge] = {1 * pic_size, 0 * pic_size, pic_size, pic_size};
   clips[tilePlayer] = {2 * pic_size, 0 * pic_size, pic_size, pic_size};

   //Second Row (Pole, Monster, Coin)
   clips[tilePole] = {0 * pic_size, 1 * pic_size, pic_size, pic_size};
   clips[tileMonster] = {1 * pic_size, 1 * pic_size, pic_size, pic_size};
   clips[tileCoin] = {2 * pic_size, 1 * pic_size, pic_size, pic_size};

   //Third Row (Bricks Small, bricks Gray, bricks Green)
   clips[tileBricksSmall] = {0 * pic_size,  2 * pic_size, pic_size, pic_size};
   clips[tileBricksGray] = {1 * pic_size, 2 * pic_size, pic_size, pic_size};
   clips[tileBricksGreen] = {2 * pic_size, 2 * pic_size, pic_size, pic_size};

   //Second Row (Bricks Orange, Bricks Red
   clips[tileBricksOrange] = {0 * pic_size, 3 * pic_size, pic_size, pic_size};
   clips[tileBricksRed] = {1 * pic_size, 3 * pic_size, pic_size, pic_size};
}
/*****************************************************************************/
