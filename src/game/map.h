#ifndef __MAP__HEADER__
#define __MAP__HEADER__
/*****************************************************************************/
#include <cstdio>
#include <math.h>
/*****************************************************************************/
#include "../globals.h"
/*****************************************************************************/
/** @image html tiles.png "Tile picture that is used" */
/*****************************************************************************/
/** @enum tile
  * Holds all the different tiles possible on the map. All values are hex. */

enum tile {
	tileSpace         = 0x0, /**< 0: Space / Empty Tile */
	tileBricksLarge   = 0x1, /**< 1: Large Brick tiles / default wall texture */
	tilePlayer        = 0x2, /**< 2: Player tile */
	tilePole          = 0x3, /**< 3: Pole tile */
	tileMonster       = 0x4, /**< 4: Monster tile */
	tileCoin          = 0x5, /**< 5: Coin tile */
	tileBricksSmall   = 0x6, /**< 6: Small Brick tile */
	tileBricksGray    = 0x7, /**< 7: Gray Brick tile */
	tileBricksGreen   = 0x8, /**< 8: Green Brick tile */
	tileBricksOrange  = 0x9, /**< 9: Orange Brick tile */
	tileBricksRed     = 0xA  /**< A: Red tile */
};
/*****************************************************************************/
/** @struct stcBox
  * Collision box */
struct stcBox {
  uint left;  /**< Left most edge of the collision box. In pixels from the left edge of the screen */
  uint right; /**< Right most edge of the collision box. In pixels from the left edge of the screen */
  uint top;   /**< Top most edge of the collision box. In pixels from the top edge of the screen */
  uint bottom;/**< Bottom most edge of the collision box. In pixels from the top edge of the screen */
};
/// @addtogroup TypeDefs
/// @{
typedef struct stcBox BOX;
/// @}
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsMap map.h "source/map.h"
/// @brief This class will hold all of the values and functions related to the
///        the map.
/////////////////////////////////////////////////


class clsMap {
  public:
    clsMap();
    ~clsMap();

    uchar numMonsters; /**< The number of monsters. */
    uchar basemap[defined::kMapHeight][defined::kMapWidth]; /**< This is the base map, the map is returned to this whenever clsMap::restart is called */
    uchar map[defined::kMapHeight][defined::kMapWidth]; /**< The CURRENT map. */
    BPLYR bplyBasePlayer; /**< The base player, which just holds the x and y values where they start. */
    VectorMonsters mstBaseMonsters; /**< The monsters on the map while restarting (see clsMap::load if this is confusing). */

    //Functions
    char move(uchar);
    void load(void);
    void restart(void);
    uchar getMapCell(uint,uint);
    void setMapCell(uint,uint,uchar);
    BPLYR getbasePlayer(void);
    bool checkOverlap(BOX,BOX);

  private:
    char checkCollision(LOC, uchar);
    bool inKillPlane(LOC);
};
/*****************************************************************************/
#endif
/* __MAP__HEADER__ */
