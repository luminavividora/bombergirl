/**
 * @file level.hpp
 * @author Martina Nazzareni (0001223089)
 * @brief defines the Level class, which represents a 
 * single playable level of the game
 * 
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * 
 * the Level class is responsible for managing 
 * the game board, enemies, bombs,power-ups, and level-specific logic
 * 
 * it handles player interactions with the environment, updates all 
 * entities each game tick, and manages level completion and 
 * transitions between levels
 * 
 */

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "board.hpp"
#include "player.hpp"
#include "list.hpp"
#include "bomb.hpp"
#include "enemies.hpp"
#include "maps.hpp"
#include "random.hpp"
#include "powerup.hpp"

#include <exception>


/**
 * @brief represents a single playable level
 *
 * a level owns its board state and the dynamic entities inside it
 * it loads data from a Map, spawns enemies and powerups, and updates bombs
 * it also handles gate transitions between levels
 */
class Level {
public:
    /**
     * @brief thrown when the level is created with an invalid player pointer
     */
    class InvalidPlayerException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "lavel has an invalid player pointer";
            }
    };

    /**
     * @brief requested transition after stepping on a gate
     */
    enum TransitionRequest {
        NEXT,
        PREV,
        NONE
    };

private:
    // the current board state
    // in this level
    Board board;
    // the player controlled entity 
    Player* player;
    // enemies currently alive
    list<Enemy*> enemies;
    // bombs currently on the board
    list<Bomb*> bombs;
    // powerups on the ground
    list<PowerUp*> powerUps;

    // rng used for drops
    Random rng;

    // current transition request
    TransitionRequest transition = TransitionRequest::NONE;

    // gate coords and booleans

    unsigned short gateNextX = 0, gateNextY = 0;
    unsigned short gatePrevX = 0, gatePrevY = 0;
    bool hasGateNext = false;
    bool hasGatePrev = false;


    // true wneh all enemies are defeated
    bool completed = false;

    // spawn position
    unsigned short spawnX = 0, spawnY = 0;

    // legacy explosion effect handler

    // int explosionTicksLeft = 0;
    // bool hasExplosions = false;

public:

    /**
     * @brief builds a level from a map
     *
     * the level creates its board, loads tiles and entities from the map,
     * and stores spawn and gate positions
     *
     * @param p player pointer used by the level
     * @param w board width
     * @param h board height
     * @param map source map data
     * @throws InvalidPlayerException if p is null
     */
    Level(Player* p, unsigned short w,
    unsigned short h, Map& map);

private:
    /**
     * @brief loads tiles, enemies, gates and spawn from the map
     *
     * this initializes board cells and spawns enemies based on map tiles
     */
    void load(Map& map);
public:

    /**
     * @brief updates all level logic for one tick
     *
     * updates enemies, bombs, powerups and explosions
     * checks collisions with player and sets transition request if needed
     */    
    void update();

    /**
     * @brief returns true if the level is completed
     *
     * a level is usually completed when no enemies remain
     */    
    bool isCompleted() const;

    /**
     * @brief returns the current transition request
     *
     * this is set when the player steps on a gate
     */
    TransitionRequest getTransitionRequest() const;
    
    /**
     * @brief converts a map tile type into a board cell type
     */
    Board::CellType tileToCell(_TileType t);
    /**
     * @brief returns a const reference to the board
     */
    const Board& getBoard() const;
    /**
     * @brief returns a const reference to the enemy list
     */    
    const list<Enemy*>& getEnemies() const;

    /**
     * @brief returns a const reference to the bomb list
     */    
    const list<Bomb*>& getBombs() const;

    /**
     * @brief clears the current transition request
     *
     * call this after the game handles the request
     */
    void clearTransitionRequest();

    /**
     * @brief tries to place a bomb for the player
     *
     * creates a bomb object, checks placement and adds it to the bombs list
     */
    void placeBomb();

    /**
     * @brief called when the player enters the level
     *
     * sets player board pointer, resets gate flags, and places the player
     * near the correct gate or at the spawn position
     *
     * @param from which transition was used to enter this level
     */
    void onEnter(Level::TransitionRequest from);

    /**
     * @brief checks if a bomb is currently placed 
     * at the given coordinates
     *
     * @return true if a placed bomb occupies (x,y)
     */
    bool isBombAt(unsigned short x, unsigned short y) const;

    /**
     * @brief called when a destructible wall is destroyed
     *
     * used for drop logic such as spawning powerups
     *
     * @return true if a wall was destroyed
     * 
     * @deprecated no longer used
     */
    // bool onWallDestroyed(unsigned short x, unsigned short y);

    /**
     * @brief returns a const reference to the powerup list
     */
    const list<PowerUp*>& getPowerUps() const;

    /**
     * @brief called when leaving this level
     *
     * used to clean temporary entities like bombs
     */
    void onExit(); 

    /**
     * @brief destroys the level and frees owned resources
     *
     * deletes enemies, bombs and powerups owned by the level
     */
    ~Level();

    /**
     * @brief returns spawn X coordinate
     */
    unsigned short getSpawnX() const;

    /**
     * @brief returns spawn Y coordinate
     */
    unsigned short getSpawnY() const;

    // debug methods

    /**
     * @brief removes every enemy from the level
     *
     * debug helper used to quickly test gates and victory logic
     */
    void killAllEnemies();

    /**
     * @brief removes all destructible walls from the board
     *
     * debug helper to test open areas
     * 
     * @note this method will not trigger powerup drop
     */
    void destroyAllDestructibles();

    /**
     * @brief toggles player immortality state
     *
     * debug helper to prevent losing lives during tests
     */
    void toggleImmortality();

    /**
     * @brief toggles an instant (0 ticks) movement 
     * mode for the player
     *
     * debug helper to move around the map quickly
     */
    void toggleUltraspeed();

    /**
     * @brief forces the player to kill themselves
     *
     * debug helper to test respawn and game over flow
     */
    void suicide();
};

#endif
