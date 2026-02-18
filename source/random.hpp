/**
 * @file random.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief simple deterministic random number generator
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines a lightweight pseudo-random number generator
 * it is used to provide randomness for enemies and powerups
 * 
 * the generator is seed-based and does 
 * not rely on the standard random library
 */

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>

/**
 * @brief pseudo-random number generator 
 * based on a 64-bit seed
 *
 * this class generates pseudo-random numbers starting
 * from an initial seed
 * 
 * the same seed always produces the same sequence of values
 *
 */
class Random {
private:
    /**
     * @brief internal 64-bit seed used by the generator
     */
    unsigned long long seed;

public:
    /**
     * @brief constructs a random generator with a given seed
     *
     * if no seed is provided, 0 will be used
     * 
     * @warning the seed 0 will always generate 0 
     */
    Random(unsigned long long seed = 0);
    /**
     * @brief generates the next raw pseudo-random value
     *
     * updates the internal seed 
     * and returns the new value
     */
    unsigned long long next();

    /**
     * @brief generates a random integer in a given range
     *
     * @param min lower bound (inclusive)
     * @param max upper bound (inclusive)
     * @return random integer between min and max
     */
    int nextInt(int min, int max);

    /**
     * @brief generates a new seed based on system state
     *
     * used to initialize generators 
     * with different starting values
     */
    static unsigned long long newSeed();

    /**
     * @brief sets a new seed for the generator
     *
     * resets the internal state of the generator
     */
    void setSeed(unsigned long long seed);

    /**
     * @brief rotates a 64-bit value to the left
     *
     * helper function used by the mixing algorithm
     */
    static unsigned long long rotl64(unsigned long long x, int r);
    
    /**
     * @brief mixes a 64-bit value to improve randomness
     *
     * used internally to scramble the seed and reduce patterns
     */
    static unsigned long long mix64(unsigned long long x);
    
};

#endif