/**
 * @file player.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief player entity and player state
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * 
 * this file defines the Player class
 *
 * the player owns position, lives, score, 
 * movement cooldown, bomb limits,
 * and timed effects like invulnerability
 * and temporary buffs
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <exception>
#include "board.hpp"

/**
 * @brief default invulnerability duration after 
 * taking damage (in ticks)
 */
#define INVULN_TIME_TICKS 110

/**
 * @brief main controllable character
 *
 * the player moves on the Board grid, places bombs, 
 * takes damage, and collects score
 * 
 * movement is limited by a cooldown in ticks 
 * to avoid instant movement every frame
 * 
 * temporary effects are handled with 
 * internal timers updated every tick
 */
class Player {
public:
    /**
     * @brief thrown when the initial (x,y) is not a valid walkable cell
     */
    class InvalidStartingPositionException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "player starting position is not valid";
            }
    };

    /**
     * @brief thrown when the board pointer is null
     */
    class InvalidBoardException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "player has an invalid board pointer";
            }
    };

    /**
     * @brief last movement direction, used for gameplay logic
     */
    enum Direction { 
        UP, 
        DOWN, 
        LEFT, 
        RIGHT 
    };
private:

    unsigned short x = 0, y = 0;

    unsigned short spawnx = 0, spawny = 0;
    int lives = 3; 
    unsigned int score = 0; 
    // board pointer used for movement
    Board* board;
    // max number of bombs at the
    // same time
    unsigned short maxBombs = 1;
    // number of currently placed bombs
    unsigned short bombsPlaced = 0;
    bool reachedGate = false;

    Direction lastMoveDirection = DOWN;

    bool reachedNextGate = false;
    bool reachedPrevGate = false; 

    //speed variables

    unsigned short moveCooldown = 4;
    unsigned short moveTick = 0;

    //invulnerability variables

    unsigned short invulnTicks = 0;
    unsigned short invulnTotal = 0;

    //speed powerup variables

    unsigned short speedBuffLeft = 0;
    unsigned short speedBonus = 0;

    // bomb range powerup variables

    unsigned short bombBuffLeft = 0;
    unsigned short bombRangeBonus = 0;   

    unsigned short bombRange = 1; 
public:
    /**
     * @brief construct a default player (requires setBoard before movement)
     */
    Player();
    /**
     * @brief construct a player at a starting position on a board
     *
     * @param x start X
     * @param y start Y
     * @param b board pointer (must not be null)
     * @throws InvalidBoardException if b is null
     * @throws InvalidStartingPositionException 
     * if (x,y) is not walkable
     */    
    Player(unsigned short x, unsigned short y, Board* b);

    /**
     * @brief construct a player with full state
     *
     * @param x start X
     * @param y start Y
     * @param lives initial lives
     * @param score initial score
     * @param maxBombs max bombs allowed
     * @param bombsPlaced currently placed bombs
     * @param b board pointer (must not be null)
     * @throws InvalidBoardException if b is null
     * @throws InvalidStartingPositionException 
     * if (x,y) is not walkable
     */    
    Player(unsigned short x, unsigned short y,
    int lives, unsigned short score,
    unsigned short maxBombs, unsigned short bombsPlaced, Board* b);

    /**
     * @brief destroy the player
     */
    ~Player();

    /**
     * @brief move one cell up if possible
     */
    void moveUp();

    /**
     * @brief move one cell down if possible
     */    
    void moveDown();

    /**
     * @brief move one cell left if possible
     */    
    void moveLeft();

    /**
     * @brief move one cell right if possible
     */    
    void moveRight();

    /**
     * @brief attempt to place a bomb and return its position
     *
     * this decreases the available 
     * bomb count only if placement succeeds
     *
     * @param bomb_x output bomb x
     * @param bomb_y output bomb y
     * @return true if a bomb was placed, false otherwise
     */
    bool placeBomb(unsigned short& bomb_x, unsigned short& bomb_y);

    /**
     * @brief current bomb explosion range for new bombs
     *
     * this includes temporary buffs
     *
     * @return range in tiles
     */    
    unsigned short getBombRange() const;

    /**
     * @brief notify the player that 
     * one of their bombs has exploded
     *
     * this frees one slot so the
     * player can place another bomb
     */
    void bombExploded();

    /**
     * @brief apply damage to the player
     *
     * if invulnerable, nothing happens
     * otherwise the player loses one life, 
     * respawns, and becomes invulnerable
     * for a short time (INVULN_TIME_TICKS)
     */
    void takeDamage();

    /**
     * @brief add points to the score
     *
     * @param points points to add
     */
    void addScore(unsigned int points);

    /**
     * @brief get current X position
     */
    unsigned short getX() const;

    /**
     * @brief get current Y position
     */    
    unsigned short getY() const;

    /**
     * @brief get remaining lives
     */   
    int getLives() const;

    /**
     * @brief get current score
     */    
    unsigned short getScore() const;

    /**
     * @brief permanently increase base bomb range by 1
     */
    void increaseBombRange();

    /**
     * @brief set or change the board pointer 
     * used for movement and interactions
     *
     * @param board new board pointer
     */
    void setBoard(Board* board);
    /**
     * @brief set X position directly
     *
     * used for level transitions and respawn
     */
    void setX(unsigned short x);

    /**
     * @brief set Y position directly
     *
     * used for level transitions and respawn
     */
    void setY(unsigned short y);

    /**
     * @brief check if the player has no lives left
     */
    bool isDead() const;
    
    /**
     * @brief set respawn position
     */
    void setSpawn(unsigned short x, unsigned short y);

    /**
     * @brief true if the player stepped on the next gate
     */
    bool hasReachedNextGate() const;

    /**
     * @brief true if the player stepped on the previous gate
     */    
    bool hasReachedPrevGate() const;

    /**
     * @brief clear gate flags after handling a transition
     */    
    void resetGateFlags();

    /**
     * @brief set remaining lives
     */
    void setLives(int lives);

    /**
     * @brief set current score
     */    
    void setScore(unsigned int score);

    /**
     * @brief set max bombs allowed
     */
    void setMaxBombs(unsigned int maxBombs);

    /**
     * @brief set movement cooldown in ticks
     *
     * higher values mean slower movement
     */
    void setMoveCooldown(unsigned short c);

    /**
     * @brief update movement timer by one tick
     *
     * call once per game tick
     */
    void updateMovementTimer();

    /**
     * @brief true if the player can move on this tick
     */    
    bool canMove() const;

    /**
     * @brief update invulnerability timer by one tick
     *
     * call once per game tick
     */
    void updateInvulnerability();

    /**
     * @brief true if the player is currently invulnerable
     */
    bool isInvulnerable() const;

    /**
     * @brief set invulnerability duration and start it now
     *
     * @param ticks duration in ticks
     */
    void setInvulnerability(unsigned short ticks);

    /**
     * @brief total invulnerability duration 
     * for the current invulnerability period
     */    
    unsigned short getInvulnTotal() const;
    
    /**
     * @brief remaining invulnerability ticks
     */    
    unsigned short getInvulnTicks() const;

    /**
     * @brief apply a temporary speed buff
     *
     * speed is implemented as a 
     * reduction of moveCooldown
     *
     * @param newCooldown number of ticks 
     * subtracted from basic cooldown
     * 
     * @param ticks effect duration in ticks
     */
    void giveSpeedBuff(unsigned short newCooldown, int ticks);

    /**
     * @brief apply a temporary bomb range buff
     *
     * @param bonus additional range
     * @param ticks duration in ticks
     */
    void giveBombRangeBuff(unsigned short bonus, int ticks);

    /**
     * @brief update all temporary effect timers by one tick
     *
     * call once per game tick
     */
    void updateTimers();

    /**
     * @brief get current movement speed value used by gameplay
     *
     * returns a cooldown value after buffs are applied
     */
    unsigned short getSpeed() const;
    /**
     * @brief reset bombsPlaced to
     * zero without exploding bombs
     *
     * used when leaving a level so 
     * the player is not blocked by old bombs
     */
    void resetCurrentBombs();

    /**
     * @brief remove invulnerability immediately
     */
    void removeInvulnerability();

    /**
     * @brief set movement speed directly
     *
     * used for debug 
     */
    void setSpeed(unsigned short speed);

    /**
     * @brief reset player state to defaults
     *
     * used for restarting a game session
     */
    void reset();

};

#endif