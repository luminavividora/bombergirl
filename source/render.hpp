/**
 * @file render.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief rendering utilities based on ncurses
 * @version 1.0
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file contains all rendering functions used by the game
 * it is responsible for drawing the game world, menus, hud,
 * leaderboards and special screens using ncurses
 * 
 * the class is static and does not hold any game state
 */

#ifndef RENDER_HPP
#define RENDER_HPP

#include <ncurses.h>
#include "leaderboard.hpp"
#include "board.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "enemies.hpp"
#include "bomb.hpp"
#include "list.hpp"

/**
 * @brief static rendering interface for the game
 *
 * this class provides only static methods 
 * and acts as a rendering layer between 
 * the game logic and the terminal
 *
 * it handles screen layout, colors, text alignment and resize events
 */
class Render {
public:

    /**
     * @brief identifiers for main screen types
     */
    enum Screen {
        SCREEN_MENU,
        SCREEN_GAME,
        SCREEN_GAMEOVER
    };

    /**
     * @brief initializes ncurses and rendering settings
     */
    static void init();

    /**
     * @brief shuts down ncurses and restores terminal state
     */
    static void shutdown();

    /**
     * @brief draws the main game screen
     *
     * renders the board, player, enemies, bombs, powerups and hud
     */
    static void draw(
        const Board& board,
        const Player& player,
        const list<Enemy*>& enemies,
        const list<Bomb*>& bombs,
        int timeLeft,
        int score,
        int lives,
        int levelIndex,
        const list<PowerUp*>& powerUps
    );

    /**
     * @brief draws the main menu screen
     *
     * highlights the currently selected menu item
     */
    static void drawMenu(int selectedItem);

    /**
     * @brief draws a simple ascii box
     */
    static void drawAsciiBox(int y, int x, int h, int w);

    /**
     * @brief draws a yellow sparkle animation inside a box
     */
    static void drawSparkle(int y, int x, int h,
    int w, int step);

    /**
     * @brief draws the game title
     */
    static void drawTitle(int y, int x);

    /**
     * @brief draws heart icons used for decoration
     */
    static void drawHearts(int y, int xLeft, 
    int xRight, bool strong) ;

    /**
     * @brief computes centered x position for a text inside a box
     */
    static int centerX(int boxX, int boxW, int textW);

    /**
     * @brief returns the length of a C-style string
     */
    static int strLen(const char* s);
    
    /**
     * @brief checks if color rendering is available
     */
    static bool colorsEnabled();

    /**
     * @brief enables a color pair if colors
     * are available
     */
    static void colorOn(int pair);

    /**
     * @brief disables a color pair if colors
     * are available
     */
    static void colorOff(int pair);
    
    /**
     * @brief draws the hud inside a boxed area
     */
    static void drawHudInBox( int boxX, int boxY, int boxW,
    int levelIndex, int timeLeft, int score, int lives);

    /**
     * @brief draws the name entry screen
     */
    static void drawNameEntry(const char name[4], int selectedIndex,
    int score, bool victory);

    /**
     * @brief prints text safely inside screen bounds
     */
    static void safePrint(int y, int x, const char* s);

    /**
     * @brief prints centered text safely
     */
    static void safeCenterPrint(int y, const char* s);

    /**
     * @brief handles terminal resize events
     */
    static void handleResize();

    /**
     * @brief draws the leaderboard empty state
     */
    static void drawLeaderboardEmpty();

    /**
     * @brief draws the leaderboard entry count selection screen
     */
    static void drawLeaderboardAskCount(int maxEntries, int current);

    /**
     * @brief draws the leaderboard with pagination
     */
    static void drawLeaderboard(const Leaderboard& lb, 
    int maxToShow, int page);

    /**
     * @brief draws the credits screen
     */
    static void drawCredits();
    
    /**
     * @brief draws an error screen for bonus mode
     */
    static void drawBonusError(const char* msg);
};

#endif
