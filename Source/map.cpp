/*****************************************************************************/
#include <cstdio>
#include <cstdlib>
/*****************************************************************************/
#include "map.h"
#include "entity.h"
#include "config.h"
#include "tick.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file map.cpp
/// @brief Holds all of the functions for the Map Class
/////////////////////////////////////////////////
/*****************************************************************************/
clsMap::clsMap() {
  /////////////////////////////////////////////////
  /// @brief Default constructor.
  /////////////////////////////////////////////////

	if (global::blnDebugMode) { printf("Map Constructor called.\n"); }
}
/*****************************************************************************/
clsMap::~clsMap() {
  /////////////////////////////////////////////////
  /// @brief Default deconstructor, deletes mstBaseMonsters.
  /////////////////////////////////////////////////

	if(global::blnDebugMode) { printf("Map Destructor called.\n"); }
	mstBaseMonsters.clear();
}
/*****************************************************************************/
void clsMap::restart(void) {
  /////////////////////////////////////////////////
  /// @brief Resets the following:
  ///        * Map Tiles
  ///        * Clock
  ///        * Player Values
  ///        * Monster Values
  /// @return void
  /////////////////////////////////////////////////

	for (uint y = 0; y < defined::kMapHeight; ++y) {
		for (uint x = 0; x < defined::kMapWidth; ++x) {
			map[y][x] = basemap[y][x];
		}
	}
	if (global::blnDebugMode) { printf("Base map put into map.\n"); }

	global::tick.resetClock();
	if (global::blnDebugMode) {printf("Clock reset.\n");}

  for (uchar i = 0; i < numMonsters; ++i) {
    global::enty.setMonster(i,mstBaseMonsters[i]);
    if (global::blnDebugMode) { printf("Finished Monster %d.\n",i); }
  }
  if (global::blnDebugMode) { printf("Base monsters placed.\n"); }

	global::enty.setPlayer(bplyBasePlayer);
	if (global::blnDebugMode) { printf("Player reset.\n"); }
}
/*****************************************************************************/
char clsMap::move(uchar direction) {
  /////////////////////////////////////////////////
  /// @brief Will move the player a direction specified, checking for collisions
  ///        and monsters and all that. (Its a long function)
  /// @param direction = Direction to move the player. See dir
  /// @return Player Status. See status
  /////////////////////////////////////////////////

	static uchar jumpcount; //how many times the player has jumped
	static bool playerfalling; //if the player is falling
	float deltat = 1.0 / defined::kGoalFPS;
	BPLYR tempEntity; //keeps track of x, y, and velocity.
	if ( !playerfalling ) { jumpcount = 0; }
  bool blnLoop = true;

	//Move monsters first, to see if player dies and we can then skip the rest
	MNSTR tempMonster;

	for (uchar i = 0; i < numMonsters; ++i) {
		tempMonster = global::enty.getMonster(i);
		if (tempMonster.living) {
			tempEntity.location = tempMonster.location;
			tempEntity.vel = tempMonster.vel;

      //Decrease velocities by friction
      tempEntity.vel.x -= tempEntity.vel.x * global::physics::fFriction;
      tempEntity.vel.y -= tempEntity.vel.y * global::physics::fFriction;

			if (tempMonster.state == stateRight) {
        tempEntity.vel.x += global::physics::fIncVelocity;
        if (abs(tempEntity.vel.x) > global::physics::fMonsMaxVelocity )
          { tempEntity.vel.x = global::physics::fMonsMaxVelocity; }
      } else if (tempMonster.state == stateLeft) {
        tempEntity.vel.x -= global::physics::fIncVelocity;
        if (abs(tempEntity.vel.x) > global::physics::fMonsMaxVelocity )
          { tempEntity.vel.x = -1 * global::physics::fMonsMaxVelocity; }
      }

      //Apply Gravity
      tempEntity.vel.y += global::physics::fGravityAcceleration;
      if (tempEntity.vel.y > global::physics::fMonsMaxVelocity)
        { tempEntity.vel.y = global::physics::fMonsMaxVelocity; }

      //Update location based on delta t
      tempEntity.location.x += round(tempEntity.vel.x * deltat);
      tempEntity.location.y += round(tempEntity.vel.y * deltat);

      //Check for Collisions
      do {
        switch ( checkCollision(tempEntity.location, tileMonster) ) {
        case tileCoin:
          //Replace the coin tile with space
          map[(uint)round(tempMonster.location.y / defined::kPicSize)]
             [(uint)round(tempMonster.location.x / defined::kPicSize)] =
              tileSpace;
        case tilePole:
        case tileSpace:
          tempMonster.location = tempEntity.location;
          tempMonster.vel = tempEntity.vel;
          blnLoop = false;
          break;
        case tilePlayer:
          //Monster runs into player, kill the player
          blnLoop = false;
          return deathMonster;
          break;
        case tileBricksGray:
        case tileBricksGreen:
        case tileBricksLarge:
        case tileBricksOrange:
        case tileBricksRed:
        case tileBricksSmall:
        default:
          //Collision with wall, first reset y
          if (tempEntity.location.y != tempMonster.location.y) {
            //Change y and try again.
            tempEntity.location.y = tempMonster.location.y;
            tempEntity.vel.y = 0;
            blnLoop = true;
          } else if (tempEntity.location.x != tempMonster.location.x) {
            //Change x and try again.
            //recoil off the wall as well
            tempEntity.location.x = tempMonster.location.x;
            tempEntity.vel.x *= global::physics::fRecoil;
            if (tempMonster.state == stateLeft) {
             tempMonster.state = stateRight;
            } else { tempMonster.state = stateLeft; }
            blnLoop = true;
          } else {
            tempMonster.living = false;
            blnLoop = false;
          }
        } //end switch check Collision
      } while (blnLoop);
      //do a check if monster is in the kill planes
      if ( inKillPlane(tempMonster.location) ) { tempMonster.living = false; }

      global::enty.setMonster(i,tempMonster);
    } // end if living
  }//End of for monsters

	//Now the player can move.
	PLYR tempPlayer;
	tempPlayer = global::enty.getPlayer();
  //Kill Player if their fitness gets too low in hard mode
  if (tempPlayer.fitness < -2.5 && (global::cnfg.getvalues(cnfgHardMode) == 1) )
    { return deathDecay; }

  tempEntity.location = tempPlayer.location;
  tempEntity.vel = tempPlayer.vel;

  //Decrease velocities by friction
  tempEntity.vel.x -= tempEntity.vel.x * global::physics::fFriction;
  tempEntity.vel.y -= tempEntity.vel.y * global::physics::fFriction;

	switch (direction) {
  case dirLeft :
    tempPlayer.state = stateLeft;
    tempEntity.vel.x -= global::physics::fIncVelocity;
    if (abs(tempEntity.vel.x) > global::physics::fMaxVelocity )
      { tempEntity.vel.x = -1 * global::physics::fMaxVelocity; }
    break;
  case dirRight :
    tempPlayer.state = stateRight;
    tempEntity.vel.x += global::physics::fIncVelocity;
    if (tempEntity.vel.x > global::physics::fMaxVelocity )
      { tempEntity.vel.x = global::physics::fMaxVelocity; }
    break;
  case dirUp :
    if (jumpcount < defined::kMaxJumpCount) {
      tempPlayer.state = stateJump;
      jumpcount++;
      playerfalling = true;
      tempEntity.vel.y -= 1.325 * global::physics::fMaxVelocity;
    } //end if less than jump count
    break;
  case dirDown :
    tempPlayer.state = stateDuck;
    tempEntity.vel.y += global::physics::fMaxVelocity;
    break;
  case dirNone:
  default:
    tempPlayer.state = stateRest;
    break;
	}; // end switch case

  //Apply Gravity
  tempEntity.vel.y += global::physics::fGravityAcceleration * deltat;
  if (tempEntity.vel.y > global::physics::fMaxVelocity)
    { tempEntity.vel.y = global::physics::fMaxVelocity; }

  //Update location based on delta t
  tempEntity.location.x += round(tempEntity.vel.x * deltat);
  tempEntity.location.y += round(tempEntity.vel.y * deltat);

	blnLoop = true;
	//Check collisions
	do {
    switch ( checkCollision(tempEntity.location, tilePlayer) ) {
    case tileCoin:
      //Replace the coin tile with space
      map[(uint)(tempEntity.location.y / defined::kPicSize)]
         [(uint)(tempEntity.location.x / defined::kPicSize)] = tileSpace;
      tempPlayer.score += defined::kCoinWorth;
    case tilePole:
    case tileSpace:
      tempPlayer.location = tempEntity.location;
      tempPlayer.vel = tempEntity.vel;
      if (tempEntity.vel.y > 0) { playerfalling = true; }
      blnLoop = false;
      break;
    case tileMonster:
      //Player is running into monster
      if (playerfalling) { //kill monster if falling on it
        global::enty.killMonster(tempEntity.location);
        tempPlayer.score += defined::kMonsterKillWorth;
      } else { return deathMonster; }
      blnLoop = false;
      break;
    case tileBricksGray:
    case tileBricksGreen:
    case tileBricksLarge:
    case tileBricksOrange:
    case tileBricksRed:
    case tileBricksSmall:
    default:
      //Collision with wall first reset y
      if (tempEntity.location.y != tempPlayer.location.y) {
        //Change y and try again.
        tempEntity.location.y = tempPlayer.location.y;
        tempEntity.vel.y *= global::physics::fRecoil;
        playerfalling = false;
        jumpcount = 0;
        blnLoop = true;
      } else if (tempEntity.location.x != tempPlayer.location.x) {
        //Change x and try again.
        //recoil off the wall as well
        tempEntity.location.x = tempPlayer.location.x;
        tempEntity.vel.x *= global::physics::fRecoil;
        blnLoop = true;
      } else {
        tempEntity.vel.y *= global::physics::fRecoil;
        tempEntity.vel.x *= global::physics::fRecoil;
        blnLoop = false;
      }
    } //end switch check Collision
	} while (blnLoop);

  //do a check if player is in the kill planes
  if ( inKillPlane(tempPlayer.location) ) { return deathFall; }

  BPLYR temp;
  temp.location = tempPlayer.location;
  temp.vel = tempPlayer.vel;
  //Check for stupid death (going too far left on hard mode)
  if (tempPlayer.location.x <
      bplyBasePlayer.location.x - 2 * defined::kPicSize
      && (global::cnfg.getvalues(cnfgHardMode) == 1)) {return deathStupid;}
  global::enty.setPlayer(temp);

	//Reduce the clock time and check if it equals 0 and kill the player if it does.
	global::tick.decClock();
	if ( global::tick.getClockTime() == 0 ) { return deathClock; }

	return statusLiving;
}
/*****************************************************************************/
void clsMap::load(void) {
  /////////////////////////////////////////////////
  /// @brief Will load map from file (if it exists) and place that into base map.
  ///        Then it will find the starting places for all the monsters and the player,
  ///        and set baseplayer and basemosnters.
  /// @return void
  /////////////////////////////////////////////////

  /** @todo (GamerMan7799#4#): Dynamically allocate map size */
  //Finds player and monster on the map, and place them in base stats used
  //when restarting the map.

uchar tempmap[defined::kMapHeight][defined::kMapWidth] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,5,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,5,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,5,1},
{1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,5,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,5,5,5,0,0,0,5,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,5,1},
{1,0,0,0,0,0,5,5,5,5,5,5,5,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,5,1},
{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,5,1},
{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,5,1},
{1,1,1,1,1,2,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,4,0,0,1,1,1,1,0,0,0,0,0,0,0,4,0,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,4,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,1,5,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,5,1,1,0,0,3,0,0,0,0,0,0,0,5,1,0,0,5,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

  FILE * mapload = fopen("map.sav", "r");
  if (mapload == nullptr) {
    printf("Could not find map save, default will be used.\n");
    for (uint y = 0; y < defined::kMapHeight; ++y) {
      for (uint x = 0; x < defined::kMapWidth; ++x) {
        basemap[y][x] = tempmap[y][x];
      } //end for x
    } //end for y
  } else {
    printf("Map Found, will load.\n");
    for (uint y = 0; y < defined::kMapHeight; ++y) {
      for (uint x = 0; x < defined::kMapWidth; ++x) {
        fscanf( mapload, "%2x", &basemap[y][x] );
      } //end for x
    } //end for y
  } //end file exists

	numMonsters = 0;
	MNSTR tempmonster;
  tempmonster.vel = {0,0};
  tempmonster.living = true;
  tempmonster.state = stateRight;

	for (uint y = 0; y < defined::kMapHeight; ++y) {
		for (uint x = 0; x < defined::kMapWidth; ++x) {
			if (basemap[y][x] == tilePlayer) {
				if (global::blnDebugMode) { printf("Found Player at (%d,%d).\n",x,y); }
				bplyBasePlayer.location =
            {x * defined::kPicSize, y * defined::kPicSize};
				bplyBasePlayer.vel = {0,0};
				basemap[y][x] = tileSpace; //Once we find the player they are going
                                   //to be moving in a velocity based pattern
                                   //so delete that tile
			} else if (basemap[y][x] == tileMonster) {
        basemap[y][x] = tileSpace;
				numMonsters++;
				if (global::blnDebugMode) { printf("Found Monster at (%d,%d)\n",x,y); }
				tempmonster.location = {x * defined::kPicSize, y * defined::kPicSize};
        mstBaseMonsters.push_back(tempmonster);
			} //end if monster tile
		} //end for width
	} //end for height
	if (global::blnDebugMode) {printf("Found %d Monsters.\n",numMonsters);}

	global::enty.allocateMonsters(numMonsters);
	if (global::blnError) {printf("Could not allocate the memory!\n"); return;}
}
/*****************************************************************************/
uchar clsMap::getMapCell(uint x, uint y) {
  /////////////////////////////////////////////////
  /// @brief Gets Map tile.
  /// @param x = X spot in array.
  /// @param y = Y spot in array.
  /// @return tile
  /////////////////////////////////////////////////

	return map[y][x];
}
/*****************************************************************************/
void clsMap::setMapCell(uint x, uint y, uchar tile) {
  /////////////////////////////////////////////////
  /// @brief Sets Map tile.
  /// @param x = X spot in array.
  /// @param y = Y spot in array.
  /// @param tile = tile to set it to.
  /// @return void
  /////////////////////////////////////////////////

	map[y][x] = tile;
}
/*****************************************************************************/
BPLYR clsMap::getbasePlayer(void) {
  /////////////////////////////////////////////////
  /// @brief Returns bplyBasePlayer
  /// @return bplyBasePlayer
  /////////////////////////////////////////////////

	return bplyBasePlayer;
}
/*****************************************************************************/
char clsMap::checkCollision(LOC placement, uchar tile) {
  /////////////////////////////////////////////////
  /// @brief Will check if Monster / Player is colliding with anything.
  /// @param placement = Monster / Player's location
  /// @param tile = This is used because the collision checking between the monsters
  ///               and the players is slightly different.
  /// @return They tile they are colliding with (or space if not colliding).
  /////////////////////////////////////////////////

  //First convert pixel

  BOX A;
  BOX B;

  A.left = placement.x;
  A.right = placement.x + defined::kPicSize;
  A.top = placement.y;
  A.bottom = placement.y + defined::kPicSize;

  if (tile == tilePlayer) { //shrink the player hitbox
    A.left += 4;
    A.right -= 2;
    //A.top += 3;
    A.bottom -= 3;
  }

  //Check if collision with a player (if monster)
  if (tile == tileMonster) {
    PLYR tempPlayer = global::enty.getPlayer();
    B.left = tempPlayer.location.x + 4;
    B.right = B.left + defined::kPicSize - 2;
    B.top = tempPlayer.location.y;
    B.bottom = B.top + defined::kPicSize - 3;

    if ( checkOverlap(A, B) ) {
      if (global::blnDebugMode) { printf("Monster collided with player.\n"); }
      return tilePlayer;
    } //end if overlap
  } //end if monster

  //Check if collision with a monster (if player)
  if (tile == tilePlayer) {
    MNSTR tempMonster;
    for (uchar i = 0; i < numMonsters; ++i) {
      tempMonster = global::enty.getMonster(i);
      if(tempMonster.living) {
        B.left = tempMonster.location.x;
        B.right = B.left + defined::kPicSize;
        B.top = tempMonster.location.y;
        B.bottom = B.top + defined::kPicSize;

        if ( checkOverlap(A, B) ) {
          if (global::blnDebugMode) { printf("Player collided with monster.\n"); }
          return tileMonster;
        } //end if overlap
      } //end if living
    } //end for monster
  } //end if player


/** \todo (GamerMan7799#1#): Make it so it doesn't have to check the entire map. */
  for (uint y = 0; y < defined::kMapHeight; ++y ) {
    for (uint x = 0; x < defined::kMapWidth; ++x ) {
      if (map[y][x] != tileSpace) {
        B.left = x * defined::kPicSize;
        B.right = B.left + defined::kPicSize;
        B.top = y * defined::kPicSize;
        B.bottom = B.top + defined::kPicSize;
        if ( checkOverlap(A, B) ) { return map[y][x]; }
      } //end if not space
    } //end for x
  } //end for y
  return tileSpace; //return a space, for no collision
}
/*****************************************************************************/
bool clsMap::checkOverlap(BOX A, BOX B) {
  /////////////////////////////////////////////////
  /// @brief Checks if two collision boxes are overlapping.
  /// @param A = Collision box for object A
  /// @param B = Collision box for object B
  /// @return TURE / FALSE if they overlap.
  /////////////////////////////////////////////////

  if( A.bottom <= B.top ){ return false; }
  if( A.top >= B.bottom ){ return false; }
  if( A.right <= B.left ){ return false; }
  if( A.left >= B.right ){ return false; }
  //If none of the sides from A are outside B
  return true;
}
/*****************************************************************************/
bool clsMap::inKillPlane(LOC place) {
  /////////////////////////////////////////////////
  /// @brief Checks if location is outside range and in "kill plane"
  /// @param place = location to check
  /// @return TURE / FALSE if in kill plane
  /////////////////////////////////////////////////

  if (place.y < 0) { return true; }
  if (place.x < 0) { return true; }
  if (place.y > defined::kMapHeight * defined::kPicSize) { return true; }
  if (place.x > defined::kMapWidth * defined::kPicSize) { return true; }

  return false;
}
/*****************************************************************************/
