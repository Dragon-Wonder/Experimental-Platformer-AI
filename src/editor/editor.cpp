/*****************************************************************************/
#include "editor.h"
/*****************************************************************************/
#include "../ui/image_error.xpm"
#include "image_toolbox_frame.xpm"
#inlcude "image_tools.xpm"
/*****************************************************************************/
clsEditor::clsEditor() {

}
/*****************************************************************************/
clsEditor::~clsEditor() {

}
/*****************************************************************************/
void clsEditor::start(clsScreen *screen) {
  bool quit = false;
  make_buttons();

  m_screen = screen;

  while ( !quit ) {
    screen.show();
    if (SDL_PollEvent( &event ) != 0 ) {
      check_events( &event );
      if (event.type == SDL_QUIT) { quit = true; }
    } //end if event
  } //end while not quit

  //Clean up the screen
  screen.cleanup();
	return;
}
/*****************************************************************************/
void clsEditor::make_buttons() {
    /////////////////////////////////////////////////
    /// @brief Makes all the buttons for the toolbar.
    /////////////////////////////////////////////////

    uint centerx; //center of the toolbox
    centerx = (uint)(Screen::window.width / 2);
    //Calculate all the places, the tool box frame has a 2 px wide border all the way around.
    //Make tile buttons
    for (uchar i = 0; i < DEFINED_NUM_BUTN_TILES; i++) {
        Toolbar::button_xplaces[i] = centerx - ( ( (DEFINED_NUM_BUTN_TILES / 2)- i ) * 4 )
                                    - ( ( (DEFINED_NUM_BUTN_TILES / 2) - i ) * global::pic_size );
    }

    for (uchar i = 0; i < DEFINED_NUM_BUTN_TILES; i++) {
        Toolbar::tilebuttons[i].buttontype = i;
        Toolbar::tilebuttons[i].clip = &Textures::clips[i];
        Toolbar::tilebuttons[i].box.w = Toolbar::tilebuttons[i].box.h = global::pic_size;
        Toolbar::tilebuttons[i].box.y = 2;
        Toolbar::tilebuttons[i].box.x = Toolbar::button_xplaces[i];
    }

    //Make menu buttons
    for (uchar i = 0; i < DEFINED_NUM_BUTN_MENU; i++) {
        Toolbar::menubuttons[i].buttontype = menuClose + i;
        Toolbar::menubuttons[i].clip = &Textures::clips[menuClose + i];
        Toolbar::menubuttons[i].box.w = Toolbar::menubuttons[i].box.h = global::pic_size;
        Toolbar::menubuttons[i].box.y = 2;
        Toolbar::menubuttons[i].box.x = (Screen::window.width - 2) - ( (i+1) * global::pic_size);
    }
}
/*****************************************************************************/
void clsEditor::draw() {
    /////////////////////////////////////////////////
    /// @brief Draws the toolbar.
    /////////////////////////////////////////////////


    //draw toolbox frame
    SDL_Rect dst;
    dst.w = dst.h = global::pic_size + 4;
    dst.y = 0;
    for (uchar i = 0; i < DEFINED_NUM_BUTN_TILES; i++) {
        dst.x = Toolbar::button_xplaces[i] - 2;
        SDL_RenderCopy(Screen::window.ren, Textures::toolboxframe, NULL, &dst);
    }

    //Show all the tile buttons
    for (uchar i = 0; i < DEFINED_NUM_BUTN_TILES; i++) {
        SDL_RenderCopy(Screen::window.ren, Textures::tilemap,
                       Toolbar::tilebuttons[i].clip, &Toolbar::tilebuttons[i].box);
        if (Toolbar::tilebuttons[i].buttontype == paintbrush.CurrentTile) {
            SDL_RenderCopy(Screen::window.ren, Textures::tilemap,
                           &Textures::clips[menuFrame] , &Toolbar::tilebuttons[i].box);
        }
    }

    //show all the menu buttons
    for (uchar i = 0; i < DEFINED_NUM_BUTN_MENU; i++) {
        SDL_RenderCopy(Screen::window.ren, Textures::tilemap,
                       Toolbar::menubuttons[i].clip, &Toolbar::menubuttons[i].box);
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
        for (uchar i = 0; i < DEFINED_NUM_BUTN_TILES; i++) {
            if ( x >= Toolbar::tilebuttons[i].box.x && x <=
                            Toolbar::tilebuttons[i].box.x + Toolbar::tilebuttons[i].box.w ) { //In the x range
                if ( y >= Toolbar::tilebuttons[i].box.y && y <=
                            Toolbar::tilebuttons[i].box.y + Toolbar::tilebuttons[i].box.h) { //in the y range
                    paintbrush.CurrentTile = Toolbar::tilebuttons[i].buttontype;
                } //end if in y
            } // end if in x
        } // end for buttons

        //check all of the menu buttons
        for (uchar i = 0; i < DEFINED_NUM_BUTN_MENU; i++) {
            if ( x >= Toolbar::menubuttons[i].box.x && x <=
                        Toolbar::menubuttons[i].box.x + Toolbar::menubuttons[i].box.w ) { //In the x range
                if ( y >= Toolbar::menubuttons[i].box.y && y <=
                        Toolbar::menubuttons[i].box.y + Toolbar::menubuttons[i].box.h) { //in the y range
                    blnButtonDown = false;
                    switch (Toolbar::menubuttons[i].buttontype) {
                    case menuSave:
                        Map::save();
                        break;
                    case menuClose:
                        if (Screen::promptuser(promptYesNo, "Do you really want to quit?") == 'Y') {
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
        mapx = (uint) ( (x + Screen::offset.x) / global::pic_size);
        mapy = (uint) ( (y + Screen::offset.y) / global::pic_size);

        global::map[mapy][mapx] = paintbrush.CurrentTile;
    } else if (e->type == SDL_KEYDOWN) {

        switch( e->key.keysym.sym) {
        //All of the directional cases
        case SDLK_UP:
        case SDLK_w:
            Screen::offset.y -= global::pic_size;
            if (Screen::offset.y < 0) {Screen::offset.y = 0;}
            break;
        case SDLK_DOWN:
        case SDLK_s:
            Screen::offset.y += global::pic_size;
            if (Screen::offset.y > (DEFINED_MAP_HEIGHT * global::pic_size) - Screen::window.height)
                {Screen::offset.y = (DEFINED_MAP_HEIGHT * global::pic_size) - Screen::window.height;}
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            Screen::offset.x += global::pic_size;
            if (Screen::offset.x > (DEFINED_MAP_WIDTH * global::pic_size) - Screen::window.width)
                {Screen::offset.x = (DEFINED_MAP_WIDTH * global::pic_size) - Screen::window.width;}
            break;
        case SDLK_LEFT:
        case SDLK_a:
            Screen::offset.x -= global::pic_size;
            if (Screen::offset.x < 0) {Screen::offset.x = 0;}
            break;
        case SDLK_HOME:
            Screen::offset.x = Screen::offset.y = 0;
            break;
        case SDLK_END:
            Screen::offset.x = (DEFINED_MAP_WIDTH * global::pic_size) - Screen::window.width;
            break;
        case SDLK_PAGEDOWN:
            Screen::offset.x -= Screen::window.width;
            break;
        case SDLK_PAGEUP:
            Screen::offset.x += Screen::window.width;
            break;

        //Quiting cases
        case SDLK_q:
        case SDLK_ESCAPE:
          //change the event type to be a quit.
          if (Screen::promptuser(promptYesNo, "Do you really want to quit?") == 'Y') {
            e->type = SDL_QUIT;
            return;
          }
          break;

        //Menu cases
        case SDLK_v:
          Map::save();
          break;
        case SDLK_n:
          Map::newmap();
          break;
        case SDLK_l:
          Map::load();
          break;

        //Switch tile
        case SDLK_1:
            Toolbar::paintbrush.CurrentTile = tileSpace;
            break;
        case SDLK_2:
            Toolbar::paintbrush.CurrentTile = tileBricksLarge;
            break;
        case SDLK_3:
            Toolbar::paintbrush.CurrentTile = tilePlayer;
            break;
        case SDLK_4:
            Toolbar::paintbrush.CurrentTile = tilePole;
            break;
        case SDLK_5:
            Toolbar::paintbrush.CurrentTile = tileMonster;
            break;
        case SDLK_6:
            Toolbar::paintbrush.CurrentTile = tileCoin;
            break;
        case SDLK_7:
            Toolbar::paintbrush.CurrentTile = tileBricksSmall;
            break;
        case SDLK_8:
            Toolbar::paintbrush.CurrentTile = tileBricksGray;
            break;
        case SDLK_9:
            Toolbar::paintbrush.CurrentTile = tileBricksGreen;
            break;
        case SDLK_0:
            Toolbar::paintbrush.CurrentTile = tileBricksOrange;
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
      Answer = Screen::promptuser(promptYesNo, "Save already exists, would you like to overwrite?");
    } else { if (global::blnDebugMode) { printf("No save found.\n"); } }//end if savemap = null

    if (Answer == 'Y' || savemap == NULL) {
      fclose(savemap);
      savemap = fopen("map.sav", "w");
      if (global::blnDebugMode) {printf("Make save.\n");}
      for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
        for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
          fprintf(savemap,"%2x", global::map[y][x]);
        } //end for x
        fprintf(savemap, "\n");
      } //end for y
      Screen::promptuser(promptOkay, "Save successful.");
    } //end if map save
    fclose(savemap);
}
/*****************************************************************************/
void clsEditor::load() {
  /////////////////////////////////////////////////
  /// @brief Loads the map from map.sav.
  /////////////////////////////////////////////////

  if (Screen::promptuser(promptYesNo, "Do you want to load old map? All progress will be lost.") == 'Y' ){
    FILE* mapload = fopen("map.sav", "r");
    if (mapload == NULL) {Screen::promptuser(promptOkay, "Saved map could not be found!"); return;}
    for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
      for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
        fscanf(mapload, "%2x", &global::map[y][x] );
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
  if (Screen::promptuser(promptYesNo, "Do you really want to completely blank the map?") == 'Y') {
    for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
      for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
        global::map[y][x] = tileSpace;
      } //end for x
    } //end for y
  } //end if yes
}
/*****************************************************************************/
