/**
 * @file powerup.hpp
 * @author Martina Nazzareni (0001223089)
 * @brief defines powerups that can 
 * be collected by the player
 * @version 1.0
 * @date 2026-02-02
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file contains the base PowerUp class
 * and all derived powerup types
 * 
 * powerups are spawned on the map and apply temporary
 * or instant effects when collected by the player
 * 
 */

#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "player.hpp"

/**
 * @brief base class for all powerups
 *
 * a powerup is an object placed on the board that the player can collect
 * each powerup has a limited lifetime and disappears when it expires
 * 
 * when collected, it applies an effect to the player
 * specific behaviors are implemented by derived classes
 */
class PowerUp{
public:
    /**
     * @brief type of powerup
     */
    enum Type {
        STAR, // grants temporary ininvulnerability
        GUNPOWDER, // increases bomb explosion range
        SPEED, // increases player movement speed
        SCORE_UP, // grants bonus score
        TIME_BONUS, // grants bonus time
        NONE // invalid powerup
    };
protected:
    unsigned short x = 0;
    unsigned short y = 0;

    // remaining lifetime before despawn
    unsigned short timeLeft = 0;

    // value of the effect
    unsigned short effectValue = 0;
    // powerup type
    PowerUp::Type type;
public:

    /**
     * @brief creates a powerup at a given position
     * @param x X coordinate
     * @param y Y coordinate
     * @param timeLeft lifetime before despawn
     * @param effectValue effect strength or duration
     */
    PowerUp(unsigned short x, unsigned short y, 
    unsigned short timeLeft, unsigned short effectValue);
    /**
     * @brief returns remaining lifetime
     * 
     */
    unsigned short getTimeLeft() const;

    /**
     * @brief returns effect value
     * 
     * @return  
     */
    unsigned short getEffectValue() const;

    /**
     * @brief returns X coordinate
     * 
     * @return  
     */
    unsigned short getX() const;

    /**
     * @brief returns Y coordinate
     * 
     * @return  
     */
    unsigned short getY() const;

    /**
     * @brief returns powerup type
     * 
     */
    PowerUp::Type getType() const;

    /**
     * @brief returns true if the powerup expired
     * 
     */
    bool isExpired() const;
    
    /**
     * @brief updates internal timers
     * 
     */
    void update();

    /**
     * @brief applies the powerup effect to the player
     *
     * @note this method is pure virtual and must
     * be implemented by each derived powerup class
     */    
    virtual void apply(Player& p) = 0;

    /**
     * @brief default destructor
     * 
     */
    virtual ~PowerUp() = default;
};

/**
 * @brief invulnerability powerup
 */
class Star : public PowerUp {
public:
    Star(unsigned short x, unsigned short y, 
    unsigned short timeLeft, unsigned short effectValue);

    /**
     * @brief grants temporary invulnerability to the player
     * 
     */
    void apply(Player& p) override;
};

/**
 * @brief bomb range increase powerup
 */
class Gunpowder : public PowerUp {
public:
    Gunpowder(unsigned short x, unsigned short y, 
    unsigned short timeLeft, unsigned short effectValue);

    /**
     * @brief increases bomb explosion range
     * 
     */
    void apply(Player& p) override;
};

/**
 * @brief movement speed powerup
 */
class SpeedUp : public PowerUp {
public:
    SpeedUp(unsigned short x, unsigned short y, 
    unsigned short timeLeft, unsigned short effectValue);

    /**
     * @brief increases player movement speed
     * 
     */
    void apply(Player& p) override;
};

/**
 * @brief score bonus powerup
 */
class ScoreUp : public PowerUp {
public:
    ScoreUp(unsigned short x, unsigned short y, 
    unsigned short timeLeft, unsigned short effectValue);

    /**
     * @brief adds bonus score to the player
     * 
     */
    void apply(Player& p) override;
};

#endif