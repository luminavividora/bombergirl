/**
 * @file menu.hpp
 * @author Martina Nazzareni (0001223089)
 * @brief main menu logic and input handling
 * @version 1.0
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines the menu class used to 
 * show the start screen and handle user choices
 * the menu supports a classic selection cursor
 *  and a hidden cheat code buffer
 * 
 * 
 */

#ifndef MENU_HPP
#define MENU_HPP

/**
 * @brief main menu controller
 *
 * it renders the menu screen and 
 * reads keyboard input until the user chooses an action
 * 
 * it also tracks a small cheat buffer to 
 * detect the secret code (BOMBERGIRL)
 */
class Menu {
public:
    /**
     * @brief menu result returned by run
     */
    enum Result {
        START, //start a normal game
        LEADERBOARD, // open leaderboard flow
        QUIT, // exit
        CREDITS, // show credits
        BONUS // start bonus mode (BOMBERGIRL)
    };

private:
    // current highlighted menu
    int selectedItem;

    // buffer used to store recent typed characters
    char cheatBuf[16];
    // current length of cheatBuf
    int cheatLen = 0;

public:
    /**
     * @brief creates a menu with default selection
     */
    Menu();

    /**
     * @brief runs the menu loop until a choice is made
     *
     * this method handles keyboard input,
     * updates the selection, and returns a Result
     *
     * @return the chosen menu action
     */
    Result run();

    /**
     * @brief clears the cheat buffer
     *
     * call this when leaving the menu
     * or after a cheat code is matched
     */    
    void resetCheat();
};

#endif