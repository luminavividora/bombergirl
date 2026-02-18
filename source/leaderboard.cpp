/**
 * @file leaderboard.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief leaderboard.hpp implementation
 * @version 1.0
 * @date 2026-02-01
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "leaderboard.hpp"

static int stringToInteger(const char* s) {
    if (s == nullptr) {
        return 0;
    }

    // skip spaces
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') {
        s++;
    }

    if (*s == '\0') {
        return 0;
    }

    int value = 0;
    bool hasDigits = false;

    while (*s >= '0' && *s <= '9') {
        hasDigits = true;
        value = value * 10 + (*s - '0');
        s++;
    }

    // skip trailing spaces
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') {
        s++;
    }

    // reject garbage at the end
    if (*s != '\0') {
        return 0;
    }

    if (hasDigits) {
        return value;
    }

    return 0;
}


Leaderboard::Leaderboard(){
    this->count = 0;
}

int Leaderboard::size() const {
    return this->count;
}

void Leaderboard::add(const char name[4], int score) {
    if (this->count < Leaderboard::MAX) {
        this->entries[this->count].name[0] = name[0];
        this->entries[this->count].name[1] = name[1];
        this->entries[this->count].name[2] = name[2];
        this->entries[this->count].name[3] = '\0';
        this->entries[this->count].score = score;
        this->count++;
        this->sortDesc();
        return;
    }

    if (score <= this->entries[this->count - 1].score){
        return;
    }

    this->entries[this->count - 1].name[0] = name[0];
    this->entries[this->count - 1].name[1] = name[1];
    this->entries[this->count - 1].name[2] = name[2];
    this->entries[this->count - 1].name[3] = '\0';
    this->entries[this->count - 1].score = score;

    this->sortDesc();
}

void Leaderboard::sortDesc() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            bool swapNeeded = false;

            if (entries[j].score < entries[j + 1].score) {
                swapNeeded = true;
            } else if (entries[j].score == entries[j + 1].score) {
                if (strcmp(entries[j].name, entries[j + 1].name) > 0) {
                    swapNeeded = true;
                }
            }

            if (swapNeeded) {
                Leaderboard::ScoreEntry tmp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = tmp;
            }
        }
    }
}

bool Leaderboard::save(const char* path) const {
    std::ofstream out(path);
    if (!out.is_open()){
        return false;
    }

    for (int i = 0; i < count; i++) {
        out << entries[i].name << ";" << entries[i].score << "\n";
    }

    out.close();
    return true;
}

bool Leaderboard::load(const char* path) {
    std::ifstream in(path);
    if (!in.is_open()){
        return false;
    }

    count = 0;

    char line[64];

    while (in.getline(line, sizeof(line))) {
        if (line[0] == '\0'){
            continue;
        }

        // find ; separator
        char* sep = strchr(line, ';');
        if (sep == nullptr){
            continue;
        }

        *sep = '\0';
        char* nameStr = line;
        char* scoreStr = sep + 1;

        int score = stringToInteger(scoreStr);

        char name3[4] = {'A','A','A','\0'};
        for (int i = 0; i < 3 && nameStr[i] != '\0'; i++) {
            name3[i] = nameStr[i];
        }

        add(name3, score);
        if (count >= MAX) break;
    }

    in.close();
    return true;
}

const Leaderboard::ScoreEntry& Leaderboard::at(int i) const {
    return entries[i];
}
