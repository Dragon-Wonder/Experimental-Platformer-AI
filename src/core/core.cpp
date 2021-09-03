/*****************************************************************************/
#include "core.h"
/*****************************************************************************/
clsCore::clsCore() {
  //ctor
}
/*****************************************************************************/
clsCore::~clsCore() {
  //dtor

  //SDL_KillThread(quit_thread);
}
/*****************************************************************************/
int clsCore::start() {
  global::cnfg.Check(); //Load the config file's values
	if (global::blnError) { printf("\nThere was an error!\n"); return 1; }

  //Put all of the config values into CnfgValues for easier reference
  Configures CnfgValues;
  CnfgValues = global::cnfg.getvalues();

  m_screen.start();
  if (!CnfgValues.blnShowMap) { m_screen.~clsScreen(); }
  else {
    exit_thread = SDL_CreateThread(exit_check, "exit_checker", (void *)NULL);
    if (exit_thread == NULL) {
      if (global::blnDebugMode) {
        printf("Exit thread not created\n");
        getchar();
      }
    }
  }

  if (global::blnError) { printf("\nThere was an error!\n"); return 1; }
  char menuselection;
  while (!global::blnQuit) {
    menuselection = m_menu.MainMenu();
    switch(menuselection) {
      case menuAbout:
        m_menu.AboutMenu();
        if (global::blnDebugMode) { printf("About shown.\n"); }
        break;
      case menuNew:
        if (global::blnDebugMode) { printf("New game.\n"); }

        srand(CnfgValues.uintSeed + ( CnfgValues.blnAppendTime ? time(NULL) : 0 ) );
        global::mymap.load();
        if (global::blnDebugMode) { printf("Map loaded\n"); }
        if (global::blnError) { printf("\nThere was an error!\n"); return 1; }
        global::mymap.restart();
        if (global::blnDebugMode) { printf("Map Restarted\n"); }
        if (global::blnError) { printf("\nThere was an error!\n"); return 1; }

        doGame();

        break;
      case menuOptions:
        m_menu.OptionsMenu();
        if (global::blnDebugMode) { printf("Options shown.\n"); }
        break;
      case menuEditor:
        global::mymap.load();
        m_editor.start();
        if (global::blnDebugMode) { printf("Editor run.\n"); }
        break;
      case menuError:
        if (global::blnDebugMode) { printf("Menu Error returned.\n"); }
      default:
        printf("ERROR!!! Now closing everything!\n");
        m_screen.showErrors();
      case menuQuit:
        global::blnQuit = true;
        break;
    } // end switch
  }; // end loop

  m_screen.~clsScreen();
	printf("\nDone\n");
}
/*****************************************************************************/
int clsCore::exit_check(void *data) {
  /////////////////////////////////////////////////
  /// @brief Function used on the multithread to always check for a quit command
  /// @return int
  /////////////////////////////////////////////////

  SDL_Event event;

  do {
    if (SDL_PollEvent( &event )) {
      if (global::blnDebugMode) {
        printf("Thread detected event.\n");
        getchar();
      }
      if (event.type == SDL_QUIT) {
          global::blnQuit = true;
          if (global::blnDebugMode) {
            printf("Thread detected close.\n");
            getchar();
          }
      } else if (event.type == SDL_KEYDOWN) {
        //Key has been pressed figure out what to do
        switch (event.key.keysym.sym) {
        case SDLK_q:
        case SDLK_ESCAPE:
          global::blnQuit = true;
          if (global::blnDebugMode) {
            printf("Thread detected quit.\n");
            getchar();
          }
          break;
        } //end switch
      }  // end what event
    } //end if event
  } while (!global::blnQuit);
}
/*****************************************************************************/
void clsCore::doFirstGeneration() {
  /////////////////////////////////////////////////
  /// @brief Handles the first generation stuff
  /////////////////////////////////////////////////
  Configures CnfgValues = global::cnfg.getvalues();
  char chrPlayerStatus;
  for (uint i = 0; i < CnfgValues.uintPlayersPerGen; ++i) {
    global::enty.newplayer();
    for (uint step = 0; step < CnfgValues.uintFirstGen; ++step) {
      chrPlayerStatus = global::enty.doPlayerStep(step, stageFirst);
      if (global::blnQuit) { return; }
      if (chrPlayerStatus != statusLiving) {
        if (CnfgValues.blnShowMap) { m_screen.playerDeath(); }
        step = CnfgValues.uintMaxPlayerSteps; //Forces for loop to stop
        global::mymap.restart();
      } else { if (CnfgValues.blnShowMap) { m_screen.update(); } }
    } //End for steps
    if (CnfgValues.blnShowMap) { m_screen.update(); }
  }//end for first gen
  global::enty.doNextGeneration(stageFirst);
}
/*****************************************************************************/
void clsCore::doGrowthGeneration() {
  /////////////////////////////////////////////////
  /// @brief Handles the growing generation stuff
  /////////////////////////////////////////////////

  Configures CnfgValues = global::cnfg.getvalues();
  char chrPlayerStatus;
  while (global::enty.uintGenSteps + CnfgValues.uintGenIncrease <=
         CnfgValues.uintMaxPlayerSteps) {
    for (uint i = 0; i < CnfgValues.uintPlayersPerGen; ++i) {
      global::enty.newplayer();
      for (uint step = 0; step <
          global::enty.uintGenSteps + CnfgValues.uintGenIncrease; ++step) {
        chrPlayerStatus = global::enty.doPlayerStep(step, stageGrowth);
        if (global::blnQuit) { return; }
        if (chrPlayerStatus != statusLiving) {
          if (CnfgValues.blnShowMap) { m_screen.playerDeath(); }
          step = CnfgValues.uintMaxPlayerSteps; //Forces for loop to stop
          global::mymap.restart();
        } else { if (CnfgValues.blnShowMap) { m_screen.update(); } }
      }//end for steps
      if (CnfgValues.blnShowMap) { m_screen.update(); }
    } //end for players
    global::enty.doNextGeneration(stageGrowth);
  }//end while loop
}
/*****************************************************************************/
void clsCore::doSteadyGernation() {
  /////////////////////////////////////////////////
  /// @brief Handles the steady generation stuff
  /////////////////////////////////////////////////

  Configures CnfgValues = global::cnfg.getvalues();
  char chrPlayerStatus;
  for (uint i = 0; i < CnfgValues.uintGensPastGrowth; ++i) {
    for (uint j = 0; j < CnfgValues.uintPlayersPerGen; ++j) {
      global::enty.newplayer();
      for (uint step = 0; step < CnfgValues.uintMaxPlayerSteps; ++step) {
        chrPlayerStatus = global::enty.doPlayerStep(step, stageSteady);
        if (global::blnQuit) { return; }
        if (chrPlayerStatus != statusLiving) {
          if (CnfgValues.blnShowMap) { m_screen.playerDeath(); }
          step = CnfgValues.uintMaxPlayerSteps; //Forces for loop to stop
          global::mymap.restart();
        } else { if (CnfgValues.blnShowMap) { m_screen.update(); } }
      }//end for steps
      if (CnfgValues.blnShowMap) { m_screen.update(); }
    } //end for players
    global::enty.doNextGeneration(stageSteady);
  }//end for gens loop
}
/*****************************************************************************/
void clsCore::doGame() {
  //Put all of the config values into CnfgValues for easier reference
  Configures CnfgValues;
  CnfgValues = global::cnfg.getvalues();
  if (!CnfgValues.blnHumanBuild) {

    if (!global::blnQuit) {
      doFirstGeneration();
      doGrowthGeneration();
      doSteadyGernation();
    } // end if quit

    if (global::blnDebugMode) { printf("Generations finished.\n"); }
  } else {
    //This is the human side of the game,
    SDL_Event event;
    char direction;
    char playerStatus;

    do {
      m_screen.update();
      global::enty.getFitness();
      if (SDL_PollEvent( &event )) {
        if (event.type == SDL_QUIT) {global::blnQuit = true;}
        else if (event.type == SDL_KEYDOWN) {
          //Key has been pressed figure out what to do
          switch (event.key.keysym.sym) {
          case SDLK_UP:
          case SDLK_w:
            direction = dirUp;
            break;
          case SDLK_DOWN:
          case SDLK_s:
            direction = dirDown;
            break;
          case SDLK_RIGHT:
          case SDLK_d:
            direction = dirRight;
            break;
          case SDLK_LEFT:
          case SDLK_a:
            direction = dirLeft;
            break;
          case SDLK_q:
          case SDLK_ESCAPE:
            global::blnQuit = true;
            break;
          case SDLK_r:
            global::mymap.restart();
            break;
          } //end switch
        } else if (event.type == SDL_KEYUP) {
          direction = dirNone;
        } //end if event
      } //end if event
      playerStatus = global::mymap.move(direction);
      if (playerStatus != statusLiving) {
        m_screen.playerDeath();
        global::mymap.restart();
      }
    } while (!global::blnQuit);
  } // end if human build
}
/*****************************************************************************/
