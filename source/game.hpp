/**
 * @file game.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief main game controller and state manager
 * @version 1.0
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines the Game class, 
 * which coordinates the entire game flow
 * 
 * it manages game states, levels, 
 * player progression, menus and transitions
 *
 * the class acts as the central control unit connecting
 * gameplay logic,rendering and user input
 * 
 */

#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "level.hpp"
#include "render.hpp"
#include "menu.hpp"
#include "leaderboard.hpp"
#include "name_entry.hpp"
#include "parser.hpp"

/**
 * @brief standard level number
 * 
 */
#define MAX_LEVEL_SIZE 5

/**
 * @brief main game controller
 *
 * this class manages the whole game lifecycle
 * handling the game heartbeat, game states, level transitions,
 * player progression, leaderboard, bonus mode and rendering
 */
class Game {
public:
    /**
     * @brief possible game states
     *
     * each value represents a different screen or phase of the game
     */
    enum State {
        MENU, //main menu
        PLAYING, //playing a level
        GAMEOVER, // game over screen
        EXIT, // close the game
        WIN, // win screen
        NAME_ENTRY, // leaderboard name entry screen
        LEADERBOARD_INPUT, // leaderboard number input screen
        LEADERBOARD_VIEW, // leaderboard screen with names
        CREDITS, // credits screen
        BONUS, // bonus activation
        BONUS_ERROR // bonus error screen
    };

private:
    //current game state
    State state;

    // player data
    Player player;
    // normal game levels
    Level* levels[MAX_LEVEL_SIZE];
    // current level pointer
    Level* level;

    // current level index
    int currentLevel;
    // remaining time for the current run 
    int worldTime;

    // number of leaderboard entries to display
    int leaderboardCountToShow = 10;
    // displayed leaderboard page
    int leaderboardPage = 0;
    // leaderboard handler 
    Leaderboard leaderboard;
    // true -> "you win" screen
    // false -> "game over" screen
    bool lastVictory;
    
    //true if bonus mode is activve
    bool bonusMode = false;

    // bonus level pointer, loaded from file
    Level* bonusLevels = nullptr;

    // reading file and bonus level error msg
    char bonusErrMsg[128];

    //completion status for each level
    bool levelCompleted[MAX_LEVEL_SIZE];

    /**
     * @brief handles menu logic and transitions
     */
    void handleMenu();

    /**
     * @brief handles keyboard input during gameplay
     */    
    void handleInput(int ch);

    /**
     * @brief updates game logic
     */    
    void update();

    /**
     * @brief renders the current game state
     */    
    void render();

    /**
     * @brief loads a level by index
     */
    void loadLevel(int index);

    /**
     * @brief initializes all classic levels
     */    
    void initLevels();

    /**
     * @brief moves to the next level
     * 
     * @deprecated use findNextIncomplete instead
     */
    void nextLevel();

    /**
     * @brief moves to the previous level
     * 
     * @deprecated use findPrevIncomplete instead
     */    
    void prevLevel();  

    /**
     * @brief sets the current level and handles transition positioning
     */
    void setCurrentLevel(int index, Level::TransitionRequest from);

    /**
     * @brief resets the game to starting state
     */
    void resetGame();

    /**
     * @brief handles leaderboard visualization input
     */
    void handleLeaderboardView();

    /**
     * @brief loads a bonus level from file (bonus.csv)
     */
    void loadBonusLevelFromFile(const char* path);

    /**
     * @brief finds the next incomplete level
     */
    int findNextIncomplete(int from) const;

    /**
     * @brief finds the previous incomplete level
     */    
    int findPrevIncomplete(int from) const;

    /**
     * @brief checks if all levels are completed
     */    
    bool allLevelsCompleted() const;


public:
    Game();
    ~Game();
    void run(); 
};

#endif
