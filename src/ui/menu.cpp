#include "menu.h"
/*****************************************************************************/
//TTF_Font* clsMenu::m_font;
/*****************************************************************************/
clsMenu::clsMenu() {
  //ctor
  clsScreen screen;
  clsSound audio;
  m_window = screen.getWinAtt();
  m_audio = audio;
}
/*****************************************************************************/
clsMenu::~clsMenu() {
  //dtor

  //TTF_CloseFont(m_font);
}
/*****************************************************************************/
char clsMenu::MainMenu() {
  /////////////////////////////////////////////////
  /// @brief The function that holds the Main menu.
  ///
  /// @return Char for what button was pressed
  ///
  /////////////////////////////////////////////////
  SDL_SetRenderDrawColor( m_window->ren, 0xFF, 0xFF, 0xFF, 0xFF );
  std::string text = "Attacker - the Game";
  SDL_Rect dst;
  SDL_Surface* surmessage = nullptr;
  SDL_Texture* texmessage = nullptr;
  SDL_Color clrblack = {0xFF,0xFF,0xFF,0xFF};

  surmessage = TTF_RenderText_Solid(m_window->font, text.c_str(),clrblack);
  texmessage = (surmessage == nullptr) ? nullptr : SDL_CreateTextureFromSurface(m_window->ren, surmessage);

  if (texmessage == nullptr) {
    printf("Failed to convert message surface to texture.\n");
    return menuError;
  } else {
    if (Global::blnDebugMode) { printf("Surface texture successfully created\n"); }
  }


  SDL_QueryTexture(texmessage,NULL,NULL, &dst.w, &dst.h);
  dst.x = (int) ((m_window->width - dst.w)/2);
  dst.y = (int) (m_window->height * 0.1);

  SDL_RenderCopy(m_window->ren, texmessage, NULL, &dst);

  button MainButtons[5];
  MainButtons[0] = {menuQuit,"Quit",{0,0,0,0}};
  MainButtons[1] = {menuOptions,"Options (WIP)",{0,0,0,0}};
  MainButtons[2] = {menuAbout, "About (WIP)",{0,0,0,0}};
  MainButtons[3] = {menuLoad,"Load Game",{0,0,0,0}};
  MainButtons[4] = {menuNew,"New Game",{0,0,0,0}};

  for (int i = 0; i < 5; i++) { //for each button
    surmessage = TTF_RenderText_Solid(m_window->font, MainButtons[i].text.c_str(),clrblack);
    texmessage = (surmessage == nullptr) ? nullptr : SDL_CreateTextureFromSurface(m_window->ren, surmessage);

    if (texmessage == nullptr) {
      printf("Failed to convert message surface to texture.\n");
      return menuError;
    } else {
      if (Global::blnDebugMode) { printf("Surface texture successfully created\n"); }
    }

    SDL_QueryTexture(texmessage,NULL,NULL, &dst.w, &dst.h);
    dst.x = (int) ((m_window->width - dst.w)/2);
    dst.y = (i==0) ? (m_window->height - dst.h - 30) :
                      (MainButtons[i-1].cords.y - dst.h - 30);

    SDL_SetRenderDrawColor( m_window->ren, 0x33, 0x66, 0x99, 0xFF );
    SDL_RenderDrawRect( m_window->ren, &dst );
    SDL_RenderCopy(m_window->ren, texmessage, NULL, &dst);
    MainButtons[i].cords = dst;
  }
  SDL_DestroyTexture(texmessage);
  SDL_FreeSurface(surmessage);

  SDL_Event event;
  char buttonpushed = menuError;

  bool done = false;
  SDL_RenderPresent(m_window->ren);
  do {
    SDL_PollEvent(&event);
    if ( event.type == SDL_MOUSEBUTTONUP ){
      LOC mouse_click;
      SDL_GetMouseState(&mouse_click.x, &mouse_click.y);
      for (int i = 0; i < 5; i++ ) {
        if ( clickcheck(mouse_click, MainButtons[i].cords)) {
          done = true;
          buttonpushed = MainButtons[i].type;
        } //end check if button
      } //end for menu buttons
    } //end if event
  } while (!done);

  return buttonpushed;
}
/*****************************************************************************/
void clsMenu::AboutMenu() {
  /////////////////////////////////////////////////
  /// @brief Will show the about menu
  /////////////////////////////////////////////////
  /// @todo (GamerMan7799#8#) Add about menu
  return;
}
/*****************************************************************************/
void clsMenu::OptionsMenu() {
  /////////////////////////////////////////////////
  /// @brief Will show the options Menu.
  ///        Possibly unneeded. Only things I can think of are:
  ///        * Screen Size (already can with config, and would require restart)
  ///        * Controls (no idea how to do this, so likely won't be able to)
  ///        * Volume
  ///        * Quality (considering how low quality this already is, if you are having issues you need a new computer)
  /////////////////////////////////////////////////
  /// @todo (GamerMan7799#9#) Add options menu (possibly unneeded)
  return;
}
/*****************************************************************************/
bool clsMenu::clickcheck(LOC mouse, SDL_Rect box) {
  /////////////////////////////////////////////////
  /// @brief Checks if mouse click was on a button
  ///
  /// @param mouse = X/Y Coordinates of the mouse click
  /// @param box = SDL_Rect coordinates for the button we are checking
  /// @return True/False if button was clicked on
  ///
  /////////////////////////////////////////////////
  if (mouse.x < box.x ) {return false;}
  else if (mouse.x > box.x + box.w) {return false;}
  else if (mouse.y < box.y) {return false;}
  else if (mouse.y > box.y + box.h) {return false;}
  else {return true;}
}
/*****************************************************************************/
char clsMenu::GameMenu() {
    /////////////////////////////////////////////////
    /// @brief Shows In game menu, which will hold things like:
    ///        * Save Game
    ///        * Load game
    ///        * Quit
    ///        * Options
    ///
    /// @return What was clicked on
    ///
    /////////////////////////////////////////////////
    /// @todo (GamerMan7799#2#) Add in-game menu
    return menuOptions;
}
/*****************************************************************************/
char clsMenu::promptUser(char promptType, std::string message) {
  /////////////////////////////////////////////////
  /// @brief Prompts the user for some information.
  ///
  /// @param propmttype = What prompt to the user
  /// @param message = The message that appears above the prompt.
  /// @return What key is pressed by the user.
  ///
  /////////////////////////////////////////////////

  /// @todo (GamerMan7799#9#) Centralize color for this class

  if(m_window->font == nullptr) {return returnError;}
  SDL_RenderClear(m_window->ren);
  SDL_SetRenderDrawColor( m_window->ren, 0xFF, 0xFF, 0xFF, 0x00 );
  SDL_Color clrblack = {0x00, 0x00, 0x00, 0x00}; //Make the color black for font

  SDL_Surface* surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                                 clrblack);
  SDL_Texture* texmessage = nullptr;
  texmessage = (surmessage == nullptr) ? nullptr :
               SDL_CreateTextureFromSurface(m_window->ren,surmessage);

  if (texmessage == nullptr) {
    printf("Message not converted to texture.\n");
    return returnError;
  }

  SDL_Rect dst;

  SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
  //figure out x and y so that message is in the middle
  dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
  dst.y = (uint) ((m_window->height / 2) - (dst.h / 2));
  SDL_RenderCopy(m_window->ren,texmessage, NULL, &dst);


  //add second message based on prompt type
  switch(promptType) {
  case promptYesNo :
    message = "Please hit Y for yes, or N for no.";
    break;
  case prompOkay :
    message = "Please hit any button to close.";
    break;
  default:
    message = " ";
    break;
  }

  surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(), clrblack);
  texmessage = (surmessage == nullptr) ? nullptr :
               SDL_CreateTextureFromSurface(m_window->ren,surmessage);

   if (texmessage == nullptr) {
    printf("Error making message.\n");
    return returnError;
   }

  SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
  //figure out x and y so that message is in the middle, but below the first message
  dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
  dst.y = (uint) ((m_window->height / 2) + (dst.h / 2));

  SDL_RenderCopy(m_window->ren,texmessage,NULL,&dst);
  SDL_RenderPresent(m_window->ren);

  bool blnStopLoop = false;
  char keypress;
  SDL_Event event;



  //start looping while waiting for response.
  do {
    if(SDL_PollEvent( &event )) {
      if (event.type == SDL_QUIT) {
        //player wants to quit, leave loop
        keypress = returnNo;
        blnStopLoop = true;
      } else if (event.type == SDL_KEYDOWN) {
        switch (promptType) {
        case promptYesNo:
          switch (event.key.keysym.sym) {
          case SDLK_y:
            keypress = returnYes;
            blnStopLoop = true;
            break;
          case SDLK_n:
            keypress = returnNo;
            blnStopLoop = true;
            break;
          default:
            printf("Invalid key choice.\n");
            break;
          } //end switch YES/NO
          break;
        case prompOkay:
        default:
          keypress = returnMaybe;
          blnStopLoop = true;
          break;
        } //end switch prompt type
      } //end if event type
    } //end if poll event
  } while (!blnStopLoop);


  SDL_DestroyTexture(texmessage);
  SDL_FreeSurface(surmessage);

  return keypress;
}
/*****************************************************************************/
stats clsMenu::doLevelUp(stats currStats) {
  /////////////////////////////////////////////////
  /// @brief Have player level up with SDL menu
  ///
  /// @param currStats = Current stats
  /// @return New stats
  ///
  /////////////////////////////////////////////////

  if(m_window->font == nullptr) {
    // No font loaded, kill screen, close game.
    return {0,0,0,0,0,0};
  }

  SDL_SetRenderDrawColor( m_window->ren, 0xFF, 0xFF, 0xFF, 0x00 );
  SDL_Color clrblack = {0x00, 0x00, 0x00, 0x00}; //Make the color black for font
  stats newStats = currStats;
  newStats.level += 1;
  char upgradePoints = 20;
  bool finish_level_up = false;
  bool refresh_screen = true;
  levelupLine LevelUpBars[5];

  int seperator_size = (m_window->height - 30)/5;
  LevelUpBars[0].text.y = m_window->height - 30 - seperator_size;

  for( int i = 1; i < 5; ++i) {
    LevelUpBars[i].text.y = LevelUpBars[i].text.y - seperator_size;
  }

  SDL_Rect dst;
  SDL_Surface* surmessage = nullptr;
  SDL_Texture* texmessage = nullptr;
  std::string message;


  do {
    if (refresh_screen) {
      SDL_RenderClear(m_window->ren);
      dst.y = 60;
      message = "LEVEL UP!";
      surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                                 clrblack);
      texmessage = (surmessage == nullptr) ? nullptr :
                   SDL_CreateTextureFromSurface(m_window->ren,surmessage);
      if (texmessage == nullptr) {
        printf("Message not converted to texture.\n");
        return {0,0,0,0,0,0};
      }
      SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
      //figure out x so that message is in the middle
      dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
      SDL_RenderCopy(m_window->ren,texmessage, NULL, &dst);

      dst.y = 120;
      message = "Press 'D', when done.\nPoints left : ";
      message += std::to_string(upgradePoints);
      surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                                 clrblack);
      texmessage = (surmessage == nullptr) ? nullptr :
                   SDL_CreateTextureFromSurface(m_window->ren,surmessage);
      if (texmessage == nullptr) {
        printf("Message not converted to texture.\n");
        return {0,0,0,0,0,0};
      }
      SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
      //figure out x so that message is in the middle
      dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
      SDL_RenderCopy(m_window->ren,texmessage, NULL, &dst);

      // define initial positions for the bars





      // draw each level up bar.
      for (int i = 0; i < 5; ++i) {
        LevelUpBars.type = (uchar) i;
        drawLevelUpBar(LevelUpBars[i]);
      }

      SDL_RenderPresent(m_window->ren);
      refresh_screen = false;
    } // end if refresh_screen

    char returnPress;
    if(SDL_PollEvent( &event )) {
      refresh_screen = true;
      if (event.type == SDL_QUIT) {
        //player wants to quit, leave loop
        newStats = {0,0,0,0,0,0};
        finish_level_up = true;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_q:
          newStats = {0,0,0,0,0,0};
          blnStopLoop = true;
          break;
        case SDLK_d:
          // done with level up
          /// @todo (GamerMan7799#9#) Clean up this section of code, can be made to be more compressed.
          if (upgradePoints > 0) {
            // user has points left, prompt if truly done
            returnPress = promptUser(promptYesNo,
                          "Unspent points remain, are you sure you're done?");
            if(returnPress == returnYes) {
              finish_level_up = true;
            } else {
              finish_level_up = false;
            }
          } else {
            // prompt to confirm new stats
            returnPress = promptUser(promptYesNo, "Confirm new Stats?");
            if(returnPress == returnYes) {
              finish_level_up = true;
            } else {
              finish_level_up = false;
            }
            SDL_RenderClear(m_window->ren);
          }
          break;
        } //end switch keypress
      } else if ( event.type == SDL_MOUSEBUTTONUP ){
        bool button_clicked = false;
        char increment_dir = 0;
        LOC mouse_click;
        SDL_GetMouseState(&mouse_click.x, &mouse_click.y);
        for (int i = 0; i < 5; i++ ) {
          if ( clickcheck(mouse_click, LevelUpBars[i].plus) ) {
            button_clicked = true;
            increment_dir = 1;
          } else if ( clickcheck(mouse_click, LevelUpBars[i].minus) ) {
            button_clicked = true;
            increment_dir = -1;
          } else { button_clicked = false; }

          if (button_clicked) {
            switch (LevelUpBars[i].type) {
            case statStr:
              // check if trying to remove points user already had
              if (newStats.str + increment_dir <= currStats.str) {
                m_audio.playSound(soundError,2);
              } else {
                upgradePoints -= increment_dir;
                newStats.str += increment_dir;
              }
              break;
            case statCon:
              // check if trying to remove points user already had
              if (newStats.con + increment_dir <= currStats.con) {
                m_audio.playSound(soundError,2);
              } else {
                upgradePoints -= increment_dir;
                newStats.son += increment_dir;
              }
              break;
            case statDef:
              // check if trying to remove points user already had
              if (newStats.def + increment_dir <= currStats.def) {
                m_audio.playSound(soundError,2);
              } else {
                upgradePoints -= increment_dir;
                newStats.def += increment_dir;
              }
              break;
            case statDex:
              // check if trying to remove points user already had
              if (newStats.dex + increment_dir <= currStats.dex) {
                m_audio.playSound(soundError,2);
              } else {
                upgradePoints -= increment_dir;
                newStats.dex += increment_dir;
              }
              break;
            case statLuk:
              // check if trying to remove points user already had
              if (newStats.luk + increment_dir <= currStats.luk) {
                m_audio.playSound(soundError,2);
              } else {
                upgradePoints -= increment_dir;
                newStats.luk += increment_dir;
              }
              break;
            default:
              if(global::blnDebugMode) {
                printf("ERROR in level up, invalid stat.\n");
              }
              break;
            } //end switch
          } // end if button_clicked
        } //end for menu buttons
      } //end if event
    } //end if poll event
  } while (!finish_level_up);
  return newStats;
}
/*****************************************************************************/
void clsMenu::drawLevelUpBar(levelupLine StatLine) {
  /////////////////////////////////////////////////
  /// @brief Finish this later
  ///
  /// @param currStats = Current stats
  /// @return New stats
  ///
  /////////////////////////////////////////////////

  SDL_SetRenderDrawColor(m_window->ren,0x00, 0x00, 0x00, 0x00);
  SDL_Color clrblack = {0x00, 0x00, 0x00, 0x00}; //Make the color black for font
  SDL_Rect dst;
  StatLine.plus.y = StatLine.text.y;
  StatLine.minus.y = StatLine.text.y;

  std::string message = "+";

  dst = StatLine.plus;
  SDL_Surface* surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                                 clrblack);
  SDL_Texture* texmessage = nullptr;
  texmessage = (surmessage == nullptr) ? nullptr :
               SDL_CreateTextureFromSurface(m_window->ren,surmessage);

  if (texmessage == nullptr) { return; }

  SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderDrawRect(m_window->ren,&dst);
  SDL_RenderCopy(m_window->ren,texmessage,NULL,&dst);

  message = "-";
  dst = StatLine.minus;
  surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                                 clrblack);
  texmessage = nullptr;
  texmessage = (surmessage == nullptr) ? nullptr :
               SDL_CreateTextureFromSurface(m_window->ren,surmessage);

  if (texmessage == nullptr) { return; }

  SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderDrawRect(m_window->ren,&dst);
  SDL_RenderCopy(m_window->ren,texmessage,NULL,&dst);

  switch (StatLine.type) {
  case statStr:
    message = "Strength";
    break;
  case statCon:
    message = "Constitution";
    break;
  case statDef:
    message = "Defense";
    break;
  case statDex:
    message = "Dexterity";
    break;
  case statLuk:
    message = "Luck";
    break;
  default:
    message = "ERROR!";
    break;
  }

  surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                                 clrblack);
  texmessage = nullptr;
  texmessage = (surmessage == nullptr) ? nullptr :
               SDL_CreateTextureFromSurface(m_window->ren,surmessage);

  if (texmessage == nullptr) { return; }
  dst = StateLine.text;

  SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);

  // Name of stat should appear above the value of it
  dst.y = (uint) (dst.y - dst.h);
  dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
  SDL_RenderCopy(m_window->ren,texmessage,NULL,&dst);

  message = std::to_string(StatLine.value);
  dst = StatLine.text;
  surmessage = TTF_RenderText_Solid(m_window->font, message.c_str(),
                                               clrblack);
  texmessage = nullptr;
  texmessage = (surmessage == nullptr) ? nullptr :
               SDL_CreateTextureFromSurface(m_window->ren,surmessage);

  if (texmessage == nullptr) { return; }

  SDL_QueryTexture(texmessage, NULL, NULL, &dst.w, &dst.h);
  dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
  SDL_RenderCopy(m_window->ren,texmessage,NULL,&dst);

  SDL_DestroyTexture(texmessage);
  SDL_FreeSurface(surmessage);
}
/*****************************************************************************/
