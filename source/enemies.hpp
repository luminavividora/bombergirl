/**
 * @file enemies.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief enemy base class and concrete 
 * enemy types (Walker, Patroller, Chaser)
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * enemies live inside a list contained in level
 * and move according to their AI logic
 * 
 *  each enemy has a speed expressed in ticks:
 *  it moves only when its internal
 *  tick counter reaches the speed threshold
 * 
 */

#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "board.hpp"
#include "player.hpp"
#include "random.hpp"
#include <exception>

/**
 * @brief walker default speed (ticks)
 * 
 */
#define WALKER_SPEED 8

/**
 * @class Enemy
 * 
 * @brief abstract base class for all enemies
 *
 * 
 * an enemy:
 * 
 * - has a position (x,y) on the Board grid
 * 
 * - has a movement speed measured in ticks
 * 
 * - updates its behavior with update() each game tick
 * 
 *
 * @note derived classes implement update(), 
 * kind() and scoreValue()
 */
class Enemy {
public:
    /**
     * @class Enemy::InvalidStartingPositionException
     * 
     * @brief thrown when an enemy is constructed 
     * on a solid cell
     */
    class InvalidStartingPositionException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "enemy starting position is not valid";
            }
    };

    /**
     * 
     * @brief thrown when an enemy is 
     * constructed without a valid Board pointer
     */
    class InvalidBoardException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "enemy has an invalid board pointer";
            }
    };

    /**
     * @enum Kind
     * 
     * @brief runtime kind identifier for enemies 
     * 
     */
    enum Kind {
        WALKER,
        PATROLLER,
        CHASER
    };

protected:
    unsigned short x, y;
    // board pointer used for AI movement
    Board* board;
    // movement frequency (ticks)
    unsigned short speed;
    // tick counter used for movement
    unsigned short tick;

    /**
     * @brief sets the X coordinate
     *
     * protected because movement logic 
     * belongs to derived classes
     */
    void setX(unsigned short x);

    /**
     * @brief sets the Y coordinate
     *
     * protected because movement logic 
     * belongs to derived classes
     */
    void setY(unsigned short y);

public:
    // DEFAULT CONSTRUCTOR NOT ALLOWED
    Enemy() = delete;

    /**
     * @brief constructs an enemy at a given start position
     *
     * the start cell must be walkable.
     *
     * @param b Board pointer (must not be null)
     * @param startX X coordinate
     * @param startY Y coordinate
     * @param s speed in ticks
     * @throws InvalidBoardException if b is null
     * @throws InvalidStartingPositionException if start cell is not walkable
     */   
    Enemy(Board* b, unsigned short startX,
    unsigned short startY, unsigned short s);

    /**
     * @brief virtual destructor for polymorphic use
     * 
     */
    virtual ~Enemy() = default;

    /**
     * @brief updates the enemy logic for one game tick
     *
     * the implementation will:
     * 
     * - increment tick
     * 
     * - if tick < speed: do nothing
     * 
     * - otherwise reset tick and try to move
     * 
     * @note this method is pure virtual and must be implemented
     * by all concrete derived classes
     */    
    virtual void update() = 0;

    /**
     * @brief returns the current X coordinate
     */
    unsigned short getX() const;

    /**
     * @brief returns the current Y coordinate
     */
    unsigned short getY() const;
    /**
     * @brief returns the Board pointer used by this enemy
     * 
     * @note the Board is not owned by the Enemy
     */
    Board* getBoard() const;

    /**
     * @brief returns the concrete enemy kind.
     * 
     * @note pure virtual
     */
    virtual Enemy::Kind kind() const = 0;
    /**
     * @brief returns the score awarded to the player
     * for killing this enemy
     * 
     * @note pure virtual
     */
    virtual int scoreValue() const = 0;
};

/**
 * @brief random moving enemy
 *
 * walker chooses a direction using a RNG and attempts to move one cell
 * 
 * the last direction is stored to reduce "vibration" 
 * effects and immediate backtracking
 */
class Walker : public Enemy{
private:
    // random number gen
    Random rng;
    //previous chosen direction
    int lastDir = -1;
public:
    // NO DEFAULT CONSTRUCTOR
    Walker() = delete;
    /**
     * @brief constructs a Walker enemy.
     *
     * @param b board pointer (must not be null)
     * @param startX X coordinate.
     * @param startY Y coordinate
     * @param randomSeed seed for the  RNG
     * @param speed  speed in ticks (default 8)
     */
    Walker(Board* b, unsigned short startX, unsigned short startY,
    unsigned long long randomSeed, unsigned short speed = 8);

    /**
     * @brief updates Walker movement
     *  
     */
    void update() override;

    /**
     * @brief returns Enemy::Kind::WALKER.
     */
    Enemy::Kind kind() const override;

    /**
     * @brief returns 100
     */    
    int scoreValue() const override;
};

/**
 * @brief enemy that moves back and forth between two points
 *
 * patroller has a start point (sx,sy) and an end point (ex,ey)
 * with each movement step it tries to get
 * closer to the current target (start or end)
 */
class Patroller : public Enemy {
private:
    // start point 
    unsigned short startX, startY;
    //end point
    unsigned short endX, endY;
    // true -> moving toward end
    // false -> moving toward start
    bool forward;
public:
    // NO DEFAULT CONSTRUCTOR
    Patroller() = delete;
    /**
     * @brief constructs a Patroller enemy
     *
     * @param b board pointer (must not be null)
     * @param sx start X
     * @param sy start Y
     * @param ex end X
     * @param ey end Y
     * @param speed movement speed in ticks (default 4)
     */
    Patroller(Board* b,
        unsigned short sx, unsigned short sy,
        unsigned short ex, unsigned short ey,
        unsigned short speed = 4);

    /**
     * @brief updates Patroller movement
     */    
    void update() override;

    /**
     * @brief returns Enemy::Kind::PATROLLER
     */
    Enemy::Kind kind() const override;

    /**
     * @brief returns 150
     */   
    int scoreValue() const override;

};

/**
 * @brief enemy that chases the player when visible, otherwise wanders
 *
 * chaser sees the player only in straight lines
 * up to a maximum range and with walls blocking vision
 *
 * when the player is visible, the chaser uses chaseSpeed 
 * otherwise it uses idleSpeed
 */
class Chaser : public Enemy {
public:
    /**
     * @brief thrown when a Chaser is 
     * constructed with a null Player pointer
     */
    class InvalidPlayerException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "enemy chaser has an invalid player pointer";
            }
    };
private:
    // player pointer used to chase
    const Player* player;
    // rng for wandering
    Random rng;

    // max vision tiles
    unsigned short vision;
    // true when Chaser is chasing
    bool chasing;

    //wandering speed
    unsigned short idleSpeed;
    //chase speed
    unsigned short chaseSpeed;

    //variables used for wander movement

    short wanderDx;
    short wanderDy;
    unsigned short wanderHold;
    unsigned short wanderHoldLeft;

    /**
     * @brief returns true if the chaser can 
     * see the player 
     * @param maxRange max range to test
     */
    bool seesPlayer(unsigned short maxRange) const;

    /**
     * @brief returns true if the chaser sees 
     * the player on the same row
     * @param maxRange max range to test
     */
    bool seesRow(unsigned short maxRange) const;
    /**
     * @brief returns true if the chaser sees 
     * the player on the same column
     * @param maxRange Max range to test.
     */
    bool seesCol(unsigned short maxRange) const;

    /**
     * @brief returns true if a cell blocks the line of sight
     */
    bool blocksVision(Board::CellType c) const;

    /**
     * @brief attempts to move by (dx,dy) if the destination is walkable
     * @return true if movement is possible, false otherwise
     */
    bool tryMove(short dx, short dy);

    /**
     * @brief picks a new wandering direction 
     * and resets wander holding counters
     */    
    void pickNewWanderDir();

    /**
     * @brief tries to move in the current wander direction 
     */
    void wanderStep();

    /**
     * @brief tries to move closer to the player 
     */
    void chaseStep();

public:
    // NO DEFAULT CONSTRUCTOR
    Chaser() = delete;
    /**
     * @briefconstructs a Chaser enemy.
     *
     * @param b Board pointer (must not be null)
     * @param p Player pointer (must not be null)
     * @param startX  X coordinate
     * @param startY  Y coordinate
     * @param seed seed for the RNG
     * @param speed chase speed in ticks (default 6)
     * @param vision maximum vision range in tiles (default 16)
     * @throws InvalidBoardException if b is null
     * @throws InvalidStartingPositionException if start cell is not walkable
     * @throws InvalidPlayerException if p is null
     */
    Chaser(Board* b, const Player* p,
    unsigned short startX, unsigned short startY,
    unsigned long long seed, unsigned short speed = 6,
    unsigned short vision = 16);

    /**
     * @brief updates Chaser logic 
     */
    void update() override;

    /**
     * @brief Returns Enemy::Kind::CHASER
     */
    Enemy::Kind kind() const override;

    /**
     * @brief returns 300
     */   
    int scoreValue() const override;
    
};

#endif
