/**********************************************************************************************************************************************/
#include "map.h"
#include "entity.h"
#include "config.h"
#include "tick.h"
#include "globals.h"
/**********************************************************************************************************************************************/
clsMap::clsMap() {
	if (Global::blnDebugMode) {printf("Map Constructor called.\n");}
}
/**********************************************************************************************************************************************/
clsMap::~clsMap() {
	if(Global::blnDebugMode) {printf("Map Destructor called.\n");}
	delete[] pmstBaseMonsters;
}
/**********************************************************************************************************************************************/
void clsMap::restart(void) {
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y ++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
			map[y][x] = basemap[y][x];
		}
	}
	if (Global::blnDebugMode) {printf("Base map put into map.\n");}

	Global::Tick.resetClock();
	if (Global::blnDebugMode) {printf("Clock reset.\n");}

	MNSTR tempMonster;

	if (pmstBaseMonsters != nullptr) {
		for (uchar i = 0; i < numMonsters; i++) {
			tempMonster.location.x = pmstBaseMonsters[i].location.x;
			tempMonster.location.y = pmstBaseMonsters[i].location.y;
			tempMonster.vel.x = pmstBaseMonsters[i].vel.x;
			tempMonster.vel.y = pmstBaseMonsters[i].vel.y;
			tempMonster.living = pmstBaseMonsters[i].living;
			tempMonster.state = pmstBaseMonsters[i].state;
			Global::Enty.setMonster(i,tempMonster);
			if (Global::blnDebugMode) {printf("Finished Monster %d.\n",i);}
		}
		if (Global::blnDebugMode) {printf("Base monsters placed.\n");}
	} else {if (Global::blnDebugMode) {printf("Base monsters equals nullptr.\n");}}
	Global::Enty.setPlayer(bplyBasePlayer);
	if (Global::blnDebugMode) {printf("Player reset.\n");}
}
/**********************************************************************************************************************************************/
char clsMap::move(uchar direction) {
	static uchar jumpcount; //how many times the player has jumped
	static bool playerfalling; //if the player is falling
	float deltat = 1.0 / DEFINED_GOAL_FPS;
	BPLYR tempEntity; //keeps track of x, y, and velocity.
	if (playerfalling == false) {jumpcount = 0;}

	float GravityVelDec = Global::Physics::fGravityAcceleration * deltat;
    bool blnLoop = true;
	//Move monsters first, to see if player dies and we can then skip the rest
	MNSTR tempMonster;

	for (uchar i = 0; i < numMonsters; i++) {
		tempMonster = Global::Enty.getMonster(i);
		if (tempMonster.living) {
			tempEntity.location.x = tempMonster.location.x;
			tempEntity.location.y = tempMonster.location.y;
			tempEntity.vel.x = tempMonster.vel.x;
			tempEntity.vel.y = tempMonster.vel.y;

            //Decrease velocities by friction
            tempEntity.vel.x -= tempEntity.vel.x * Global::Physics::fFriction;
            tempEntity.vel.y -= tempEntity.vel.y * Global::Physics::fFriction;

			if (tempMonster.state == stateRight) {
                if (tempEntity.vel.x + Global::Physics::fIncVelocity < Global::Physics::fMaxVelocity) {tempEntity.vel.x += Global::Physics::fIncVelocity;}
                else {tempEntity.vel.x = Global::Physics::fMaxVelocity;}
            } else if (tempMonster.state == stateLeft) {
                if ( abs(tempEntity.vel.x - Global::Physics::fIncVelocity) < Global::Physics::fMaxVelocity) {tempEntity.vel.x -= Global::Physics::fIncVelocity;}
                else {tempEntity.vel.x = -1.00 * Global::Physics::fMaxVelocity;}
            }

            //Apply Gravity
            if (tempEntity.vel.y + GravityVelDec < Global::Physics::fMaxVelocity) {tempEntity.vel.y += GravityVelDec;}
            else {tempEntity.vel.y = Global::Physics::fMaxVelocity;}

            //Update location based on delta t
            tempEntity.location.x += round(tempEntity.vel.x * deltat);
            tempEntity.location.y += round(tempEntity.vel.y * deltat);

            //Check for Collisions
            do {
                switch ( checkCollision(tempEntity.location, tileMonster) ) {
                case tileCoin:
                    //Replace the coin tile with space
                    map[(uint)(tempMonster.location.y / DEFINED_PIC_SIZE)][(uint)(tempMonster.location.x / DEFINED_PIC_SIZE)] = tileSpace;
                case tilePole:
                case tileSpace:
                    tempMonster.location.x = tempEntity.location.x;
                    tempMonster.location.y = tempEntity.location.y;
                    tempMonster.vel.x = tempEntity.vel.x;
                    tempMonster.vel.y = tempEntity.vel.y;
                    blnLoop = false;
                    break;
                case tilePlayer:
                    //Monster runs into player, kill the player
                    blnLoop = false;
                    return deathMonster;
                    break;
                case tileWall:
                default:
                    //Collision with wall first reset y
                    if (tempEntity.location.y != tempMonster.location.y) {
                        //Change y and try again.
                        tempEntity.location.y = tempMonster.location.y;
                        tempEntity.vel.y = 0;
                        blnLoop = true;
                    } else if (tempEntity.location.x != tempMonster.location.x) {
                        //Change x and try again.
                        //recoil off the wall as well
                        tempEntity.location.x = tempMonster.location.x;
                        tempEntity.vel.x *= Global::Physics::fRecoil;
                        blnLoop = true;
                    } else { tempMonster.living = false; blnLoop = false; }
                } //end switch check Collision
            } while (blnLoop);
            //do a check if monster is in the kill planes
            if (tempMonster.location.y < 0 || tempMonster.location.x < 0 || tempMonster.location.y > DEFINED_MAP_HEIGHT * DEFINED_PIC_SIZE || tempMonster.location.x > DEFINED_MAP_WIDTH * DEFINED_PIC_SIZE) {
                tempMonster.living = false;
            }

            Global::Enty.setMonster(i,tempMonster);
		} // end if living
    }//End of for monsters


	//Now the player can move.

	PLYR tempPlayer;
	tempPlayer = Global::Enty.getPlayer();
    //Kill Player if their fitness gets too low in hard mode
    if (tempPlayer.fitness < -2.5 && (Global::Cnfg.getvalues(cnfgHardMode) == 1) ) {return deathDecay;}

    tempEntity.location = tempPlayer.location;
    tempEntity.vel = tempPlayer.vel;

    //Decrease velocities by friction
    tempEntity.vel.x -= tempEntity.vel.x * Global::Physics::fFriction;
    tempEntity.vel.y -= tempEntity.vel.y * Global::Physics::fFriction;

	switch (direction) {
		case dirLeft :
		    tempPlayer.state = stateLeft;
		    if ( abs(tempEntity.vel.x - Global::Physics::fIncVelocity) < Global::Physics::fMaxVelocity) {tempEntity.vel.x -= Global::Physics::fIncVelocity;}
		    else {tempEntity.vel.x = Global::Physics::fMaxVelocity;}
			break;
		case dirRight :
		    tempPlayer.state = stateRight;
            if ( tempEntity.vel.x + Global::Physics::fIncVelocity < Global::Physics::fMaxVelocity) {tempEntity.vel.x += Global::Physics::fIncVelocity;}
		    else {tempEntity.vel.x = Global::Physics::fMaxVelocity;}
			break;
		case dirUp :
		    if (jumpcount < DEFINED_MAX_JUMP_COUNT) {
                tempPlayer.state = stateJump;
                jumpcount++;
                if ( abs(tempEntity.vel.y - Global::Physics::fIncVelocity) < Global::Physics::fMaxVelocity) {tempEntity.vel.y -= Global::Physics::fIncVelocity;}
                else {tempEntity.vel.y = Global::Physics::fMaxVelocity;}
		    } //end if less than jump count
			break;
        case dirDown :
            tempPlayer.state = stateDuck;
            break;
        case dirNone:
        default:
            tempPlayer.state = stateRest;
            break;
	};

    //Apply Gravity
    if (tempEntity.vel.y + GravityVelDec < Global::Physics::fMaxVelocity) {tempEntity.vel.y += GravityVelDec;}
    else {tempEntity.vel.y = Global::Physics::fMaxVelocity;}

    //Update location based on delta t
    tempEntity.location.x += round(tempEntity.vel.x * deltat);
    tempEntity.location.y += round(tempEntity.vel.y * deltat);

	blnLoop = true;
	//Check collisions
	do {
        switch ( checkCollision(tempEntity.location, tilePlayer) ) {
        case tileCoin:
            //Replace the coin tile with space
            map[(uint)(tempMonster.location.y / DEFINED_PIC_SIZE)][(uint)(tempMonster.location.x / DEFINED_PIC_SIZE)] = tileSpace;
            tempPlayer.score += DEFINED_COIN_WORTH;
        case tilePole:
        case tileSpace:
            tempPlayer.location.x = tempEntity.location.x;
            tempPlayer.location.y = tempEntity.location.y;
            tempPlayer.vel.x = tempEntity.vel.x;
            tempPlayer.vel.y = tempEntity.vel.y;
            if (tempEntity.vel.y > 0) {playerfalling = true;}
            blnLoop = false;
            break;
        case tileMonster:
            //Player is running into monster
            if (tempEntity.vel.y > 0) { //kill monster if falling on it
                Global::Enty.killMonster(tempEntity.location.x, tempEntity.location.y);
                tempPlayer.score += DEFINED_MONS_KILL_POINTS;
            } else { return deathMonster;}
            blnLoop = true;
            break;
        case tileWall:
        default:
            //Collision with wall first reset y
            if (tempEntity.location.y != tempPlayer.location.y) {
                //Change y and try again.
                tempEntity.location.y = tempPlayer.location.y;
                tempEntity.vel.y = 0.0;
                playerfalling = false;
                blnLoop = true;
            } else if (tempEntity.location.x != tempPlayer.location.x) {
                //Change x and try again.
                //recoil off the wall as well
                tempEntity.location.x = tempPlayer.location.x;
                tempEntity.vel.x *= Global::Physics::fRecoil;
                blnLoop = true;
            } else { blnLoop = false; return deathError; }
        } //end switch check Collision
	} while (blnLoop);

    //do a check if player is in the kill planes
    if (tempPlayer.location.y > DEFINED_MAP_HEIGHT * DEFINED_PIC_SIZE) {
        return deathFall;
    }

    BPLYR temp;
    temp.location = tempPlayer.location;
    temp.vel = tempPlayer.vel;
    //Check for stupid death (going too far left on hard mode)
    if (tempPlayer.location.x < bplyBasePlayer.location.x - 2 * DEFINED_PIC_SIZE && (Global::Cnfg.getvalues(cnfgHardMode) == 1)) {return deathStupid;}
    Global::Enty.setPlayer(temp);

	//Reduce the clock time and check if it equals 0 and kill the player if it does.
	Global::Tick.decClock();
	if (Global::Tick.getClockTime() == 0) {return deathClock;}

	return statusLiving;
}
/**********************************************************************************************************************************************/
void clsMap::load(void) {
	//Finds player and monster on the map, and place them in base stats used
	//when restarting the map.

uchar tempmap[DEFINED_MAP_HEIGHT][DEFINED_MAP_WIDTH] = {{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,5,5,5,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,5,5,5,5,5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,0},
{0,0,0,0,0,0,5,5,5,5,5,5,5,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,1,1,0,0,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,0},
{0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,3,0,0,0,0,1,1,1,1,1,0,0,0},
{1,1,1,1,1,2,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,4,0,0,1,1,1,1,0,0,0,0,0,0,0,4,0,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,3,0,0,0,1,1,1,1,1,1,0,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

    //set base map (I couldn't do it as a const anymore because I'm trying to modify it)

    /* TODO (xPUREx#5#): Look into vectors for basemonsters */
	numMonsters = 0;
	for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++) {
		for (uint x = 0; x < DEFINED_MAP_WIDTH; x++) {
            basemap[y][x] = tempmap[y][x];
			if (basemap[y][x] == tilePlayer) {
				if (Global::blnDebugMode) {printf("Found Player at (%d,%d).\n",x,y);}
				bplyBasePlayer.location.x = x * DEFINED_PIC_SIZE;
				bplyBasePlayer.location.y = y * DEFINED_PIC_SIZE;
				bplyBasePlayer.vel.x = 0;
				bplyBasePlayer.vel.y = 0;
				basemap[y][x] = tileSpace; //Once we find the player they are going
                                           //to be moving in a velocity based pattern
                                           //so delete that tile
			}
			else if (basemap[y][x] == tileMonster) {
                basemap[y][x] = tileSpace;
				numMonsters++;
				if (Global::blnDebugMode) {printf("Found Monster at (%d,%d)\n",x,y);}
				if (numMonsters == 1) {
					pmstBaseMonsters = new (std::nothrow) MNSTR[1];
					if (pmstBaseMonsters == nullptr) {
						//Could not allocate the memory
						Global::blnError = true;
						return;
					}
					pmstBaseMonsters[0].location.x = x * DEFINED_PIC_SIZE;
					pmstBaseMonsters[0].location.y = y * DEFINED_PIC_SIZE;
                    pmstBaseMonsters[0].vel.x = x;
                    pmstBaseMonsters[0].vel.y = y;
					pmstBaseMonsters[0].living = true;
					pmstBaseMonsters[0].state = stateLeft;
				} else {
					//Allocate a temp array to hold the old array + the new monster
					MNSTR* pTemp = new (std::nothrow) MNSTR[numMonsters];
					if (pTemp == nullptr) {
						//Could not allocate the memory
						Global::blnError = true;
						return;
					}

					for (uchar i = 0; i < numMonsters - 1; i++) {
                        /* TODO (GamerMan7799#5#): Consider using std::copy from algorithm library
                            Do not worry about if xPUREx converts to vectors.*/
						//std::copy(pmstBaseMonsters, pmstBaseMonsters + numMonsters - 1, pTemp);
						pTemp[i].location.x = pmstBaseMonsters[i].location.x;
						pTemp[i].location.y = pmstBaseMonsters[i].location.y;
						pTemp[i].vel.x = pmstBaseMonsters[i].vel.x;
                        pTemp[i].vel.y = pmstBaseMonsters[i].vel.y;
						pTemp[i].living = pmstBaseMonsters[i].living;
						pTemp[i].state = pmstBaseMonsters[i].state;
					}
					//Delete old array as it isn't needed.
					delete[] pmstBaseMonsters;

					//Place newest Monster values into temp array.
					pTemp[numMonsters - 1].location.x = x * DEFINED_PIC_SIZE;
					pTemp[numMonsters - 1].location.y = y * DEFINED_PIC_SIZE;
                    pTemp[numMonsters - 1].vel.x = 0;
                    pTemp[numMonsters - 1].vel.y = 0;
					pTemp[numMonsters - 1].living = true;
					pTemp[numMonsters - 1].state = stateLeft;

					//Generate new Base Monster array
					pmstBaseMonsters = new (std::nothrow) MNSTR[numMonsters];
					if (pmstBaseMonsters == nullptr) {
						//Could not allocate the memory
						Global::blnError = true;
						return;
					}

					for (uchar i = 0; i < numMonsters; i++) {
						//Copy Temp Array into new array.
						pmstBaseMonsters[i].location.x = pTemp[i].location.x;
						pmstBaseMonsters[i].location.y = pTemp[i].location.y;
						pmstBaseMonsters[i].vel.x = pTemp[i].vel.x;
                        pmstBaseMonsters[i].vel.y = pTemp[i].vel.y;
						pmstBaseMonsters[i].living = pTemp[i].living;
						pmstBaseMonsters[i].state = pTemp[i].state;
					}

					//Delete Temp Array
					delete [] pTemp;

				} //end if first or not
			} //end if monster tile
		} //end for width
	} //end for height
	if (Global::blnDebugMode) {printf("Found %d Monsters.\n",numMonsters);}

	Global::Enty.allocateMonsters(numMonsters);
	if (Global::blnError) {printf("Could not allocate the memory!\n"); return;}
}
/**********************************************************************************************************************************************/
uchar clsMap::getMapCell(uint x, uint y) {
	return map[y][x];
}
/**********************************************************************************************************************************************/
void clsMap::setMapCell(uint x, uint y, uchar tile) {
	map[y][x] = tile;
}
/**********************************************************************************************************************************************/
BPLYR clsMap::getbasePlayer(void) {
	return bplyBasePlayer;
}
/**********************************************************************************************************************************************/
char clsMap::checkCollision(LOC placement, uchar tile) {
    //First convert pixel

    BOX A;
    BOX B;

    A.left = placement.x;
    A.right = placement.x + DEFINED_PIC_SIZE;
    A.top = placement.y;
    A.bottom = placement.y + DEFINED_PIC_SIZE;

    //Check if collision with a player (if monster)
    if (tile == tileMonster) {
        PLYR tempPlayer = Global::Enty.getPlayer();
        B.left = tempPlayer.location.x;
        B.right = B.left + DEFINED_PIC_SIZE;
        B.top = tempPlayer.location.y;
        B.bottom = B.top + DEFINED_PIC_SIZE;

        if ( checkOverlap(A, B) ) {
            if (Global::blnDebugMode) {printf("Monster collided with player.\n");}
            return tilePlayer;
        } //end if overlap
    } //end if monster

    //Check if collision with a monster (if player)
    if (tile == tilePlayer) {
        MNSTR tempMonster;
        for (uchar i = 0; i < numMonsters; i++) {
            tempMonster = Global::Enty.getMonster(i);
            B.left = tempMonster.location.x;
            B.right = B.left + DEFINED_PIC_SIZE;
            B.top = tempMonster.location.y;
            B.bottom = B.top + DEFINED_PIC_SIZE;

            if ( checkOverlap(A, B) ) {
                if (Global::blnDebugMode) {printf("Player collided with monster.\n");}
                return tileMonster;
            } //end if overlap
        } //end for monster
    } //end if player


/* TODO (GamerMan7799#1#): Make it so it doesn't have to check the entire map. */
    for (uint y = 0; y < DEFINED_MAP_HEIGHT; y++ ) {
        for (uint x = 0; x < DEFINED_MAP_WIDTH; x++ ) {
            if (map[y][x] != tileSpace) {
                B.left = x * DEFINED_PIC_SIZE;
                B.right = B.left + DEFINED_PIC_SIZE;
                B.top = y * DEFINED_PIC_SIZE;
                B.bottom = B.top + DEFINED_PIC_SIZE;
                if ( checkOverlap(A, B) ) {
                    if (Global::blnDebugMode) {printf("Map collision found.\n");}
                    return map[y][x];
                }
            } //end if not space
        } //end for x
    } //end for y
    return tileSpace; //return a space, for no collision
}
/**********************************************************************************************************************************************/
bool clsMap::checkOverlap(BOX A, BOX B) {
    if ( (A.right >= B.left && A.right <= B.right) || (A.left <= B.right && A.left >= B.left) ) { //X's cross now check y's
        if ( (A.bottom < B.bottom && A.bottom > B.top) || (A.top > B.bottom && A.top < B.top) ) {
            //if (Global::blnDebugMode) {printf("Overlap found.\n");}
            return true;
        } //end collide y
    } //end collide x
    return false;
}
/**********************************************************************************************************************************************/
