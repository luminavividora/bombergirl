/**
 * @file game.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief game.hpp implementation
 * @version 1.0
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "game.hpp"

// SECTION GAME PARAMETERS

static const int LEVEL_COUNT = 5;

static const int START_SCORE = 0;
static const int START_MAX_BOMBS = 1;

static const int FPS = 30;
static const int FRAME_MS = 1000 / FPS;

#ifdef DEBUG_MODE
    static const int START_LIVES = 99;
    static const int WORLD_TIME_START = 9999 * FPS;
#else
    static const int START_LIVES = 4;
    static const int WORLD_TIME_START = 800 * FPS;
#endif

//!SECTION

Game::Game(): state(Game::MENU), level(nullptr), currentLevel(0), 
worldTime(WORLD_TIME_START) {
    this->bonusErrMsg[0] = '\0';
    for (int i = 0; i < MAX_LEVEL_SIZE; i++) {
        this->levels[i] = nullptr;
    }

    for (int i = 0; i < LEVEL_COUNT; i++){
        this->levelCompleted[i] = false;
    }

    this->player.setLives(START_LIVES);
    this->player.setScore(START_SCORE);
    this->player.setMaxBombs(START_MAX_BOMBS);
}

int Game::findNextIncomplete(int from) const {
    int i = from + 1;
    while (i < LEVEL_COUNT && this->levelCompleted[i]){
        i++;
    }
    if(i >= LEVEL_COUNT){
        i = -1;
    }
    return i;
}

int Game::findPrevIncomplete(int from) const {
    int i = from - 1;
    while (i >= 0 && levelCompleted[i]){
        i--;
    }
    if(i < 0){
        i = -1;
    }
    return i;
}

bool Game::allLevelsCompleted() const {
    for (int i = 0; i < LEVEL_COUNT; i++) {
        if (this->levelCompleted[i] == false){
            return false;
        }
    }
    return true;
}

void Game::handleMenu() {
    Menu menu;
    Menu::Result res = menu.run();

    if (res == Menu::Result::QUIT) {
        this->state = Game::State::EXIT;
        return;
    }

    if(res == Menu::Result::CREDITS){
        this->state = Game::State::CREDITS;
    }

    if (res == Menu::Result::START) {
        this->currentLevel = 0;

        this->worldTime = WORLD_TIME_START;

        loadLevel(currentLevel);
        this->state = Game::State::PLAYING;
        return;
    }

    if (res == Menu::Result::LEADERBOARD) {
        this->state = Game::State::LEADERBOARD_INPUT;
        return;
    }

    if(res == Menu::Result::BONUS) {
        this->state = Game::State::BONUS;
        return;
    }
}

void Game::handleInput(int ch) {
    if (level == nullptr){
        return;
    }

#ifdef DEBUG_MODE
    // used to clear all enemies when debugging
    if (ch == 'k' || ch == 'K') {
        if (level != nullptr) {
            level->killAllEnemies();
        }
        return;
    }
#endif

#ifdef DEBUG_MODE
    // used to clear all walls when debugging
    if (ch == 'd' || ch == 'D') {
        if (level != nullptr) {
            level->destroyAllDestructibles();
        }
        return;
    }
#endif

#ifdef DEBUG_MODE
    // used to prevent death when debugging
    if (ch == 'i' || ch == 'I') {
        if (level != nullptr) {
            level->toggleImmortality();
        }
        return;
    }
#endif

#ifdef DEBUG_MODE
    // used to move faster when debugging
    if (ch == 'u' || ch == 'U') {
        if (level != nullptr) {
            level->toggleUltraspeed();
        }
        return;
    }
#endif

#ifdef DEBUG_MODE
    // used to try respawn when debugging
    if (ch == 's' || ch == 'S') {
        if (level != nullptr) {
            level->suicide();
        }
        return;
    }
#endif

#ifdef DEBUG_MODE
    // used to lose on purpose when debugging
    if (ch == 'l' || ch == 'L') {
        this->state = Game::State::NAME_ENTRY;
        this->lastVictory = false;
        return;
    }
#endif


    if (ch == 'q' || ch == 'Q') {
        this->state = EXIT;
        return;
    }

    if (ch == ' ') {
        this->level->placeBomb();
        return;
    }

    if (!this->player.canMove()){
        return;
    }

    switch (ch) {
        case KEY_UP:    
            this->player.moveUp();    
        break;
        case KEY_DOWN:  
            this->player.moveDown();  
        break;
        case KEY_LEFT:  
            this->player.moveLeft();  
        break;
        case KEY_RIGHT: 
            this->player.moveRight(); 
        break;
        default: 
            break;
    }
}

void Game::update() {
    this->level->update();
    this->player.updateMovementTimer();
    this->worldTime--;

    Level::TransitionRequest tr = this->level->getTransitionRequest();

    if (this->player.isDead() || this->worldTime <= 0){
        this->state = Game::State::NAME_ENTRY;
        this->lastVictory = false;
        return;
    }

    if (levelCompleted[currentLevel] == false) {
        if (level->getEnemies().len() == 0) {
            levelCompleted[currentLevel] = true;
        }
    }

    if (tr == Level::TransitionRequest::NEXT) {
        if (this->bonusMode) {
            this->level->clearTransitionRequest();

           
            if (this->bonusLevels != nullptr) {
                delete this->bonusLevels;
                this->bonusLevels = nullptr;
            }

            this->bonusMode = false;
            this->resetGame();
            this->state = Game::MENU; 
            return;
        }

        if (this->allLevelsCompleted() == true) {
            this->state = Game::State::NAME_ENTRY;
            this->lastVictory = true;
            this->player.addScore((this->worldTime / FPS) * 10 );
            return;
        }

        int next = findNextIncomplete(currentLevel);
        if (next != -1) {
            setCurrentLevel(next, Level::TransitionRequest::NEXT);
        }
        
        return;
    }

    if (tr == Level::TransitionRequest::PREV) {

        if (this->bonusMode) {
            this->level->clearTransitionRequest();

           
            if (this->bonusLevels != nullptr) {
                delete this->bonusLevels;
                this->bonusLevels = nullptr;
            }

            this->bonusMode = false;
            this->resetGame();
            this->state = Game::State::MENU; 
            return;
        }

        int prev = this->findPrevIncomplete(currentLevel);
        if (prev != -1) {
            this->setCurrentLevel(prev, Level::TransitionRequest::PREV);
        }
        return;
    }

}

void Game::render() {
    if (this->level == nullptr){
        return;
    }
    Render::draw(
        this->level->getBoard(),
        this->player,
        this->level->getEnemies(),
        this->level->getBombs(),
        this->worldTime / FPS,
        this->player.getScore(),
        this->player.getLives(),
        this->currentLevel,
        this->level->getPowerUps()
    );
}

void Game::initLevels() {
    for (int i = 0; i < LEVEL_COUNT; i++){
        levels[i] = nullptr;
    }

    // static Map m0 = MapBuilder::outsand();
    // static Map m1 = MapBuilder::crossroads();

    static Map motherboard = MapBuilder::motherboard();
    static Map ram = MapBuilder::ram();
    static Map storage = MapBuilder::storage();
    static Map cpu = MapBuilder::cpu();
    static Map gpu = MapBuilder::gpu();

    this->levels[0] = new Level(&player, MAP_WIDTH, MAP_HEIGHT, motherboard);
    this->levels[1] =  new Level(&player, MAP_WIDTH, MAP_HEIGHT, ram);
    this->levels[2] = new Level(&player, MAP_WIDTH, MAP_HEIGHT, storage);
    this->levels[3] = new Level(&player, MAP_WIDTH, MAP_HEIGHT, cpu);
    this->levels[4] = new Level(&player, MAP_WIDTH, MAP_HEIGHT, gpu);

    this->player.setSpawn(levels[0]->getSpawnX(), levels[0]->getSpawnY());
}

void Game::nextLevel() {
    int next = this->currentLevel + 1;

    if (next >= LEVEL_COUNT){
        return;
    }
    if (this->levels[next] == nullptr){
        return;
    }

    setCurrentLevel(next, Level::TransitionRequest::NEXT);
}

void Game::prevLevel() {
    int prev = currentLevel - 1;

    if (prev < 0){
        return;
    }
    if (levels[prev] == nullptr){
        return;
    }

    setCurrentLevel(prev, Level::TransitionRequest::PREV);
}

void Game::setCurrentLevel(int index, Level::TransitionRequest from) {
    if (index < 0 || index >= LEVEL_COUNT){
        return;
    }
    if (this->levels[index] == nullptr){
        return;
    }

    if(this->level != nullptr){
        level->onExit(); // clear placed bombs
    }

    this->currentLevel = index;
    this->level = this->levels[index];

    if(this->currentLevel == 0){
        this->player.setMaxBombs(1);
    } else if(this->currentLevel == 1 ||
    this->currentLevel == 2){
        this->player.setMaxBombs(2);
    } else {
        this->player.setMaxBombs(3);
    }

    this->level->onEnter(from);
}

void Game::resetGame() {
    this->player.reset();

    this->player.setLives(START_LIVES);
    this->player.setScore(START_SCORE);
    this->player.setMaxBombs(START_MAX_BOMBS);



    this->worldTime = WORLD_TIME_START;

    this->currentLevel = 0;

    for(int i = 0; i < LEVEL_COUNT; i++){
        if(this->levels[i] != nullptr){
            delete this->levels[i];
            this->levels[i] = nullptr;
        }
    }
    this->level = nullptr;


    this->state = MENU;
}

void Game::loadBonusLevelFromFile(const char* path) {
    char err[128];
    err[0] = '\0';

    //bonus map info
    static Map bonusMap;

    bool ok = Parser::loadBonusFile(path, bonusMap, err, 128);
    if (!ok) {
        // copy error into buffer
        int i = 0;
        for (; err[i] != '\0' && i < 127; i++){
            this->bonusErrMsg[i] = err[i];
        }
        this->bonusErrMsg[i] = '\0';

        this->state = Game::State::BONUS_ERROR;
        return;
    }

    // allocate bonus level
    try {
        this->bonusLevels = new Level(
            &player, 
            MAP_WIDTH, 
            MAP_HEIGHT, 
            bonusMap
        );
    } catch (const std::exception& ex) {
        const char* msg = "Cannot create bonus level.";
        int i = 0;
        for (; msg[i] != '\0' && i < 127; i++){
            this->bonusErrMsg[i] = msg[i];
        }
        this->bonusErrMsg[i] = '\0';

        this->state = Game::State::BONUS_ERROR;
        return;
    }
}

void Game::run() {

    this->leaderboard.load("scores.csv");
    while (this->state != Game::State::EXIT) {

        if (this->state == Game::State::MENU) {
            this->handleMenu();
            continue;
        }

        if (this->state == Game::State::PLAYING) {
            int ch = getch();
            this->handleInput(ch);
            this->update();
            this->render();
        } else if (state == Game::State::GAMEOVER) {

            erase();
            mvprintw(LINES/2, (COLS - 9)/2, "GAME OVER");
            mvprintw(LINES/2 + 2, (COLS - 24)/2, "Press ENTER for menu");
            mvprintw(LINES/2 + 3, (COLS - 12)/2, "Press Q to quit");
            refresh();

            int ch = getch();

            if (ch == 'q' || ch == 'Q') {
                this->state = Game::State::EXIT;
            }
            if (ch == 10 || ch == KEY_ENTER) {
                this->lastVictory = false;
                this->state = Game::State::MENU;
            }
        } else if (state == WIN) {
            erase();
            mvprintw(LINES/2, (COLS - 8)/2, "YOU WIN!");
            mvprintw(LINES/2 + 2, (COLS - 28)/2, "Press ENTER for menu");
            mvprintw(LINES/2 + 3, (COLS - 16)/2, "Press Q to quit");
            refresh();

            int ch = getch();

            if (ch == 'q' || ch == 'Q'){
                state = EXIT;
            }

            if (ch == 10 || ch == KEY_ENTER) {
                this->state = Game::State::MENU;
                this->lastVictory = true;
            }
        } else if (this->state == Game::State::NAME_ENTRY) {

            char name[4];
            int finalScore = (int)player.getScore();

            bool ok = false;

            if(!this->bonusMode){
                ok = NameEntry::run(name, 
                finalScore, this->lastVictory);
            }

            if (ok) {
                this->leaderboard.add(name, finalScore);
                this->leaderboard.save("scores.csv"); 
            }

            resetGame();
            if(this->lastVictory){
                this->state = Game::State::WIN;
            } else {
                this->state = Game::State::GAMEOVER;
            }
            this->bonusMode = false;
            continue;
        } else if (this->state == Game::State::LEADERBOARD_INPUT){
                nodelay(stdscr, FALSE);
                flushinp(); 

                this->leaderboard.load("scores.csv");

                int maxEntries = this->leaderboard.size();
                if (maxEntries <= 0) {
                    nodelay(stdscr, FALSE);

                    // input drain
                    nodelay(stdscr, TRUE);
                    int c;
                    while ((c = getch()) != ERR) { }
                    nodelay(stdscr, FALSE);

                    Render::drawLeaderboardEmpty();

                    // wait for ENTER/Q
                    int ch2 = getch();
                    (void)ch2;

                    // clear leaderboard and go back
                    flushinp();
                    this->state = Game::State::MENU;
                    continue;
                }

                int current = 10;
                if (current > maxEntries){
                    current = maxEntries;
                }

                while (true) {
                    Render::drawLeaderboardAskCount(maxEntries, current);
                    int ch2 = getch();

                    if (ch2 == 'q' || ch2 == 'Q' || ch2 == 27 /* ESC */) {
                        flushinp();
                        this->state = Game::MENU;
                        break;
                    }

                    // handle player count selection

                    if (ch2 == KEY_UP) {
                        if (current < maxEntries){
                            current++;
                        }
                    } 
                    else if (ch2 == KEY_DOWN) {
                        if (current > 1){
                            current--;
                        }
                    }
                    else if (ch2 == KEY_RIGHT) {
                        current += 5;
                        if (current > maxEntries){
                            current = maxEntries;
                        }
                    }
                    else if (ch2 == KEY_LEFT) {
                        current -= 5;
                        if (current < 1){
                            current = 1;
                        }
                    }
                    else if (ch2 == 10 || ch2 == KEY_ENTER) {
                        this->leaderboardCountToShow = current;
                        flushinp();
                        this->state = Game::State::LEADERBOARD_VIEW;
                        break;
                    }
                }

                continue;
        }  else if (this->state == Game::State::LEADERBOARD_VIEW) {
                nodelay(stdscr, FALSE);
                flushinp();

                Render::drawLeaderboard(this->leaderboard, 
                this->leaderboardCountToShow, this->leaderboardPage);

                int ch = getch();

                int total = this->leaderboard.size();
                int maxShow = this->leaderboardCountToShow;
                if (maxShow < 1){
                    maxShow = 1;
                }

                if (maxShow > total){
                    maxShow = total;
                }

                const int PAGE_SIZE = 5;
                int pages = (maxShow + PAGE_SIZE - 1) / PAGE_SIZE;
                if (pages < 1){
                    pages = 1;
                }

                if (ch == KEY_LEFT) {
                    if (this->leaderboardPage > 0){
                        this->leaderboardPage--;
                    }
                } else if (ch == KEY_RIGHT) {
                    if (this->leaderboardPage < pages - 1){
                        this->leaderboardPage++;
                    }
                } else if (ch == 'q' || ch == 'Q' ||
                ch == 27 || ch == 10 || ch == KEY_ENTER) {
                    state = Game::State::MENU;
                    flushinp();
                }

            continue;
        } else if (this->state == Game::State::CREDITS) {
            nodelay(stdscr, FALSE);

            Render::drawCredits();

            int ch2 = getch();
            if (ch2 == 'q' || ch2 == 'Q' ||
                ch2 == 10 || ch2 == KEY_ENTER ||
                ch2 == 27 /* ESC */) {

                this->state = Game::State::MENU;
                flushinp();
            }
        } else if (this->state == Game::State::BONUS) {
            this->bonusMode = true;
            this->worldTime = WORLD_TIME_START;

            if (this->bonusLevels != nullptr) {
                delete this->bonusLevels;
                this->bonusLevels = nullptr;
            }

            this->loadBonusLevelFromFile("bonus.csv");

            if (this->state == Game::State::BONUS_ERROR){
                continue;
            }

            this->currentLevel = 0;
            this->level = bonusLevels;
            this->level->onEnter(Level::TransitionRequest::NONE);

            this->state = Game::State::PLAYING;
           continue;

        } else if (this->state == Game::State::BONUS_ERROR) {
            nodelay(stdscr, FALSE);

            const char* msg = "UNKNOWN BONUS ERROR";
            if (this->bonusErrMsg[0] != '\0') {
                msg = this->bonusErrMsg;
            }

            Render::drawBonusError(msg);

            int ch = getch();

            if (ch == 10 || ch == KEY_ENTER ||
                ch == 'q' || ch == 'Q' ||
                ch == 27 /* ESC */) {

                flushinp();

                if (this->bonusLevels != nullptr) {
                    delete this->bonusLevels;
                    this->bonusLevels = nullptr;
                }

                level = nullptr;
                state = Game::State::MENU;
            }
            continue;
        }
        // frame cap
        napms(FPS);
    }
}

void Game::loadLevel(int index) {
    if (this->levels[0] == nullptr) {
        this->initLevels();
    }

    this->setCurrentLevel(index, Level::TransitionRequest::NONE);   
}

void Game::handleLeaderboardView() {
    Render::drawLeaderboard(this->leaderboard, 
    this->leaderboardCountToShow,this->leaderboardPage);

    int ch = getch();
    if (ch == 'q' || ch == 'Q' || ch == 10 || ch == KEY_ENTER) {
        this->state = Game::State::MENU;
    }
}

Game::~Game(){
    for(int i = 0; i < LEVEL_COUNT; i++){
        if(this->levels[i] != nullptr){
            delete this->levels[i];
            this->levels[i] = nullptr;
        }
    }

    if(this->bonusLevels != nullptr){
        delete this->bonusLevels;
    }
}
