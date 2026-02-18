/**
 * @file enemies.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief enemies.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "enemies.hpp"

Enemy::Enemy(Board* b, unsigned short startX,
unsigned short startY, unsigned short s)
: x(startX), y(startY), board(b), speed(s), tick(0){
    if (board == nullptr) {
        throw Enemy::InvalidBoardException();
    }

    if (board->isWalkable(startX, startY) == false) {
        throw Enemy::InvalidStartingPositionException();
    }
}

unsigned short Enemy::getX() const {
    return this->x;
}

unsigned short Enemy::getY() const {
    return this->y;
}

Board* Enemy::getBoard() const {
    return this->board;
}


void Enemy::setX(unsigned short x){
    this->x = x;
}

void Enemy::setY(unsigned short y){
    this->y = y;
}

Walker::Walker(Board* b, unsigned short startX,
unsigned short startY, unsigned long long randomSeed,
unsigned short speed) 
: Enemy(b, startX, startY, speed), rng(randomSeed) { }

static int opposite(int d) {
    if (d == 0){
        return 1;
    }
    if (d == 1){
        return 0;
    }
    if (d == 2){
        return 3;
    }
    return 2;
}

void Walker::update() {
    tick++;
    if (tick < speed){
        return;
    }
    tick = 0;

    int dirs[4] = {0, 1, 2, 3};

    // shuffle
    for (int i = 0; i < 4; ++i) {
        int j = rng.nextInt(i, 3);
        int tmp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = tmp;
    }

    // don't go opposite if possible
    for (int pass = 0; pass < 2; ++pass) {
        for (int k = 0; k < 4; ++k) {
            int dir = dirs[k];

            if (pass == 0 && this->lastDir != -1 
            && dir == opposite(this->lastDir)) {
                continue;
            }

            int newX = (int)x;
            int newY = (int)y;

            if (dir == 0){
                newY--;
            } else if (dir == 1){
                newY++;
            } else if (dir == 2){
                newX--;
            } else{
                newX++;
            }

            if (newX < 0 || newY < 0){
                continue;
            }

            if (this->board->isWalkable(
            (unsigned short)newX, (unsigned short)newY)) {
                this->x = (unsigned short)newX;
                this->y = (unsigned short)newY;
                this->lastDir = dir;
                return;
            }
        }
    }
}



Patroller::Patroller(Board* b,
unsigned short sx, unsigned short sy,
unsigned short ex, unsigned short ey,
unsigned short spd) : Enemy(b, sx, sy, spd),
startX(sx), startY(sy), endX(ex), endY(ey),
forward(true) {
    if (!board->isWalkable(sx, sy)) {
        throw Enemy::InvalidStartingPositionException();
    }

    this->x = sx; 
    this->y = sy;
}

void Patroller::update() {
    this->tick++;
    if (this->tick < this->speed){
        return;
    }
    this->tick = 0;

    unsigned short tx;
    unsigned short ty;

    if (this->forward) {
        tx = this->endX;
        ty = this->endY;
    } else {
        tx = this->startX;
        ty = this->startY;
    }

    if (this->x == tx && this->y == ty) {
        this->forward = !this->forward;
        return;
    }

    int stepX = 0;
    int stepY = 0;

    if (this->x < tx){
        stepX = 1;
    } else if (this->x > tx){
        stepX = -1;
    }

    if (this->y < ty){
        stepY = 1;
    } else if (this->y > ty){
        stepY = -1;
    }

    if (stepX != 0) {
        int nx = (int)this->x + stepX;
        int ny = (int)this->y;

        if (nx >= 0 && this->board->isWalkable((
        unsigned short)nx, (unsigned short)ny)) {
            this->x = (unsigned short)nx;
            return;
        }
    }

    if (stepY != 0) {
        int nx = (int)this->x;
        int ny = (int)this->y + stepY;

        if (ny >= 0 && this->board->isWalkable(
        (unsigned short)nx,(unsigned short)ny)) {
            this->y = (unsigned short)ny;
            return;
        }
    }

    this->forward = !this->forward;
}


bool Chaser::blocksVision(Board::CellType c) const {
    if (c == Board::CellType::WALL_SOLID || 
        c == Board::CellType::WALL_DESTRUCTIBLE ||
        c == Board::CellType::BOMB ){
        return true;
    }

    return false;
}

bool Chaser::seesRow(unsigned short maxRange) const {
    if (this->player == nullptr){
        return false;
    }

    unsigned short py = this->player->getY();
    if (py != this->y){
        return false;
    }

    int px = (int)this->player->getX();
    int ex = (int)this->x;

    int dist = px - ex;
    if (dist < 0){
        dist = -dist;
    }

    if (dist > (int)maxRange){
        return false;
    }
    int step = -1;
    if(this->player->getX() > this->x){
        step = 1;
    }

    for (int x = ex + step; x != px; x += step) {
        Board::CellType c = this->board->getCell(
            (unsigned short)x, this->y
        );
        if (this->blocksVision(c)){
            return false;
        }
    }

    return true;
}

bool Chaser::seesCol(unsigned short maxRange) const {
    if (this->player == nullptr){
        return false;
    }

    unsigned short px = this->player->getX();
    if (px != this->x){
        return false;
    }

    int py = (int)this->player->getY();
    int ey = (int)this->y;

    int dist = py - ey;
    if (dist < 0){
        dist = -dist;
    }

    if (dist > (int)maxRange){
        return false;
    }

    int step = -1;
    if(this->player->getY() > this->y){
        step = 1;
    }

    for (int y = ey + step; y != py; y += step) {
        Board::CellType c = this->board->getCell(
            this->x, 
            (unsigned short)y
        );
        if (this->blocksVision(c)){
            return false;
        }
    }
    return true;
}


bool Chaser::seesPlayer(unsigned short maxRange) const {
    return seesRow(maxRange) || seesCol(maxRange);
}

void Chaser::pickNewWanderDir() {
    int d = this->rng.nextInt(0, 3);

    if (d == 0) { 
        this->wanderDx = 0;  
        this->wanderDy = -1; 
    }
    if (d == 1) { 
        this->wanderDx = 0;  
        this->wanderDy =  1; 
    }
    if (d == 2) { 
        this->wanderDx = -1; 
        this->wanderDy =  0; 
    }
    if (d == 3) { 
        this->wanderDx =  1; 
        this->wanderDy =  0; 
    }

    this->wanderHoldLeft = (unsigned short)rng.nextInt(4, 10);
}


void Chaser::wanderStep() {
    if (this->wanderHoldLeft == 0) {
        pickNewWanderDir();
    } else {
        this->wanderHoldLeft--;
    }

    if (tryMove(this->wanderDx, this->wanderDy)){
        return;
    }

    for (int i = 0; i < 6; i++) {
        this->pickNewWanderDir();
        if (this->tryMove(wanderDx, wanderDy)){
            return;
        }
    }

}

bool Chaser::tryMove(short dx, short dy) {
    int nx = (int)this->x + (int)dx;
    int ny = (int)this->y + (int)dy;

    if (nx < 0 || ny < 0){
        return false;
    }

    if (!this->board->isWalkable(
        (unsigned short)nx, (unsigned short)ny)) {
        return false;
    }

    this->x = (unsigned short)nx;
    this->y = (unsigned short)ny;
    return true;
}


void Chaser::chaseStep() {
    if (player == nullptr){
        return;
    }

    short dx = 0;
    short dy = 0;

    if (player->getX() < this->x){
        dx = -1;
    } else if (player->getX() > this->x){
        dx = 1;
    }

    if (player->getY() < this->y){
        dy = -1;
    } else if (player->getY() > this->y){
        dy = 1;
    }

    int adx = dx; // abs x
    if (adx < 0) {
        adx = -adx;
    }

    int ady = dy; // abs y
    if (ady < 0) {
        ady = -ady;
    }

    bool tryXFirst = true;
    if (adx < ady) {
        tryXFirst = false;
    }
    if (adx == ady) {
        tryXFirst = (this->rng.nextInt(0, 1) == 0);
    }

    if (tryXFirst) {
        if (dx != 0 && this->tryMove(dx, 0)){
            return;
        }
        if (dy != 0 && this->tryMove(0, dy)){
            return;
        }
    } else {
        if (dy != 0 && this->tryMove(0, dy)){
            return;
        }
        if (dx != 0 && this->tryMove(dx, 0)){
            return;
        }
    }

    this->wanderStep();
}

void Chaser::update() {
    bool see = false;

    if(!this->player->isInvulnerable()){
        see = seesPlayer(this->vision);
    }

    unsigned short desiredSpeed = this->idleSpeed;

    if(see == true){
        desiredSpeed = this->chaseSpeed;
    }

    this->speed = desiredSpeed;

    this->tick++;
    if (this->tick < this->speed){
        return;
    }
    this->tick = 0;

    if (see){
        this->chaseStep();
    } else{
        this->wanderStep();
    }
}


Chaser::Chaser(Board* b, const Player* p,
unsigned short startX, unsigned short startY,
unsigned long long seed, unsigned short speed, unsigned short vision) : 
Enemy(b, startX, startY, speed) {

    this->idleSpeed = 10;
    this->player = p;
    this->vision = vision;
    this->rng.setSeed(seed);
    this->chaseSpeed = speed;
    this->pickNewWanderDir();
}

Enemy::Kind Chaser::kind() const {
    return Enemy::Kind::CHASER;
}

Enemy::Kind Walker::kind() const {
    return Enemy::Kind::WALKER;
}

Enemy::Kind Patroller::kind() const {
    return Enemy::Kind::PATROLLER;
}

int Patroller::scoreValue() const {
    return 150;
}

int Walker::scoreValue() const {
    return 100;
}

int Chaser::scoreValue() const {
    return 300; 
}
