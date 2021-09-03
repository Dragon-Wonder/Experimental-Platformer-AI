/*****************************************************************************/
#include "editor.h"
/*****************************************************************************/
clsEditor::clsEditor() {
  //ctor
  //clsScreen screen;
  m_window = screen.getWinAtt();
  m_textures = screen.get_Textures();
  m_offset = {0,0};

  for (uchar i = 0; i < defined::kNumMapTiles; ++i ) { map_clips[i] = screen.getMapClips(i); }
  for (uchar i = 0; i < defined::kNumTools; ++i ) { tool_clips[i] = screen.getToolClips(i); }
}
/*****************************************************************************/
clsEditor::~clsEditor() {

}
/*****************************************************************************/
void clsEditor::start() {
  bool quit = false;
  make_buttons();
  SDL_Event event;

  while ( !quit ) {
    if (SDL_PollEvent( &event ) != 0 ) {
      check_events( &event );
      update();
      if (event.type == SDL_QUIT) { quit = true; }
    } //end if event
  } //end while not quit

	return;
}
/*****************************************************************************/
void clsEditor::make_buttons() {
    /////////////////////////////////////////////////
    /// @brief Makes all the buttons for the toolbar.
    /////////////////////////////////////////////////

    uint centerx; //center of the toolbox
    centerx = (uint)(m_window->width / 2);
    //Calculate all the places, the tool box frame has a 2 px wide border all the way around.
    //Make tile buttons
    for (uchar i = 0; i < defined::kNumMapTiles; i++) {
        button_xplaces[i] = centerx - ( ( (defined::kNumMapTiles / 2)- i ) * 4 )
                                    - ( ( (defined::kNumMapTiles / 2) - i ) * m_window->pic_size );
    }



    for (uchar i = 0; i < defined::kNumMapTiles; i++) {
      tilebuttons[i].buttontype = i;
      tilebuttons[i].clip = &map_clips[i];
      tilebuttons[i].box.w = tilebuttons[i].box.h = m_window->pic_size;
      tilebuttons[i].box.y = 2;
      tilebuttons[i].box.x = button_xplaces[i];
    }


    //Make menu buttons
    for (uchar i = 0; i < defined::kNumTools; i++) {
        menubuttons[i].buttontype = toolQuit + i;
        menubuttons[i].clip = &tool_clips[i];
        menubuttons[i].box.w = menubuttons[i].box.h = m_window->pic_size;
        menubuttons[i].box.y = 2;
        menubuttons[i].box.x = (m_window->width - 2) - ( (i+1) * m_window->pic_size);
    }
}
/*****************************************************************************/
void clsEditor::toolbar_draw() {
  /////////////////////////////////////////////////
  /// @brief Draws the toolbar.
  /////////////////////////////////////////////////


  //draw toolbox frame
  SDL_Rect dst;
  dst.w = dst.h = m_window->pic_size + 4;
  dst.y = 0;
  for (uchar i = 0; i < defined::kNumMapTiles; i++) {
    dst.x = button_xplaces[i] - 2;
    SDL_RenderCopy(m_window->ren, m_textures->toolframetex, NULL, &dst);
  }

  //Show all the tile buttons
  for (uchar i = 0; i < defined::kNumMapTiles; i++) {
    SDL_RenderCopy(m_window->ren, m_textures->maptiles,
                   tilebuttons[i].clip, &tilebuttons[i].box);
    if (tilebuttons[i].buttontype == paintbrush.CurrentTile) {
        SDL_RenderCopy(m_window->ren, m_textures->maptiles,
                       NULL , &tilebuttons[i].box);
    }
  }

  //show all the menu buttons
  for (uchar i = 0; i < defined::kNumTools; i++) {
    SDL_RenderCopy(m_window->ren, m_textures->maptiles,
                   menubuttons[i].clip, &menubuttons[i].box);
  }
}
/*****************************************************************************/
void clsEditor::check_events(SDL_Event* e) {
    /////////////////////////////////////////////////
    /// @brief Checks an SDL event to see what the program should do.
    /// @param e = Pointer to the event that happened.
    /////////////////////////////////////////////////

    static bool blnButtonDown;
    if (e->type == SDL_MOUSEBUTTONUP) {blnButtonDown = false;}
    if ( e->type == SDL_MOUSEBUTTONDOWN || blnButtonDown) {
        blnButtonDown = true;
        //get Mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //check all of the tile buttons to see if we are on that one.
        for (uchar i = 0; i < defined::kNumMapTiles; i++) {
          if ( x >= tilebuttons[i].box.x && x <=
                        tilebuttons[i].box.x + tilebuttons[i].box.w ) { //In the x range
            if ( y >= tilebuttons[i].box.y && y <=
                        tilebuttons[i].box.y + tilebuttons[i].box.h) { //in the y range
                paintbrush.CurrentTile = tilebuttons[i].buttontype;
            } //end if in y
          } // end if in x
        } // end for buttons

        //check all of the menu buttons
        for (uchar i = 0; i < defined::kNumTools; i++) {
          if ( x >= menubuttons[i].box.x && x <=
                    menubuttons[i].box.x + menubuttons[i].box.w ) { //In the x range
            if ( y >= menubuttons[i].box.y && y <=
                    menubuttons[i].box.y + menubuttons[i].box.h) { //in the y range
                blnButtonDown = false;
                switch (menubuttons[i].buttontype) {
                case menuSave:
                    /** \todo (GamerMan7799#1#): Re-implement saving */
                    //Map::save();
                    break;
                case menuQuit:
                    if (promptuser(promptYesNo, "Do you really want to quit?") == 'Y') {
                      e->type = SDL_QUIT;
                    } //end if yes
                    break;
                } //end switch
            } //end if in y
          } // end if in x
        } //end for menu buttons


        //user did not click on any buttons therefore change the map tile.
        //convert to map coordinates
        uint mapx, mapy;
        mapx = (uint) ( (x + m_offset.x) / m_window->pic_size);
        mapy = (uint) ( (y + m_offset.y) / m_window->pic_size);

        global::mymap.setMapCell(mapx,mapy,paintbrush.CurrentTile);
    } else if (e->type == SDL_KEYDOWN) {

        switch( e->key.keysym.sym) {
        //All of the directional cases
        case SDLK_UP:
        case SDLK_w:
            m_offset.y -= m_window->pic_size;
            if (m_offset.y < 0) { m_offset.y = 0; }
            break;
        case SDLK_DOWN:
        case SDLK_s:
            m_offset.y += m_window->pic_size;
            if (m_offset.y > (defined::kMapHeight * m_window->pic_size) - m_window->height)
                { m_offset.y = (defined::kMapHeight * m_window->pic_size) - m_window->height; }
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            m_offset.x += m_window->pic_size;
            if (m_offset.x > (defined::kMapWidth * m_window->pic_size) - m_window->width)
                { m_offset.x = (defined::kMapWidth * m_window->pic_size) - m_window->width; }
            break;
        case SDLK_LEFT:
        case SDLK_a:
            m_offset.x -= m_window->pic_size;
            if (m_offset.x < 0) { m_offset.x = 0; }
            break;
        case SDLK_HOME:
            m_offset.x = m_offset.y = 0;
            break;
        case SDLK_END:
            m_offset.x = (defined::kMapWidth * m_window->pic_size) - m_window->width;
            break;
        case SDLK_PAGEDOWN:
            m_offset.x -= m_window->width;
            break;
        case SDLK_PAGEUP:
            m_offset.x += m_window->width;
            break;

        //Quiting cases
        case SDLK_q:
        case SDLK_ESCAPE:
          //change the event type to be a quit.
          if (promptuser(promptYesNo, "Do you really want to quit?") == 'Y') {
            e->type = SDL_QUIT;
            return;
          }
          break;

        //Menu cases
        case SDLK_v:
          save();
          break;
        case SDLK_n:
          newmap();
          break;
        case SDLK_l:
          load();
          break;

        //Switch tile
        case SDLK_1:
            paintbrush.CurrentTile = tileSpace;
            break;
        case SDLK_2:
            paintbrush.CurrentTile = tileBricksLarge;
            break;
        case SDLK_3:
            paintbrush.CurrentTile = tilePlayer;
            break;
        case SDLK_4:
            paintbrush.CurrentTile = tilePole;
            break;
        case SDLK_5:
            paintbrush.CurrentTile = tileMonster;
            break;
        case SDLK_6:
            paintbrush.CurrentTile = tileCoin;
            break;
        case SDLK_7:
            paintbrush.CurrentTile = tileBricksSmall;
            break;
        case SDLK_8:
            paintbrush.CurrentTile = tileBricksGray;
            break;
        case SDLK_9:
            paintbrush.CurrentTile = tileBricksGreen;
            break;
        case SDLK_0:
            paintbrush.CurrentTile = tileBricksOrange;
            break;
        } //end switch
    } //end if event type
}
/*****************************************************************************/
void clsEditor::save() {
    /////////////////////////////////////////////////
    /// @brief Saves the map to map.sav.
    /////////////////////////////////////////////////

    FILE* savemap;
    savemap = fopen("map.sav", "r");
    char Answer;
    if (savemap != NULL) {
      if (global::blnDebugMode) { printf("Save found.\n");}
      Answer = promptuser(promptYesNo, "Save already exists, would you like to overwrite?");
    } else { if (global::blnDebugMode) { printf("No save found.\n"); } }//end if savemap = null

    if (Answer == 'Y' || savemap == NULL) {
      fclose(savemap);
      savemap = fopen("map.sav", "w");
      if (global::blnDebugMode) {printf("Make save.\n");}
      for (uint y = 0; y < defined::kMapHeight; y++) {
        for (uint x = 0; x < defined::kMapWidth; x++) {
          fprintf(savemap,"%2x", global::mymap.getMapCell(x,y));
        } //end for x
        fprintf(savemap, "\n");
      } //end for y
      promptuser(promptOkay, "Save successful.");
    } //end if map save
    fclose(savemap);
}
/*****************************************************************************/
void clsEditor::load() {
  /////////////////////////////////////////////////
  /// @brief Loads the map from map.sav.
  /////////////////////////////////////////////////

  if (promptuser(promptYesNo, "Do you want to load old map? All progress will be lost.") == 'Y' ){
    FILE* mapload = fopen("map.sav", "r");
    if (mapload == NULL) { promptuser(promptOkay, "Saved map could not be found!"); return; }
    for (uint y = 0; y < defined::kMapHeight; y++) {
      for (uint x = 0; x < defined::kMapWidth; x++) {
        fscanf(mapload, "%2x", global::mymap.getMapCell(x,y) );
      } //end for x
    } //end for y
  } //end if yes
}
/*****************************************************************************/
void clsEditor::newmap() {
  /////////////////////////////////////////////////
  /// @brief Fills all of the map with spaces.
  /////////////////////////////////////////////////

  //New map; completely blank
  if (promptuser(promptYesNo, "Do you really want to completely blank the map?") == 'Y') {
    for (uint y = 0; y < defined::kMapHeight; y++) {
      for (uint x = 0; x < defined::kMapWidth; x++) {
        global::mymap.setMapCell(x,y,tileSpace);
      } //end for x
    } //end for y
  } //end if yes
}
/*****************************************************************************/
void clsEditor::update() {
  SDL_RenderClear(m_window->ren);
  screen.update();
  //map_draw();
  toolbar_draw();
  SDL_RenderPresent(m_window->ren);
}
/*****************************************************************************/
char clsEditor::promptuser(uchar prompttype, std::string message) {
  /////////////////////////////////////////////////
  /// @brief Prompts the user for some information.
  ///
  /// @param propmttype = What prompt to the user, Yes/No or an Okay.
  /// @param message = The message that appears above the prompt.
  /// @return What key is pressed by the user.
  ///
  /////////////////////////////////////////////////

  //Prompt the user for something, prompt the user for something.
  //returns what they say.

  //Clear the Renderer
  SDL_RenderClear(m_window->ren);

  SDL_Surface* surmessage = TTF_RenderText_Solid(m_window->font,
                            message.c_str(), m_window->colors.Black);
  if (surmessage == nullptr) {
    //screen.showErrors();
    return 'F';
  }

  SDL_Rect dst;

  m_texmessage = SDL_CreateTextureFromSurface(m_window->ren, surmessage);
  if (m_texmessage == nullptr) {
    //screen.showErrors();
    return 'F';
  } else { blnMessage = true; }

  SDL_QueryTexture(m_texmessage, NULL, NULL, &dst.w, &dst.h);
  //figure out x and y so that message is in the middle

  dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
  dst.y = (uint) ((m_window->height / 2) - (dst.h / 2));

  SDL_RenderCopy(m_window->ren, m_texmessage, NULL, &dst);

  std::string message2;

  switch (prompttype) {
  case promptYesNo:
    message2 = "Please hit Y for yes, or N for no.";
    break;
  case promptOkay:
    message2 = "Please hit any button to close.";
    break;
  default :
    message2 = " ";
    break;
  }

  surmessage = TTF_RenderText_Solid(m_window->font,
                              message2.c_str(), m_window->colors.Black);
  if (surmessage == nullptr) {
    //screen.showErrors();
    return 'F';
  }

  m_texmessage = SDL_CreateTextureFromSurface(m_window->ren, surmessage);
  if (m_texmessage == nullptr) {
    //screen.showErrors();
    return 'F';
  } else { blnMessage = true; }

  SDL_QueryTexture(m_texmessage, NULL, NULL, &dst.w, &dst.h);
  //figure out x and y so that message is in the middle, but below the first message
  dst.x = (uint) ((m_window->width / 2) - (dst.w / 2));
  dst.y = (uint) ((m_window->height / 2) + (dst.h / 2));

  SDL_RenderCopy(m_window->ren, m_texmessage, NULL, &dst);

  bool blnStopLoop = false;
  char keyPress;
  SDL_Event event;

  //Start looping while wait for a response.
  do {
      SDL_RenderPresent(m_window->ren);
      if (SDL_PollEvent( &event ) ) {
          if (event.type == SDL_QUIT) {
              //player wants to quit leave the loop
              keyPress = 'N';
              blnStopLoop = true;
          } else if (event.type == SDL_KEYDOWN) {
              switch (prompttype) {
              case promptOkay:
                  keyPress = 'O';
                  blnStopLoop = true;
                  break;
              case promptYesNo:
                  switch (event.key.keysym.sym) {
                  case SDLK_y:
                      keyPress = 'Y';
                      blnStopLoop = true;
                      break;
                  case SDLK_n:
                      keyPress = 'N';
                      blnStopLoop = true;
                      break;
                  } //end switch key
              }//end switch prompt type
          } // end if event type
      } //end if poll event
  } while (blnStopLoop == false);
  return keyPress;
}
/*****************************************************************************/
