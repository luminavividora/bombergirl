/**
 * @file main.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * and Martina Nazzareni (0001223089)
 * @brief bombergirl main filem, program entry point
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file contains the main function of the Bombergirl game
 * 
 * tt initializes the game environment, creates the main Game
 * object, and starts the game loop (game.run())
 * 
 */

#include "board.hpp"
#include "bomb.hpp"
#include "enemies.hpp"
#include "game.hpp"
#include "leaderboard.hpp"
#include "level.hpp"
#include "maps.hpp"
#include "menu.hpp"
#include "name_entry.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "random.hpp"
#include "render.hpp"

#include "list.hpp"

#include <ctime>
#include <fstream>
#include <ncurses.h>



int main() {
    Render::init();

    Game game;
    game.run();

    Render::shutdown();

    return 0;
}
