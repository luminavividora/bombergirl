/**
 * @file bomb.cpp
 * @author Martina Nazzareni (0001223089)
 * @brief bomb.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "bomb.hpp"

void Bomb::update(list<Enemy*>& el, list<PowerUp*>& powerups, Random& rng) {
    if (!this->placed){
        return;
    } 

    if (this->timer > 0) {
        this->timer--;
    }

    if (this->timer == 0) {
        this->explode(el, powerups, rng);
    }
}

Bomb::Bomb(Board* b, Player* p, unsigned short t, unsigned short r)
: x(BOMB_INVALID), y(BOMB_INVALID), board(b),timer(t),
range(r), owner(p), placed(false) {
    if (board == nullptr){
        throw Bomb::InvalidBoardException();
    }

    if (owner == nullptr){
        throw Bomb::InvalidPlayerException();
    } 

    this->placed = this->owner->placeBomb(this->x, this->y);
}

bool Bomb::isPlaced() const{
    return this->placed;
}

void Bomb::explode(list<Enemy*>& el, 
list<PowerUp*>& powerups, Random& rng) {
    if (this->placed == false){
        return;
    }

    this->board->setCell(x, y, Board::CellType::EXPLOSION);

    this->owner->bombExploded();

    if (this->owner->getX() == this->x && this->owner->getY() == this->y) {
        this->owner->takeDamage();
    }

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (int dir = 0; dir < 4; dir++) {
        for (unsigned short r = 1; r <= this->range; r++) {
            unsigned short nx = this->x + dx[dir] * r;
            unsigned short ny = this->y + dy[dir] * r;

            try {
                
                Board::CellType cell = this->board->getCell(nx, ny);

                if(cell == Board::CellType::WALL_SOLID){
                    break;
                }

                // check for player
                if(this->owner->getX() == nx && this->owner->getY() == ny){
                    this->owner->takeDamage();               
                }

                // check for enemies

                for(unsigned int i = 0; i < el.len(); i++){
                    Enemy* e = el.at(i);
                    if(e->getX() == nx && e->getY() == ny){
                        this->owner->addScore(e->scoreValue());
                        delete e;
                        el.remove(i);
                        break;
                    }
                }

                if (cell == Board::CellType::WALL_DESTRUCTIBLE) {
                    this->owner->addScore(10);
                    if(!this->trySpawnPowerUp(nx,ny,powerups,rng)){
                        this->board->setCell(
                            nx,
                            ny,
                            Board::CellType::EXPLOSION
                        );
                    } else {
                        this->board->setCell(
                            nx,
                            ny,
                            Board::CellType::EMPTY
                        );                        
                    }
                    break;
                }

                if (cell == Board::CellType::EMPTY) {
                    this->board->setCell(
                        nx,
                        ny,
                        Board::CellType::EXPLOSION
                    );
                }

            } catch (const Board::BoardOutOfBoundsException&) {
                break;
            }
        }
    }

    this->placed = false;
    this->x = BOMB_INVALID;
    this->y = BOMB_INVALID;
}

unsigned short Bomb::getX() const {
    return this->x;
}

unsigned short Bomb::getY() const {
    return this->y;
}

bool Bomb::trySpawnPowerUp(unsigned short x, unsigned short y,
list<PowerUp*>& powerups, Random& rng) {
    // avoid double spawn in the same tile
    for (unsigned int i = 0; i < powerups.len(); i++) {
        PowerUp* p = powerups.at(i);
        if (p && p->getX() == x && p->getY() == y)
            return false;
    }

    int roll = rng.nextInt(1, 1000);

    PowerUp* p = nullptr;

    if (roll <= 880) {
        return false;
    } else if (roll <= 928) {
        p = new SpeedUp(x, y, 600, 180);
    } else if (roll <= 964) {
        p = new Gunpowder(x, y, 600, 300);
    } else if (roll <= 994) {
        p = new ScoreUp(x, y, 600, 250);
    } else {
        p = new Star(x, y, 600, 90);
    }

    if (p) {
        powerups.push_back(p);
        return true;
    }
    return false;
}
