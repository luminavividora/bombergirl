/**
 * @file leaderboard.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief leaderboard management system
 * @version 1.0
 * @date 2026-02-01
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines the Leaderboard class used to store,
 * sort, load and save high scores
 * 
 * it is used to keep track of player names and scores
 * across multiple game sessions
 * 
 */

#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <cstring>
#include <ncurses.h>
#include <fstream>


/**
 * @brief manages the game leaderboard
 *
 * this class stores player scores with a fixed maximum size
 * it supports adding new entries, sorting them,
 * and saving or loading the leaderboard from a file
 */
class Leaderboard {
public:
    /**
     * @brief maximum number of leaderboard entries
     */
    static const int MAX = 50;

    /**
     * @brief single leaderboard entry
     *
     * stores a 3 character player name and a score value
     */
    struct ScoreEntry {
        char name[4]; 
        int score;
    };
private:
    // array of leaderboard entries
    Leaderboard::ScoreEntry entries[Leaderboard::MAX];
    // current number of entries
    int count;
public:
    /**
     * @brief constructs an empty leaderboard
     */
    Leaderboard();

    /**
     * @brief adds a new entry to the leaderboard
     *
     * if the leaderboard is full, the entry may be flushed
     * depending on its score
     */
    void add(const char name[4], int score);

    /**
     * @brief sorts the leaderboard in descending order
     *
     * highest scores will appear first, as it should be
     */
    void sortDesc();
    /**
     * @brief returns the number of stored entries
     */
    int size() const;

    /**
     * @brief returns a reference to an entry by index
     *
     * the index must be valid
     */    
    const Leaderboard::ScoreEntry& at(int i) const;

    /**
     * @brief loads the leaderboard from a file
     *
     * returns false if the file cannot be opened or read
     */
    bool load(const char* path);

    /**
     * @brief saves the leaderboard to a file
     *
     * returns false if the method fails
     */   
    bool save(const char* path) const;
};

#endif 