/**
 * @file powerup.cpp
 * @author Martina Nazzareni (0001223089)
 * @brief powerup.hpp implementation
 * @version 1.0
 * @date 2026-02-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "powerup.hpp"

PowerUp::PowerUp(unsigned short x, unsigned short y, 
unsigned short timeLeft, unsigned short effectValue) {
    this->x = x;
    this->y = y;
    this->timeLeft = timeLeft;
    this->effectValue = effectValue;
    this->type = PowerUp::Type::NONE;
}

unsigned short PowerUp::getEffectValue() const {
    return this->effectValue;
}

unsigned short PowerUp::getTimeLeft() const {
    return this->timeLeft;
}

unsigned short PowerUp::getX() const {
    return this->x;
}
unsigned short PowerUp::getY() const {
    return this->y;
}

void PowerUp::update(){
    if(this->timeLeft > 0){
        this->timeLeft--;
    }
}

bool PowerUp::isExpired() const {
    return this->timeLeft == 0;
}

Star::Star(unsigned short x, unsigned short y, 
unsigned short timeLeft, unsigned short effectValue) : 
PowerUp(x, y, timeLeft, effectValue){
    this->type = PowerUp::Type::STAR;
}

Gunpowder::Gunpowder(unsigned short x, unsigned short y, 
unsigned short timeLeft, unsigned short effectValue) : 
PowerUp(x, y, timeLeft, effectValue){
    this->type = PowerUp::Type::GUNPOWDER;
}

SpeedUp::SpeedUp(unsigned short x, unsigned short y, 
unsigned short timeLeft, unsigned short effectValue) : 
PowerUp(x, y, timeLeft, effectValue){
    this->type = PowerUp::Type::SPEED;
}

ScoreUp::ScoreUp(unsigned short x, unsigned short y, 
unsigned short timeLeft, unsigned short effectValue) : 
PowerUp(x, y, timeLeft, effectValue){
    this->type = PowerUp::Type::SCORE_UP;
}

void Star::apply(Player& p) {
    p.setInvulnerability(this->effectValue);
}

void Gunpowder::apply(Player& p){
    p.giveBombRangeBuff(1, this->effectValue);
}

void SpeedUp::apply(Player& p){
    p.giveSpeedBuff(2, 180);
}

void ScoreUp::apply(Player& p) {
    p.addScore(this->effectValue);
}

PowerUp::Type PowerUp::getType() const{
    return this->type;
}
