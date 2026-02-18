/**
 * @file name_entry.hpp
 * @author Martina Nazzareni (0001223089)
 * @brief player name input screen for the leaderboard
 * @version 1.0
 * @date 2026-02-02
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines the NameEntry helper class
 * 
 * it is responsible for handling the arcade-style name input
 * after a game over or a victory, before saving the score
 * 
 */

#ifndef NAME_ENTRY_HPP
#define NAME_ENTRY_HPP

#include "render.hpp"

/**
 * @brief handles player name input
 *
 * this class shows a dedicated screen where the player can enter
 * a three-letter name using keyboard input
 * the result is written into the provided output buffer
 */
class NameEntry {
public:
    /**
     * @brief runs the name entry screen
     *
     * this method blocks until the 
     * user confirms or cancels the input
     *
     * @param outName output buffer for the name 
     * (must be size 4, including null terminator)
     * @param score final player score, 
     * shown on screen
     * @param victory true if the game ended 
     * with a win, false if game over
     * @return true if the name was confirmed, 
     * false if the user aborted
     */
    static bool run(char outName[4], int score, bool victory);
};

#endif