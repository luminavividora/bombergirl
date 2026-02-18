/**
 * @file name_entry.cpp
 * @author Martina Nazzareni (0001223089)
 * @brief name_entry.hpp implementation
 * @version 1.0
 * @date 2026-02-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "name_entry.hpp"

/**
 * @brief clamp an integer value to an 
 * uppercase alphabet character
 *
 * converts any integer value into a 
 * letter between 'A' and 'Z'
 * 
 * negative values are wrapped 
 * correctly using modulo arithmetic
 *
 * examples:
 * 
 *  0  -> 'A'
 * 
 *  25 -> 'Z'
 * 
 *  26 -> 'A'
 * 
 *  -1 -> 'Z'
 *
 * @param v integer value to convert
 * @return uppercase character in range 'A' to 'Z'
 */
inline char clampChar(int v) {
    if (v < 0){
        // make negative values 
        // positive before applying modulo
        v += 26 * ((-v / 26) + 1);
    }
    v %= 26;
    return (char)('A' + v);
}

bool NameEntry::run(char outName[4], int score, bool victory) {
    outName[0] = 'A';
    outName[1] = 'A';
    outName[2] = 'A';
    outName[3] = '\0';

    int idx = 0;
    int vals[3] = {0,0,0}; 

    nodelay(stdscr, FALSE);
    keypad(stdscr, TRUE);

    while (true) {
        Render::drawNameEntry(outName, idx, score, victory);

        int ch = getch();

        if (ch == 'q' || ch == 'Q') {
            nodelay(stdscr, TRUE);
            return false;
        }

        switch (ch) {
            case KEY_LEFT:
                idx--;
                if (idx < 0) idx = 2;
                break;

            case KEY_RIGHT:
                idx++;
                if (idx > 2) idx = 0;
                break;

            case KEY_UP:
                vals[idx] = (vals[idx] + 1) % 26;
                outName[idx] = clampChar(vals[idx]);
                break;

            case KEY_DOWN:
                vals[idx] = (vals[idx] - 1);
                if (vals[idx] < 0) vals[idx] += 26;
                outName[idx] = clampChar(vals[idx]);
                break;

            case 10:
            case KEY_ENTER:
                nodelay(stdscr, TRUE);
                return true;

            default:
                break;
        }
    }
}
