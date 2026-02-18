/**
 * @file board.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief board.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "board.hpp"

Board::Board() : width(0), height(0), grid(nullptr) { }

Board::Board(unsigned short w, unsigned short h) : width(w), height(h) {
    this->grid = new Board::CellType*[this->height];
    this->explosionTtl = new unsigned short*[this->height];
    for (unsigned short y = 0; y < this->height; y++) {
        this->grid[y] = new Board::CellType[width];
        this->explosionTtl[y] = new unsigned short[width];
        for (unsigned short x = 0; x < this->width; x++) {
            this->grid[y][x] = Board::CellType::EMPTY;
            this->explosionTtl[y][x] = 0;
        }
    }
}

Board::~Board() {
    for (unsigned short y = 0; y < this->height; y++) {
        delete[] this->grid[y];
        delete[] this->explosionTtl[y];
    }
    delete[] this->grid;
    delete[] this->explosionTtl;
}

void Board::clear(){
    for (unsigned short y = 0; y < this->height; y++) {
        for (unsigned short x = 0; x < this->width; x++) {
            this->grid[y][x] = Board::CellType::EMPTY;
        }
    } 
}

void Board::setCell(unsigned short x, unsigned short y,
Board::CellType type){ 
    if(this->grid == nullptr) {
        throw Board::BoardNotInitializedException();
    }
    if (x >= this->width || y >= this->height) {
        throw Board::BoardOutOfBoundsException();
    }
    this->grid[y][x] = type;

    if (type == Board::CellType::EXPLOSION) {
        this->explosionTtl[y][x] = 15;
    }
}

Board::CellType Board::getCell(unsigned short x,
unsigned short y) const {
    if(this->grid == nullptr) {
        throw Board::BoardNotInitializedException();
    }
    if (x >= this->width || y >= this->height) {
        throw Board::BoardOutOfBoundsException();
    }

    return this->grid[y][x];    
}

unsigned short Board::getWidth() const {
    return this->width;
}

unsigned short Board::getHeight() const  {
    return this->height;
}

bool Board::isWalkable(unsigned short x,
unsigned short y ) const  {
    if(this->grid == nullptr) {
        throw Board::BoardNotInitializedException();
    }
    if (x >= this->width || y >= this->height) {
        throw Board::BoardOutOfBoundsException();
    }   

    return (this->grid[y][x] == Board::CellType::EMPTY ||
    this->grid[y][x] == Board::CellType::EXPLOSION || 
    this->grid[y][x] == Board::CellType::GATE_NEXT ||
    this->grid[y][x] == Board::CellType::GATE_PREV);
}

bool Board::damageTaken(unsigned short x, unsigned short y) const {
    if(this->grid == nullptr) {
        throw Board::BoardNotInitializedException();
    }
    if (x >= this->width || y >= this->height) {
        throw Board::BoardOutOfBoundsException();
    }

    Board::CellType c = this->grid[y][x];

    return (c == Board::CellType::EXPLOSION); 
}

void Board::clearExplosions() {
    if (this->grid == NULL) {
        throw Board::BoardNotInitializedException();
    }

    for (unsigned short y = 0; y < this->height; y++) {
        for (unsigned short x = 0; x < this->width; x++) {
            if (this->grid[y][x] == Board::CellType::EXPLOSION) {
                this->grid[y][x] = Board::CellType::EMPTY;
            }
        }
    }
}

void Board::updateExplosions() {
    if (!grid){
        throw Board::BoardNotInitializedException();
    }

    for (unsigned short y = 0; y < this->height; y++) {
        for (unsigned short x = 0; x < this->width; x++) {
            if (this->explosionTtl[y][x] > 0) {
                this->explosionTtl[y][x]--;

                if (this->explosionTtl[y][x] == 0) {
                    if (this->grid[y][x] == Board::CellType::EXPLOSION) {
                        this->grid[y][x] = Board::CellType::EMPTY;
                    }
                }
            }
        }
    }
}
