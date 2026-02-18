/**
 * @file maps.hpp
 * @author Martina Nazzareni (0001223089)
 * @brief map and tile definitions used to build levels
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines the tile format used by the map builder
 * a map is a fixed grid of tiles that describes
 * walls, spawn points, gates, and enemy spawns
 * 
 * mapbuilder provides a set of hard coded maps used by the game
 */

#ifndef MAPS_HPP
#define MAPS_HPP

#define MAP_HEIGHT 22
#define MAP_WIDTH 27

/**
 * @brief tile type used inside the map grid
 *
 * values describe both static 
 * tiles (walls, gates, spawn) and enemy spawn markers
 */
enum _TileType {
    EMPTY,
    SOLID,
    DESTRUCTIBLE,
    SPAWN,
    WALKER,
    PATROLLER,
    CHASER,
    GATE_NEXT,
    GATE_PREV
};

/**
 * @brief a single map tile with optional parameters
 *
 * tiles can store extra data to configure enemies
 * 
 * - for patroller: sx,sy is the start target 
 * and ex,ey is the end target, speed is movement speed
 * 
 * - for chaser or walker: speed can be 
 * used as movement speed
 *
 * coordinates x,y are not stored in the tile 
 * because they are the indices in the map grid
 */
class _Tile {
protected:
    // the tile type
    _TileType type = _TileType::EMPTY;

    //optional speed value (for enemies)
    unsigned short speed = 0;

    //optional start and end values
    //for patrollers

    unsigned short sx = 0, sy = 0 ;
    unsigned short ex = 0, ey = 0;

public:
    /**
     * @brief creates an empty tile
     */
    _Tile();

    /**
     * @brief creates a tile with a given type
     * @param type tile type
     */
    _Tile(_TileType type);

    /**
     * @brief creates a tile with a type and a single coordinate pair
     *
     * useful for tiles that only need one pair of coordinates
     *
     * @param type tile type
     * @param x X value stored in sx
     * @param y Y value stored in sy
     */
    _Tile(_TileType type, unsigned short x,
    unsigned short y);

    /**
     * @brief creates a tile with a type and start/end coordinates
     * @param type tile type
     * @param sx start X
     * @param sy start Y
     * @param ex end X
     * @param ey end Y
     */
    _Tile(_TileType type, unsigned short sx,
    unsigned short sy, unsigned short ex, unsigned short ey);

    /**
     * @brief creates a tile with full parameters
     * @param type tile type
     * @param sx start X
     * @param sy start Y
     * @param ex end X
     * @param ey end Y
     * @param speed speed parameter
     */
    _Tile(_TileType type, unsigned short sx,
    unsigned short sy, unsigned short ex,
    unsigned short ey, unsigned short speed);

    /**
     * @brief returns the tile type
     */
    _TileType getType() const;

     /**
     * @brief returns the start X value
     */   
    unsigned short getStartX() const;

    /**
     * @brief returns the end X value
     */    
    unsigned short getEndX() const; 

     /**
     * @brief returns the start Y value
     */   
    unsigned short getStartY() const;

    /**
     * @brief returns the end Y value
     */    
    unsigned short getEndY() const; 

    /**
     * @brief returns the speed parameter
     */
    unsigned short getSpeed() const;

};

/**
 * @brief fixed size grid used to describe a level layout
 *
 * the map stores tiles in a static array [MAP_HEIGHT][MAP_WIDTH]
 * helper methods exist to place enemies, walls and patterns easily
 */
class Map {
private:
    _Tile grid[MAP_HEIGHT][MAP_WIDTH];

    /**
     * @brief checks if a type is allowed 
     * for fill operations
     *
     * used to avoid accidental filling 
     * with enemy or gate markers
     *
     * @param t tile type to check
     * @return true if the type is safe to 
     * use for row/column/fill helpers
     */
    static bool isValidFillType(_TileType t);

public:

    /**
     * @brief creates a map filled with EMPTY tiles
     */
    Map();

    /**
     * @brief sets a tile in the grid
     * @param x X coordinate
     * @param y Y coordinate
     * @param tile tile value to store
     */
    void set(unsigned short x, unsigned short y, _Tile tile);

    /**
     * @brief returns a const reference to a tile in the grid
     * @param x X coordinate
     * @param y Y coordinate
     * @return const reference to the stored tile
     */    
    const _Tile& at(unsigned short x, unsigned short y) const;

    /**
     * @brief returns map width
     */
    unsigned short width() const;

    /**
     * @brief returns map height
     */
    unsigned short height() const; 


    /**
     * @brief places a patroller marker at (x,y) with its path data
     * @param x tile X
     * @param y tile Y
     * @param sx path start X
     * @param sy path start Y
     * @param ex path end X
     * @param ey path end Y
     * @param speed movement speed
     */
    void spawnPatroller(unsigned short x ,unsigned short y ,
    unsigned short sx, unsigned short sy,
    unsigned short ex, unsigned short ey, unsigned short speed);

   /**
     * @brief places a chaser marker at (x,y)
     * @param x tile X
     * @param y tile Y
     * @param speed movement speed (default 10)
     */
    void spawnChaser(unsigned short x, unsigned short y,
    unsigned short speed = 10);

    /**
     * @brief places a walker marker at (x,y)
     * @param x tile X
     * @param y tile Y
     * @param speed movement speed (default 12)
     */
    void spawnWalker(unsigned short x, unsigned short y,
    unsigned short speed = 12);

    /**
     * @brief places a tile at (x,y)
     * @param x tile X
     * @param y tile Y
     * @param t tile value
     */
    void placeTile(unsigned short x, unsigned short y, _Tile t);

    /**
     * @brief fills a horizontal segment on row y
     * @param y row index (Y)
     * @param x1 start X
     * @param x2 end X
     * @param t tile type to place
     */
    void row(unsigned short y,
    unsigned short x1, unsigned short x2,
    _TileType t);

    /**
     * @brief fills a vertical segment on column x
     * @param x column index (X)
     * @param y1 start Y
     * @param y2 end Y
     * @param t tile type to place
     */
    void column(unsigned short x,
    unsigned short y1, unsigned short y2,
    _TileType t);

    /**
     * @brief fills a rectangle area with a tile type
     * @param x1 left X
     * @param y1 top Y
     * @param x2 right X
     * @param y2 bottom Y
     * @param t tile type to place
     */
    void fill(unsigned short x1, unsigned short y1,
    unsigned short x2, unsigned short y2, _TileType t);

    /**
     * @brief places solid walls on the map border
     *
     * @note always use this method after map creation
     */
    void borderWalls();

    /**
     * @brief sets the player spawn marker
     * @param x spawn X
     * @param y spawn Y
     */
    void setSpawn(unsigned short x, unsigned short y);
};

class MapBuilder {
public:
    // debug map 1
    /**
     * @brief first debug map
     * 
     * @author Martina Lisa Saffo Ramponi
     * 
     */
    static Map outsand(); 

    // debug map 2
    /**
     * @brief second debug map
     * 
     * @author Martina Lisa Saffo Ramponi
     * 
     */
    static Map crossroads();

    /**
     * @brief first level map
     * 
     * @author Martina Nazzareni
     */
    static Map motherboard();

    /**
     * @brief second level map
     * 
     * @author Martina Nazzareni
     */    
    static Map ram();

    /**
     * @brief third level map
     * 
     * @author Martina Nazzareni
     */   
    static Map storage();

    /**
     * @brief fourth level map
     * 
     * @author Martina Nazzareni
     */   
    static Map cpu();

    /**
     * @brief fifth level map
     * 
     * @author Martina Nazzareni
     */   
    static Map gpu();

};

#endif
