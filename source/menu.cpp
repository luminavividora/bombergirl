/**
 * @file menu.cpp
 * @author Martina Nazzareni (0001223089)
 * @brief menu.hpp implementation
 * @version 1.0
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "menu.hpp"
#include "render.hpp"
#include <ncurses.h>

Menu::Menu() : selectedItem(0) {}

void Menu::resetCheat() {
    this->cheatLen = 0; 
    this->cheatBuf[0] = '\0'; 
}

static bool isLetter(int ch) {
    return (ch >= 'A' && ch <= 'Z') 
    || (ch >= 'a' && ch <= 'z');
}

static char toUpperChar(int ch) {
    if (ch >= 'a' && ch <= 'z'){
        return (char)(ch - 'a' + 'A');
    }
    return (char)ch;
}

static bool matchBombergirl(const char* buf, int len) {
    const char* secret = "BOMBERGIRL";

    if (len < 10){
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (buf[len - 10 + i] != secret[i]){
            return false;
        }
    }
    return true;
}

Menu::Result Menu::run() {
    nodelay(stdscr, TRUE);

    while (true) {
        Render::drawMenu(selectedItem);

        int ch = getch();
        if (ch == ERR) {
            napms(30);
            continue;
        }

        if (ch == KEY_RESIZE) {
            Render::handleResize();

            // debounce resize events
            int drain;
            while ((drain = getch()) == KEY_RESIZE) { /*flush*/ }

            if (drain != ERR) {
                ungetch(drain);
            }

            continue;
        }

        if (isLetter(ch)) {
            char up = toUpperChar(ch);

            if (cheatLen < 15) {
                this->cheatBuf[cheatLen++] = up;
                this->cheatBuf[cheatLen] = '\0';
            } else {
                for (int i = 0; i < 14; i++){
                    cheatBuf[i] = cheatBuf[i + 1];
                }
                this->cheatBuf[14] = up;
                this->cheatBuf[15] = '\0';
                this->cheatLen = 15;
            }

            if (matchBombergirl(this->cheatBuf, this->cheatLen)) {
                this->resetCheat();
                return Menu::Result::BONUS;
            }
        } else {
            if (ch == 27 /*ESC*/ || ch == KEY_ENTER || ch == 10) {
                this->resetCheat();
            }
        }

        switch (ch) {
            case 'q':
            case 'Q':
                return Menu::Result::QUIT;
            case 'c':
            case 'C':
                return Menu::Result::CREDITS;
            case KEY_UP:
                this->selectedItem--;
                if (this->selectedItem < 0){
                    selectedItem = 2;
                }
                break;

            case KEY_DOWN:
                this->selectedItem++;
                if (this->selectedItem > 2){
                    this->selectedItem = 0;
                }
                break;

            case 10: 
            case KEY_ENTER:
                if (selectedItem == 0) {
                    return Menu::Result::START;
                }
                if (selectedItem == 1) {
                    return Menu::Result::LEADERBOARD;
                }
                if (selectedItem == 2) {
                    return Menu::Result::QUIT;
                }
                break;

            default:
                break;
        }
        napms(30);
    }
}

