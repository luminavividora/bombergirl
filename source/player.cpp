/**
 * @file player.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief player.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "player.hpp"

Player::Player () : x(0), y(0),spawnx(0),spawny(0), 
lives(0), score(0), board(nullptr), maxBombs(0),
bombsPlaced(0) { }

Player::Player(unsigned short x, unsigned short y, Board* b) {
    if(b == nullptr) {
        throw Player::InvalidBoardException();
    }

    if (b->isWalkable(x, y) == false) {
        throw Player::InvalidStartingPositionException();
    }

    this->x = x;
    this->y = y;
    this->board = b;

    this->spawnx = x;
    this->spawny = y;

}

Player::Player(unsigned short x, unsigned short y,
int lives, unsigned short score,
unsigned short maxBombs, unsigned short bombsPlaced, Board* b) {
    if (b == nullptr) {
        throw Player::InvalidBoardException();
    }

    if (b->isWalkable(x, y) == false) {
        throw Player::InvalidStartingPositionException();
    }

    this->board = b;
    this->x = x;
    this->y = y;
    this->lives = lives;
    this->score = score;
    this->maxBombs = maxBombs;
    this->bombsPlaced = bombsPlaced;

    this->spawnx = x;
    this->spawny = y;
}

void Player::moveDown() {
    unsigned short nx = this->x;
    unsigned short ny = this->y + 1;

    if (!this->board->isWalkable(nx, ny)){
        return;
    }

    // gate event
    Board::CellType under = this->board->getCell(nx, ny);
    if (under == Board::CellType::GATE_NEXT){
        this->reachedNextGate = true;
    }
    if (under == Board::CellType::GATE_PREV){
        this->reachedPrevGate = true;
    }

    this->x = nx;
    this->y = ny;

    this->lastMoveDirection = Player::Direction::DOWN;


    this->moveTick = this->getSpeed();
}

void Player::moveLeft() {
    unsigned short nx = this->x - 1;
    unsigned short ny = this->y;

    if (!this->board->isWalkable(nx, ny)){
        return;
    }

    Board::CellType left = this->board->getCell(nx, ny);
    if (left == Board::CellType::GATE_NEXT){
        this->reachedNextGate = true;
    }

    if (left == Board::CellType::GATE_PREV){
        this->reachedPrevGate = true;
    }

    this->x = nx;
    this->y = ny;

    this->lastMoveDirection = Player::Direction::LEFT;

    this->moveTick = this->getSpeed();
}

void Player::moveRight() {
    unsigned short nx = this->x + 1;
    unsigned short ny = this->y;

    if (!this->board->isWalkable(nx, ny)){
        return;
    }

    Board::CellType right = this->board->getCell(nx, ny);
    if (right == Board::CellType::GATE_NEXT){
        this->reachedNextGate = true;
    }
    if (right == Board::CellType::GATE_PREV){
        this->reachedPrevGate = true;
    }

    this->x = nx;
    this->y = ny;

    this->lastMoveDirection = Player::Direction::RIGHT;

    this->moveTick = this->getSpeed();
}

void Player::moveUp() {
    unsigned short nx = this->x;
    unsigned short ny = this->y - 1;

    if (!this->board->isWalkable(nx, ny)){
        return;
    }

    Board::CellType up = this->board->getCell(nx, ny);
    if (up == Board::CellType::GATE_NEXT){
        this->reachedNextGate = true;
    }
    if (up == Board::CellType::GATE_PREV){
        this->reachedPrevGate = true;
    }

    this->x = nx;
    this->y = ny;

    this->lastMoveDirection = Player::Direction::UP;

    this->moveTick = this->getSpeed();
}

bool Player::placeBomb(unsigned short& bomb_x, unsigned short& bomb_y){
    if (this->bombsPlaced >= this->maxBombs) {
        return false;
    }

    Board::CellType ct = this->board->getCell(this->x,this->y);
    // can't place a bomb on a gate
    // on a solid wall or have multiple
    // bombs in the same tile
    if(ct == Board::CellType::GATE_NEXT ||
    ct == Board::CellType::GATE_PREV ||
    ct == Board::CellType::WALL_SOLID ||
    ct == Board::CellType::BOMB){
        return false;
    }

    // ct == Board::CellType::WALL_SOLID is always false
    // if the game has no bugs

    bomb_x = this->x;
    bomb_y = this->y;
    this->bombsPlaced++;
    this->board->setCell(this->x,this->y, Board::CellType::BOMB);
    return true;
}

void Player::bombExploded(){
    if(this->bombsPlaced > 0) {
        this->bombsPlaced--;
    }
}

void Player::addScore(unsigned int points) {
    this->score += points;
}

unsigned short Player::getX() const {
    return this->x;
}
unsigned short Player::getY() const {
    return this->y;
}

int Player::getLives() const {
    return this->lives;
}

unsigned short Player::getScore() const {
    return this->score;
}

void Player::takeDamage() {
    if (this->isInvulnerable()){
        return;
    }

    if(this->lives > 0) {
        this->lives--;

        this->x = this->spawnx;
        this->y = this->spawny;
    }

    this->invulnTicks = INVULN_TIME_TICKS;
}

Player::~Player() { }

void Player::setBoard(Board* board){
    this->board = board;
}

void Player::setX(unsigned short x){
    this->x = x;
}

void Player::setY(unsigned short y){
    this->y = y;
}

bool Player::isDead() const {
    return (this->lives <= 0);
}

void Player::setSpawn(unsigned short x, unsigned short y) {
    this->spawnx = x;
    this->spawny = y;
}

bool Player::hasReachedNextGate() const {
    return reachedNextGate; 
}

bool Player::hasReachedPrevGate() const {
    return reachedPrevGate; 
}

void Player::resetGateFlags() {
    reachedNextGate = false;
    reachedPrevGate = false;
}

void Player::setLives(int lives) {
    this->lives = lives;
}

void Player::setScore(unsigned int score) {
    this->score = score;
}

void Player::setMaxBombs(unsigned int maxBombs) {
    this->maxBombs = maxBombs;
}

void Player::setMoveCooldown(unsigned short c) {
    this->moveCooldown = c;
}

void Player::updateMovementTimer() {
    if (this->moveTick > 0){
        this->moveTick--;
    }
}

bool Player::canMove() const {
    return this->moveTick == 0;
}

void Player::updateInvulnerability() {
    if (invulnTicks > 0){
        invulnTicks--;
    }
}

bool Player::isInvulnerable() const {
    return invulnTicks > 0;
}

void Player::setInvulnerability(unsigned short ticks) {
    this->invulnTotal = ticks;
    this->invulnTicks = ticks;
}

unsigned short Player::getInvulnTotal() const {
    return this->invulnTotal;
}

unsigned short Player::getInvulnTicks() const {
    return this->invulnTicks;
}

void Player::giveSpeedBuff(unsigned short bonus, int ticks) {
    this->speedBuffLeft = ticks;
    this->speedBonus = bonus;
}

void Player::giveBombRangeBuff(unsigned short bonus, int ticks) {
    this->bombRangeBonus = bonus;
    this->bombBuffLeft = ticks;
}

void Player::updateTimers(){

    if (this->speedBuffLeft > 0) {
        this->speedBuffLeft--;
        if (this->speedBuffLeft == 0){
            this->speedBonus = 0;
        }
    }

    if (this->bombBuffLeft > 0) {
        this->bombBuffLeft--;
        if (this->bombBuffLeft == 0){
            this->bombRangeBonus = 0;
        }
    }   
}

unsigned short Player::getBombRange() const {
    return this->bombRangeBonus + this->bombRange;
}

void Player::increaseBombRange() {
    this->bombRange++;
}

unsigned short Player::getSpeed() const {
    short s = this->moveCooldown - this->speedBonus;
    if(s < 0){
        s = 0;
    }
    return (unsigned short) s;
}

void Player::resetCurrentBombs(){
    this->bombsPlaced = 0;
}

void Player::reset() {
    this->x = 0;
    this->y = 0;

    this->bombBuffLeft = 0;
    this->speedBuffLeft = 0;

    this->bombRangeBonus = 0;
    this->speedBonus = 0;

    this->invulnTicks = 0;
    this->invulnTotal = 0;

    this->bombsPlaced = 0;

    this->board = nullptr;

    this->lives = 3;

    this->moveCooldown = 4;
    this->maxBombs = 1;
    this->bombRange = 1;

    this->lastMoveDirection = DOWN;
}

void Player::removeInvulnerability(){
    this->invulnTicks = 0;
    this->invulnTotal = 0;
}

void Player::setSpeed(unsigned short speed){
    this->moveCooldown = speed;
}