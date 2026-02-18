/**
 * @file random.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief random.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * 
 */

#include "random.hpp"

Random::Random(unsigned long long seed) : seed(seed) { }

unsigned long long Random::next() {
    // XOR and shifts
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    seed ^= seed >> 9; 
    seed ^= seed << 11;
    return seed;
}

int Random::nextInt(int min, int max) {
    return min + (next() % (max - min + 1));
}

unsigned long long Random::rotl64(unsigned long long x, int r){
    return (x << r) | (x >> (64 - r));
}

unsigned long long Random::mix64(unsigned long long x) {
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    x =  x ^ (x >> 31);
    return x;
}

unsigned long long Random::newSeed() {

    // time in second
    unsigned long long t = (unsigned long long) time(NULL);

    // cpu clock
    unsigned long long c = (unsigned long long) clock();

    // using stack addresses as salt 
    unsigned long long a1 = (unsigned long long)&t;
    unsigned long long a2 = (unsigned long long)&c;
    unsigned long long a3 = (unsigned long long)&Random::newSeed;

    unsigned long long x = 0;
    x ^= t + 3909100244418426048ULL;
    x ^= (c << 1) + 8296989661994112640ULL;
    x ^= (a1 << 7);
    x ^= (a2 << 13);
    x ^= (a3 << 17);
    x ^= Random::rotl64(t, 21);
    x ^= Random::rotl64(c, 17);
    x ^= Random::rotl64(a1, 13);
    x ^= Random::rotl64(a2, 7);
    x ^= Random::rotl64(a3, 31);

    x = Random::mix64(x);

    if (x == 0ULL){
        x = 0xD1B54A32D192ED03ULL;
    }

    return x;
}


void Random::setSeed(unsigned long long seed) {
    this->seed = seed;
}