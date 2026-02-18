/**
 * @file bomb.hpp
 * @author Martina Nazzareni (0001223089)
 * @brief bomb entity and explosion logic
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * a Bomb belongs to a Player and is placed on a Board
 * 
 * after a countdown, it explodes in a cross shape with the given range,
 * damages entities in the blast and can spawn powerups when destructible walls
 * are destroyed
 * 
 */

#ifndef BOMB_HPP
#define BOMB_HPP

#include "board.hpp"
#include "player.hpp"
#include "list.hpp"
#include "enemies.hpp"
#include "random.hpp"
#include "powerup.hpp"

/**
 * @brief special coordinate value used to mark an
 * invalid bomb position
 *
 * we use 65535 because it's the maximum unsigned 
 * short, and is obviusly outside any valid map range
 */
#define BOMB_INVALID 65535

/**
 * @brief represents a placed bomb with a 
 * timer and an explosion range
 *
 * the bomb entity is created by the Level logic
 * 
 * if placement fails, the bomb
 * remains not placed (isPlaced() == false).
 *
 * on explosion:
 * 
 * - sets EXPLOSION cells on the board
 * 
 * - damages the owner if caught in the blast (calling Player::takeDamage())
 * 
 * - kills enemies in the blast
 * 
 * - can spawn powerups when a destructible wall is destroyed
 * 
 */
class Bomb {
public:

    /**
     * @class Bomb::InvalidBoardException 
     * 
     * @brief thrown when the bomb is constructed 
     * without a valid Board pointer
     */
    class InvalidBoardException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "bomb has an invalid Board pointer";
        }
    };

    /**
     * @class Bomb::InvalidPlayerException 
     * 
     * @brief thrown when the bomb is constructed 
     * without a valid Player pointer
     */
    class InvalidPlayerException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "bomb has an invalid Player pointer";
        }
    };
private:
    unsigned short x, y; // coordinates
    // board pointer used for explosion
    Board* board; 
    // bomb timer (ticks)
    unsigned short timer;
    // bomb range (crossed)
    unsigned short range;
    // player pointer used for damage
    // and to get location
    Player* owner;
    
    // when false -> remove
    bool placed = false;

    /**
     * @brief tries to spawn a PowerUp at a given position
     *
     * this is called when a destructible wall is destroyed by an explosion
     * 
     * this method:
     * 
     * - uses RNG to decide whether to spawn a PowerUp
     * 
     * - avoids spawning multiple powerups on the same cell
     *
     * @param x x coordinate
     * @param y y coordinate.
     * @param powerups list where the new PowerUp 
     * will be added if spawned
     * @param rng random generator used to roll spawn chances
     * @return true if a powerup was spawned, false otherwise
     */
    bool trySpawnPowerUp(unsigned short x, unsigned short y,
    list<PowerUp*>& powerups, Random& rng);

public:
    /**
     * @brief constructs a bomb and tries to place it
     *
     * the bomb is placed according to the player's placement rules
     * 
     * if placement is not possible, the bomb becomes "not placed" 
     * (isPlaced() == false) and its coordinates are set to BOMB_INVALID
     *
     * @param b pointer to the Board (must not be null)
     * @param p pointer to the owner Player (must not be null)
     * @param t countdown timer (ticks) before explosion
     * @param r blast range (default 1)
     * @throws InvalidBoardException if b is null
     * @throws InvalidPlayerException if p is null
     */
    Bomb(Board* b, Player* p,unsigned short t = 90,
    unsigned short r = 1);

    /**
     * @brief updates the bomb timer and triggers explosion 
     * if timer reaches 0
     *
     * @param el enemy list, used to kill enemies 
     * @param powerups powerup list, used to spawn new powerups
     * @param rng random generator used for powerup spawning
     */
    void update(list<Enemy*>& el, list<PowerUp*>& powerups,
    Random& rng);

    /**
     * @brief the bomb explodes, applying damage and board changes.
     *
     * after exploding, the bomb is marked as not placed
     * and its coordinates become BOMB_INVALID
     *
     * @param el enemy list, enemies that are hit are removed and deleted
     * @param powerups powerup list, new powerups may be spawned
     * @param rng random generator used for powerup spawning
     */
    void explode(list<Enemy*>& el, list<PowerUp*>& powerups,
    Random& rng);

    /**
     * @brief returns whether this bomb is 
     * currently placed on the board
     */
    bool isPlaced() const;

    /**
     * @brief returns the X coordinate (BOMB_INVALID if not placed)
     */
    unsigned short getX() const;

    /**
     * @brief returns the Y coordinate (BOMB_INVALID if not placed)
     */
    unsigned short getY() const;

    /**
     * @brief default destructor.
     */
    ~Bomb() = default;
};

#endif
