/**
 * @file render.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief render.hpp implementation
 * @version 1.0
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "render.hpp"

static bool g_colorsEnabled = false; // used for color check

bool Render::colorsEnabled() {
    return g_colorsEnabled;
}

void Render::colorOn(int pair) {
    if (!g_colorsEnabled){
        return;
    }

    attron(COLOR_PAIR(pair));
}

void Render::colorOff(int pair) {
    if (!g_colorsEnabled){
        return;
    }
    attroff(COLOR_PAIR(pair));
}

void Render::init() {
    initscr();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    nodelay(stdscr, TRUE);

    // that's a real color check, if false the game will run without colors
    if (!has_colors()) {
        g_colorsEnabled = false;
        clear();
        mvprintw(0, 0, "WARNING: Terminal does NOT support colors.");
        refresh();
        napms(2000);
        return;
    }

    start_color();
    use_default_colors();

    if (COLORS < 8 || COLOR_PAIRS < 8) {
        g_colorsEnabled = false;
        clear();
        mvprintw(0, 0,
            "WARNING: Not enough colors (%d) or pairs (%d).",
            COLORS, COLOR_PAIRS);
        refresh();
        napms(2000);
        return;
    }

    // setting ncurses colors
    init_pair(1,COLOR_WHITE,-1);
    init_pair(2,COLOR_BLUE, -1);
    init_pair(3,COLOR_YELLOW, -1);
    init_pair(4,COLOR_RED, -1);
    init_pair(5, COLOR_GREEN,-1);
    init_pair(6,COLOR_MAGENTA,-1);
    init_pair(7,COLOR_CYAN,-1);
    init_pair(8,  COLOR_WHITE,-1);

    g_colorsEnabled = true;

    clear();

    int y = LINES / 2 - 4;
    int x = (COLS - 20) / 2;

    for (int i = 1; i <= 7; i++) {
        attron(COLOR_PAIR(i) | A_BOLD);
        mvprintw(y + i - 1, x, 
        "@@@@@@@@@@ COLOR %d @@@@@@@@@@", i);
        attroff(COLOR_PAIR(i) | A_BOLD);
    }

    attron(A_BOLD);
    mvprintw(y - 2, x + 10, "COLOR TEST");
    attroff(A_BOLD);

    refresh();

    // beep beep beep
    beep();
    napms(300);
    beep();
    napms(300);
    beep();

    // wait 3 seconds
    napms(3000);

    clear();
    refresh();
}


void Render::drawAsciiBox(int y, int x, int h, int w) {
    mvaddch(y,     x,     '+');
    mvaddch(y,     x+w-1, '+');
    mvaddch(y+h-1, x,     '+');
    mvaddch(y+h-1, x+w-1, '+');
    
    for (int i = 1; i < w-1; i++) {
        mvaddch(y,     x+i, '-');
        mvaddch(y+h-1, x+i, '-');
    }

    for (int i = 1; i < h-1; i++) {
        mvaddch(y+i, x,     '|');
        mvaddch(y+i, x+w-1, '|');
    }
}

void Render::drawTitle(int y, int x) {
    //ASCII ART
    //i know it looks like gibberish
    mvprintw(y+0, x, " ____   ___  __  __ ____  _____ ____"
    "   ____ ___ ____  _     ");
    mvprintw(y+1, x, "| __ ) / _ \\|  \\/  | __ )| ____|"
    "  _ \\ / ___|_ _|  _ \\| |    ");
    mvprintw(y+2, x, "|  _ \\| | | | |\\/| |  _ \\|  _| "
    "| |_) | |  _ | || |_) | |    ");
    mvprintw(y+3, x, "| |_) | |_| | |  | | |_) | |___|  "
    "_ <| |_| || ||  _ <| |___ ");
    mvprintw(y+4, x, "|____/ \\___/|_|  |_|____/|_____|_| "
    "\\_\\\\____|___|_| \\_\\_____|");
}

void Render::drawSparkle(int y, int x, int h, int w, int step) {
    int perimeter = 2*(w-2) + 2*(h-2);

    if (perimeter <= 0){ 
        return;
    }

    int s = step % perimeter;
    int sx, sy;

    if (s < (w-2)) {
        sx = x + 1 + s;
        sy = y;
    } else {
        s -= (w-2);
        if (s < (h-2)) {
            sx = x + w - 1;
            sy = y + 1 + s;
        } else {
            s -= (h-2);
            if (s < (w-2)) {
                sx = x + w - 2 - s;
                sy = y + h - 1;
            } else {
                s -= (w-2);
                sx = x;
                sy = y + h - 2 - s;
            }
        }
    }

    Render::colorOn(3);
    attron(A_BOLD);
    mvaddch(sy, sx, '*');
    attroff(A_BOLD);
    Render::colorOff(3);
}

int Render::strLen(const char* s) {
    if (s == NULL){
        return 0;
    }
    int n = 0;
    while (s[n] != '\0') {
        n++;
    }
    return n;
}

int Render::centerX(int boxX, int boxW, int textW) {
    int cx = boxX + (boxW - textW) / 2;
    if (cx < boxX + 1){
        cx = boxX + 1;
    }
    return cx;
}

void Render::drawHearts(int y, int xLeft, int xRight, bool strong) {
    if (strong){
        attron(A_BOLD);
    }
    mvprintw(y, xLeft,  "<3");
    mvprintw(y, xRight, "<3");
    if (strong){
        attroff(A_BOLD);
    }
}

void Render::safePrint(int y, int x, const char* s) {
    if (s == NULL){
        return;
    }

    //clamp y
    if (y < 0){
        y = 0;
    }
    if (y >= LINES){
        y = LINES - 1;
    }

    // and x
    if (x < 0){
        x = 0;
    }
    if (x >= COLS){
        x = COLS - 1;
    }

    mvprintw(y, x, "%s", s);
}

void Render::safeCenterPrint(int y, const char* s) {
    if (s == NULL){
        return;
    }

    if (y < 0 || y >= LINES){
        return;
    }

    int len = Render::strLen(s);
    int x = (COLS - len) / 2;

    if (x < 0){
        x = 0;
    }

    int maxLen = COLS - x;
    if (maxLen <= 0){
        return;
    }

    for (int i = 0; i < len && i < maxLen; i++) {
        mvaddch(y, x + i, s[i]);
    }
}

void Render::drawMenu(int selectedItem) {

    const int ITEM_COUNT = 3;
    const char* items[ITEM_COUNT] = {
        "START GAME",
        "LEADERBOARD",
        "EXIT"
    };

    static int animStep = 0;
    static int dots = 0;

    erase();

    // display error, menu does not fit in the terminal
    if (LINES < 22 || COLS < 74) {
        erase();

        Render::colorOn(4); // red error
        attron(A_BOLD);

        Render::safeCenterPrint(LINES / 2, 
        "PLEASE RESIZE THE TERMINAL");

        attroff(A_BOLD);
        Render::colorOff(4);

        refresh();
        animStep++;
        return;
    }

    int boxW = 74;
    int boxH = 20;
    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;

    Render::colorOn(6);
    drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    drawSparkle(startY, startX, boxH, boxW, animStep);

    // title animation 
    const char* titleLine0 =
        " ____   ___  __  __ ____  _____ ____   ____ ___ ____  _     ";
    int titleW = Render::strLen(titleLine0);

    int titleX = centerX(startX, boxW, titleW);
    int titleY = startY + 1;

    // when true -> bold title
    bool breathe = ((animStep / 10) % 2) == 0; 

    Render::colorOn(6);
    if (breathe){
        attron(A_BOLD);
    }
    drawTitle(titleY, titleX);
    if (breathe){
        attroff(A_BOLD);
    }
    Render::colorOff(6);

    // heart (<3) position
    int heartLeftX = titleX - 4;
    int heartRightX = titleX + titleW + 2;

    if (heartLeftX < startX + 2){
        heartLeftX = startX + 2;
    }
    if (heartRightX > startX + boxW - 4){
        heartRightX = startX + boxW - 4;
    }

    drawHearts(titleY, heartLeftX, heartRightX, breathe);

    // this is for dots animation (...)
    int phase = (animStep / 24) % 3; 
    dots = phase;

    const char* dotStr;

    if (dots == 0) {
        dotStr = ".";
    }

    else if (dots == 1) {
        dotStr = "..";
    }

    else {
        dotStr = "...";
    }

    // hint text
    Render::colorOn(7);
    mvprintw(startY + 7, startX + 22, 
    "Use ARROWS + ENTER to select%s", dotStr);
    mvprintw(startY + 8, startX + 22, 
    "Press Q to quit");
    Render::colorOff(7);

    
    int baseRow = startY + 11;
    for (int i = 0; i < ITEM_COUNT; i++) {
        int row = baseRow + i * 2;

        if (i == selectedItem) {
            Render::colorOn(6); 

            attron(A_REVERSE | A_BOLD);
            mvprintw(row, startX + 24, " > %-18s < ", items[i]);
            attroff(A_REVERSE | A_BOLD);
            Render::colorOff(6);

        } else {
            Render::colorOn(1);

            mvprintw(row, startX + 24, "   %-18s   ", items[i]);
            Render::colorOff(1);
        }
    }
    Render::colorOn(7);
    mvprintw(startY + boxH - 2, startX + 2,
    "[UP/DOWN] Move   [ENTER] Select   [Q] Quit   [C] Credits");
    Render::colorOff(7);

    refresh();

    animStep++;
}


void Render::shutdown() {
    endwin();
}

void Render::drawHudInBox( int boxX, int boxY, int boxW,
int levelIndex, int timeLeft, int score, int lives) {
    char buf[128];
    
    sprintf(buf,
        "LEVEL %d   TIME %d   SCORE %d   LIVES %d",
        levelIndex + 1,
        timeLeft,
        score,
        lives
    );

    int len = Render::strLen(buf);

    int x = boxX + (boxW - len) / 2;

    if (x < boxX + 1){
        x = boxX + 1;
    }

    int y = boxY + 1;

    Render::colorOn(7);
    attron(A_BOLD);
    mvprintw(y, x, "%s", buf);
    attroff(A_BOLD);
    Render::colorOff(7);
}

static int blinkPeriodFromInvuln(int invulnLeft, int invulnTotal) {
    if(invulnTotal == 0){
        return 10; // fallback
    }
    
    int progress = invulnTotal - invulnLeft;
    if (progress < 0){
        progress = 0;
    }
    if (progress > invulnTotal){
        progress = invulnTotal;
    }

    // starting with 12 tick blink
    int p = 12 - (progress * 7) / invulnTotal;
    if (p < 5){
        p = 5; // capping the blink to 5 frames
    }
    return p;
}

static bool blinkPhase(int animFrame, int period) {
    int half = period / 2; // half times blink
    if (half < 1){
        half = 1;
    }
    return ((animFrame / half) % 2) == 0;
}

static int powerUpColor(PowerUp::Type t) {
    switch (t) {
        case PowerUp::STAR:       
            return 7; // cyan
        case PowerUp::GUNPOWDER:  
            return 3; // yellow
        case PowerUp::SPEED:      
            return 5; // green
        case PowerUp::SCORE_UP:   
            return 6; // pink
        default:                  
            return 1;
    }
    return 1; // avoid warnings
}

static int enemyColor(Enemy::Kind k){
    switch(k){
        case Enemy::Kind::WALKER: 
            return 5;
        case Enemy::Kind::PATROLLER:
            return 3;
        case Enemy::Kind::CHASER:
            return 4;
        default:
            return 1; 
    }
    return 1; // same as powerup
}

void Render::draw(const Board& board, const Player& player,
const list<Enemy*>& enemies, const list<Bomb*>& bombs,
int timeLeft, int score, int lives, int levelIndex,
const list<PowerUp*>& powerUps) {
    // static variable used for 
    // invulnerability blink
    static int animFrame = 0;
    animFrame++;

    erase();

    const int tileW = 2;
    const int hudH = 3;
    const int padY= 1;
    const int padX = 2;

    const int bw = (int)board.getWidth();
    const int bh = (int)board.getHeight();

    const int boardPixelW = bw * tileW;
    const int boardPixelH = bh;

    const int frameW = boardPixelW + 2; // this: |
    const int frameH = boardPixelH + 2; /// this: -

    const int totalH = hudH + padY + frameH;
    const int totalW = frameW + padX * 2;

    // terminal size error
    if (LINES < totalH + 2 || COLS < totalW + 2) {
        Render::colorOn(4);
        attron(A_BOLD);
        const char* msg = "RESIZE TERMINAL";
        mvprintw(
            LINES / 2, 
            (COLS - (int)Render::strLen(msg)) / 2,
            "%s", 
            msg
        );
        attroff(A_BOLD);
        Render::colorOff(4);
        refresh();
        return;
    }

    int startY = (LINES - totalH) / 2;
    int startX = (COLS  - totalW) / 2;

    int hudBoxW = frameW;
    int hudBoxH = 3;
    int hudBoxX = startX + padX; 
    int hudBoxY = startY;

    Render::colorOn(7);
    Render::drawAsciiBox(hudBoxY, hudBoxX, hudBoxH, hudBoxW);
    Render::colorOff(7);

    drawHudInBox(
        hudBoxX, hudBoxY, hudBoxW,
        levelIndex,
        timeLeft,
        score,
        lives
    );

    int frameY = startY + hudH + padY;
    int frameX = startX + padX;


    Render::colorOn(6);
    //this is the pink box
    Render::drawAsciiBox(frameY, frameX, frameH, frameW);
    Render::colorOff(6);

    int boardY = frameY + 1;
    int boardX = frameX + 1;

    //printing the board
    for (int y = 0; y < bh; y++) {
        for (int x = 0; x < bw; x++) {
            Board::CellType c = board.getCell(
                (unsigned short)x, (unsigned short)y
            );

            char ch1 = ' ';
            char ch2 = ' ';
            int color = 1;

            switch (c) {
                case Board::CellType::EMPTY:
                    ch1 = ' '; ch2 = ' ';
                    color = 1;
                    break;

                case Board::CellType::WALL_SOLID:
                    ch1 = '#'; ch2 = '#';
                    color = 2;
                    break;

                case Board::CellType::WALL_DESTRUCTIBLE:
                    ch1 = '+'; ch2 = '+';
                    color = 3;
                    break;

                case Board::CellType::EXPLOSION:
                    ch1 = '*'; ch2 = '*';
                    color = 4;
                    break;

                case Board::CellType::GATE_NEXT:
                    ch1 = '>'; ch2 = '>';
                    color = 7;
                    break;

                case Board::CellType::GATE_PREV:
                    ch1 = '<'; ch2 = '<';
                    color = 7;
                    break;

                default:
                    ch1 = '?'; ch2 = '?';
                    color = 1;
                    break;
            }

            int sx = boardX + x * tileW;
            int sy = boardY + y;

            Render::colorOn(color);
            mvaddch(sy, sx, ch1);
            mvaddch(sy, sx + 1, ch2);
            Render::colorOff(color);
        }
    }

    /**
     * @brief lambda function used to print entities with 2w
     * 
     */
    auto drawEntity = [&](unsigned short ex, 
    unsigned short ey, char c, int colorPair) {
        if (ex >= (unsigned short)bw || 
        ey >= (unsigned short)bh){
            return;
        }
        int sx = boardX + (int)ex * tileW;
        int sy = boardY + (int)ey;

        Render::colorOn(colorPair);
        mvaddch(sy, sx, c);
        mvaddch(sy, sx + 1, ' '); 
        Render::colorOff(colorPair);
    };

    //show bombs
    for (unsigned int i = 0; i < bombs.len(); i++) {
        Bomb* b = bombs.at(i);
        if (!b || !b->isPlaced()) continue;
        drawEntity(b->getX(), b->getY(), 'O', 8);
    }

    // show enemies
    for (unsigned int i = 0; i < enemies.len(); i++) {
        Enemy* e = enemies.at(i);
        if (!e) continue;
        drawEntity(e->getX(), e->getY(), 'X', enemyColor(e->kind()));
    }

    for (unsigned int i = 0; i < powerUps.len(); i++) {
        PowerUp* pw = powerUps.at(i);
        if (!pw) continue;
        drawEntity(pw->getX(), pw->getY(), '!', powerUpColor(pw->getType()));
    }

    // show player
    bool inv = player.isInvulnerable();
    if (inv) { // inv blink
        int period = blinkPeriodFromInvuln(player.getInvulnTicks(), 
        player.getInvulnTotal());
        bool phase = blinkPhase(animFrame, period);

        if (phase) attron(A_BOLD);
        else       attron(A_DIM);

        drawEntity(player.getX(), player.getY(), '@', 5);

        if (phase) attroff(A_BOLD);
        else       attroff(A_DIM);
    } else {
        drawEntity(player.getX(), player.getY(), '@', 5);
    }

    refresh();
}

void Render::drawNameEntry(const char name[4], int selectedIndex,
int score, bool victory) {
    erase();

    const int boxW = 60;
    const int boxH = 16;
    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;
    if (startY < 0) startY = 0;
    if (startX < 0) startX = 0;

    Render::colorOn(6);
    drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    Render::colorOn(6);
    attron(A_BOLD);
    if (victory) {
        mvprintw(startY + 1, startX + 18, "YOU WIN!");
    } else {
        mvprintw(startY + 1, startX + 16, "GAME OVER");
    }
    attroff(A_BOLD);
    Render::colorOff(6);

    Render::colorOn(7);
    mvprintw(startY + 3, startX + 18, "SCORE: %d", score);
    Render::colorOff(7);

    mvprintw(startY + 5, startX + 8, 
    "ENTER YOUR INITIALS");
    mvprintw(startY + 6, startX + 8, 
    "UP/DOWN change letter  LEFT/RIGHT move");
    mvprintw(startY + 7, startX + 8,
     "ENTER confirm          Q cancel");

    int lettersY = startY + 10;
    int lettersX = startX + 24;

    for (int i = 0; i < 3; i++) {
        if (i == selectedIndex) {
            Render::colorOn(6);
            attron(A_REVERSE | A_BOLD);
            mvaddch(lettersY, lettersX + i * 4, name[i]);
            attroff(A_REVERSE | A_BOLD);
            Render::colorOff(6);
        } else {
            Render::colorOn(6);
            attron(A_BOLD);
            mvaddch(lettersY, lettersX + i * 4, name[i]);
            attroff(A_BOLD);
            Render::colorOff(6);
        }

        mvprintw(lettersY + 1, lettersX + i * 4 - 1, "___");
    }

    refresh();
}

void Render::handleResize() {
    static int lastH = -1;
    static int lastW = -1;

    int h, w;
    getmaxyx(stdscr, h, w);

    // if terminal is sending KEY_RESIZE
    // but size is the same, flush the input
    if (h == lastH && w == lastW) {
        flushinp(); //input flushed
        return;
    }

    lastH = h;
    lastW = w;

#ifdef NCURSES_VERSION
    resizeterm(h, w);
#endif

    clearok(stdscr, TRUE);
    erase();
    refresh();

    flushinp(); // flush resize spam
}

void Render::drawLeaderboardEmpty() {
    erase();

    const int boxW = 60;
    const int boxH = 10;
    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;

    Render::colorOn(6);
    Render::drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    Render::colorOn(6);
    attron(A_BOLD);
    Render::safeCenterPrint(startY + 2, 
    "LEADERBOARD");
    attroff(A_BOLD);
    Render::colorOff(6);

    Render::colorOn(7);
    Render::safeCenterPrint(startY + 5, 
    "Leaderboard is empty.");
    Render::safeCenterPrint(startY + 7, 
    "Press ENTER to go back.");
    Render::colorOff(7);

    refresh();
}

void Render::drawLeaderboardAskCount(int maxEntries, int current) {
    erase();

    const int boxW = 60;
    const int boxH = 12;
    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;

    Render::colorOn(6);
    Render::drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    Render::colorOn(6);
    attron(A_BOLD);
    Render::safeCenterPrint(startY + 2, "LEADERBOARD");
    attroff(A_BOLD);
    Render::colorOff(6);

    Render::colorOn(7);
    Render::safeCenterPrint(startY + 5, 
    "How many entries to show?");
    Render::safeCenterPrint(startY + 6, 
    "Use UP/DOWN, ENTER to confirm");
    Render::colorOff(7);

    Render::colorOn(6);
    attron(A_BOLD);
    mvprintw( 
        startY + 8, 
        startX + (boxW - 10) / 2, 
        "[ %2d / %2d ]", 
        current, 
        maxEntries
    );
    attroff(A_BOLD);
    Render::colorOff(6);

    Render::colorOn(7);
    Render::safeCenterPrint(startY + boxH - 2, "ESC/Q to go back");
    Render::colorOff(7);

    refresh();
}

static void appendNumber(char* out, int& idx, int v) {
    char tmp[10];
    int len = 0;

    if (v == 0) {
        tmp[len++] = '0';
    } else {
        while (v > 0) {
            tmp[len++] = (char)('0' + (v % 10));
            v /= 10;
        }
    }

    // reverse
    for (int i = len - 1; i >= 0; i--) {
        out[idx++] = tmp[i];
    }
}


void Render::drawLeaderboard(const Leaderboard& lb,
 int maxToShow, int page) {
    erase();

    int total = lb.size();
    if (total <= 0) {
        Render::drawLeaderboardEmpty();
        return;
    }

    // clamp maxToShow
    if (maxToShow < 1){
        maxToShow = 1;
    }
    if (maxToShow > total){
        maxToShow = total;
    }

    const int PAGE_SIZE = 5;

    int pages = (maxToShow + PAGE_SIZE - 1) / PAGE_SIZE;

    if (pages < 1){
        pages = 1;
    }

    if (page < 0){
        page = 0;
    }

    if (page >= pages){
        page = pages - 1;
    }

    int startIndex = page * PAGE_SIZE;
    int endIndex = startIndex + PAGE_SIZE;

    if (endIndex > maxToShow){
        endIndex = maxToShow;
    }

    int n = endIndex - startIndex;

    const int boxW = 60;
    int boxH = 9 + n;

    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;

    Render::colorOn(6);
    Render::drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    Render::colorOn(6);
    attron(A_BOLD);
    Render::safeCenterPrint(startY + 1, "LEADERBOARD");
    attroff(A_BOLD);
    Render::colorOff(6);

    Render::colorOn(7);
    mvprintw(startY + 3, startX + 4, "#   NAME   SCORE");
    Render::colorOff(7);

    int rowY = startY + 5;
    for (int i = 0; i < n; i++) {
        const Leaderboard::ScoreEntry& e = lb.at(startIndex + i);
        int rank = startIndex + i + 1;

        Render::colorOn(1);
        mvprintw(
            rowY + i, 
            startX + 4, 
            "%2d  %c%c%c    %d",
            rank,
            e.name[0], 
            e.name[1], 
            e.name[2],
            e.score
        );
        Render::colorOff(1);
    }

    int lastRowY = rowY + n - 1;

    Render::colorOn(7);

    // footer: "Showing X/Y   Page a/b"

    char footer[64];
    int idx = 0;

    footer[idx++] = 'S'; 
    footer[idx++] = 'h'; 
    footer[idx++] = 'o';
    footer[idx++] = 'w'; 
    footer[idx++] = 'i'; 
    footer[idx++] = 'n';
    footer[idx++] = 'g'; 
    footer[idx++] = ' ';

    appendNumber(footer, idx, maxToShow);
    footer[idx++] = '/';
    appendNumber(footer, idx, total);

    footer[idx++] = ' ';
    footer[idx++] = ' ';
    footer[idx++] = 'P'; 
    footer[idx++] = 'a'; 
    footer[idx++] = 'g';
    footer[idx++] = 'e'; 
    footer[idx++] = ' ';

    appendNumber(footer, idx, page + 1);
    footer[idx++] = '/';
    appendNumber(footer, idx, pages);

    footer[idx] = '\0';

    Render::safeCenterPrint(lastRowY + 2, footer);
    Render::safeCenterPrint(
        lastRowY + 3,
        "LEFT/RIGHT page  |  ENTER/Q back"
    );

    Render::colorOff(7);

    refresh();
}

void Render::drawCredits() {
    erase();

    const int boxW = 70;
    const int boxH = 14;

    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;

    Render::colorOn(6);
    Render::drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    Render::colorOn(6);
    attron(A_BOLD);
    Render::safeCenterPrint(startY + 1, "CREDITS");
    attroff(A_BOLD);
    Render::colorOff(6);

    Render::colorOn(7);
    Render::safeCenterPrint(
        startY + 3,
        "University of Bologna - Computer Science (6640)"
    );
    Render::colorOff(7);

    int tableY = startY + 6;
    int colName = startX + 6;
    int colID   = startX + boxW - 18;

    Render::colorOn(7);
    mvprintw(tableY, colName, "NAME                     SURNAME");
    mvprintw(tableY, colID,   "ID");
    Render::colorOff(7);

    Render::colorOn(1);
    mvprintw(tableY + 2, colName, "Martina Lisa Saffo        Ramponi");
    mvprintw(tableY + 2, colID,   "0001220008");

    mvprintw(tableY + 3, colName, "Martina                   Nazzareni ");
    mvprintw(tableY + 3, colID,   "0001223089");

    mvprintw(tableY + 4, colName, "---------                 ---------  ");
    mvprintw(tableY + 4, colID,   "---------");
    Render::colorOff(1);

    Render::colorOn(7);
    Render::safeCenterPrint(
        startY + boxH - 2,
        "Press ENTER or Q to go back"
    );
    Render::colorOff(7);

    refresh();
}

void Render::drawBonusError(const char* msg){
    erase();

    const int boxW = 74;
    const int boxH = 14;

    int startY = (LINES - boxH) / 2;
    int startX = (COLS  - boxW) / 2;

    if (startY < 0){
        startY = 0;
    }

    if (startX < 0){
        startX = 0;
    }

    Render::colorOn(6);
    Render::drawAsciiBox(startY, startX, boxH, boxW);
    Render::colorOff(6);

    Render::colorOn(6);
    attron(A_BOLD);
    Render::safeCenterPrint(startY + 2, "BONUS MODE ERROR");
    attroff(A_BOLD);
    Render::colorOff(6);

    
    const int maxTextW = boxW - 6;
    char line1[128];
    char line2[128];

    line1[0] = '\0';
    line2[0] = '\0';

    if (msg == NULL) {
        msg = "error placeholder";
    }

    int len = Render::strLen(msg);

    if (len <= maxTextW) {
        int i = 0;
        for (; msg[i] != '\0' && i < (int)sizeof(line1) - 1; i++) {
            line1[i] = msg[i];
        }
        line1[i] = '\0';
    } else {
        int cut = maxTextW;

        while (cut > 0 && msg[cut] != ' ' && msg[cut] != '\0') {
            cut--;
        }

        if (cut <= 0) {
            cut = maxTextW;
        }

        int i = 0;
        for (; i < cut && msg[i] != '\0' &&
         i < (int)sizeof(line1) - 1; i++) {
            line1[i] = msg[i];
        }
        line1[i] = '\0';

        // skip spaces
        int j = cut;
        while (msg[j] == ' '){
            j++;
        }


        int k = 0;
        while (msg[j] != '\0' && k < maxTextW &&
        k < (int)sizeof(line2) - 1) {
            line2[k++] = msg[j++];
        }
        line2[k] = '\0';
    }

    Render::colorOn(4);
    Render::safeCenterPrint(startY + 6, line1);
    if (line2[0] != '\0') {
        Render::safeCenterPrint(startY + 7, line2);
    }
    Render::colorOff(4);


    Render::colorOn(7);
    Render::safeCenterPrint(startY + boxH - 3, 
    "Press ENTER / Q to go back");
    Render::colorOff(7);

    refresh();

}
