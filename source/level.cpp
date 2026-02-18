/**
 * @file level.cpp
 * @author Martina Nazzareni (0001223089)
 * @brief level.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "level.hpp"

Level::Level(Player* p, unsigned short w, 
unsigned short h, Map& map) : 
board(w, h), player(p) {
    if (player == nullptr) {
        throw Level::InvalidPlayerException();
    }
    
    rng.setSeed(Random::newSeed());

    load(map);
}

bool Level::isCompleted() const {
    return this->completed;
}

Board::CellType Level::tileToCell(_TileType t) {
    switch (t) {
        case _TileType::EMPTY:
            return Board::CellType::EMPTY;
        case _TileType::SOLID: 
            return Board::CellType::WALL_SOLID;
        case _TileType::DESTRUCTIBLE:
            return Board::CellType::WALL_DESTRUCTIBLE;
        case _TileType::GATE_NEXT: 
            return Board::CellType::GATE_NEXT;
        case _TileType::GATE_PREV:
            return Board::CellType::GATE_PREV;
        default:  
            return Board::CellType::EMPTY;
    }
}

void Level::load(Map& map){

    this->player->setBoard(&board);

    this->completed = false;

    this->board.clear();

    for (unsigned short y = 0; y < map.height(); y++) {
        for (unsigned short x = 0; x < map.width(); x++) {
            _Tile t = map.at(x,y);

            if(t.getType() == _TileType::PATROLLER){
                this->enemies.push_back(new Patroller(
                    &this->board
                    , t.getStartX(),
                    t.getStartY(),
                    t.getEndX(),
                    t.getEndY(),
                    t.getSpeed()        
                ));
            } else if (t.getType() == _TileType::WALKER) {
                this->enemies.push_back(new Walker(
                    &this->board,
                    t.getStartX(),
                    t.getStartY(),
                    Random::newSeed(),
                    t.getSpeed()  
                ));
            } else if (t.getType() == _TileType::CHASER) {
                this->enemies.push_back(new Chaser(
                    &this->board,
                    this->player,
                    t.getStartX(),
                    t.getStartY(),
                    Random::newSeed(),
                    t.getSpeed()
                ));                
            } else if (t.getType()== _TileType::SPAWN){
                this->spawnX = x;
                this->spawnY = y;
            } else if (t.getType() == _TileType::GATE_NEXT) {
                board.setCell(x, y, Board::CellType::GATE_NEXT);
                gateNextX = x; gateNextY = y;
                hasGateNext = true;
            } else if (t.getType() == _TileType::GATE_PREV) {
                board.setCell(x, y, Board::CellType::GATE_PREV);
                gatePrevX = x; gatePrevY = y;
                hasGatePrev = true;
            } else {
                this->board.setCell(x,y,Level::tileToCell(t.getType()));
            }
        }
    }
}

void Level::update() {

    for (unsigned int i = 0; i < enemies.len(); i++) {
        Enemy* e = enemies.at(i);
        e->update();
    }

    unsigned short px = player->getX();
    unsigned short py = player->getY();

    for (unsigned int i = 0; i < this->enemies.len(); i++) {
        Enemy* e = this->enemies.at(i);
        if (e->getX() == px && e->getY() == py) {
            this->player->takeDamage();
                break;
        }
    }

    for (unsigned int i = 0; i < this->bombs.len(); i++) {
        Bomb* b = this->bombs.at(i);
        b->update(this->enemies, this->powerUps, this->rng);
    }

    for (unsigned int i = 0; i < this->bombs.len(); ) {
        Bomb* b = this->bombs.at(i);
        if (!b->isPlaced()) {
            this->bombs.remove(i);
            delete b;
        } else {
            i++;
        }
    }

    for (unsigned int i = 0; i < powerUps.len(); ) {
        PowerUp* pu = powerUps.at(i);
        if (!pu) { 
            powerUps.remove(i); 
            continue; 
        }

        pu->update();

        // give powerup effect and remove from list
        if (pu->getX() == player->getX() && pu->getY() == player->getY()) {
            pu->apply(*player);
            delete pu;
            powerUps.remove(i);
            continue;
        }

        // despawn if expired
        if (pu->isExpired()) {
            delete pu;
            this->powerUps.remove(i);
            continue;
        }

        i++;
    }

    Board::CellType under = this->board.getCell(px, py);

    if (under == Board::CellType::GATE_PREV) {
        this->transition = Level::TransitionRequest::PREV;
        return;
    }
    if (under == Board::CellType::GATE_NEXT) {
        this->transition = Level::TransitionRequest::NEXT;
        return;
    }


    this->board.updateExplosions();
    this->player->updateInvulnerability();
    this->player->updateTimers();

}

const Board& Level::getBoard() const {
    return this->board;
}

const list<Enemy*>& Level::getEnemies() const {
    return this->enemies;
}

const list<Bomb*>& Level::getBombs() const {
    return this->bombs;
}

void Level::clearTransitionRequest() {
    this->transition = Level::TransitionRequest::NONE; 
}

void Level::placeBomb() {
    Bomb* b = nullptr;

    try {
        b = new Bomb(&board, player, 90, 
        player->getBombRange());
    } catch (...) {
        if (b != nullptr){
            delete b;
        }
        return;
    }

    if (!b->isPlaced()) {
        delete b;
        return;
    }

    this->bombs.push_back(b);
}

void Level::onEnter(Level::TransitionRequest from) {
    player->setBoard(&board);
    player->resetGateFlags();
    player->setSpawn(this->spawnX,this->spawnY);
    this->transition = Level::TransitionRequest::NONE;

    if (from == Level::TransitionRequest::NEXT) {
        player->setX(gatePrevX + 1);
        player->setY(gatePrevY);
        return;
    }

    if (from == Level::TransitionRequest::PREV) {
        player->setX(gateNextX - 1);
        player->setY(gateNextY);
        return;
    }

    player->setX(this->spawnX);
    player->setY(this->spawnY);

}

Level::TransitionRequest Level::getTransitionRequest() const {
    return this->transition;
}

bool Level::isBombAt(unsigned short x, unsigned short y) const {
    for (unsigned int i = 0; i < bombs.len(); i++) {
        Bomb* b = bombs.at(i);
        if (!b || !b->isPlaced()) continue;
        if (b->getX() == x && b->getY() == y) {
            return true;
        }
    }
    return false;
}

const list<PowerUp*>& Level::getPowerUps() const {
    return this->powerUps;
}

Level::~Level(){
    for(unsigned int i = 0; i < bombs.len(); i++){
        Bomb* b = bombs.at(i);
        if(b != nullptr){
            delete b;
        }
        b = nullptr;
    }

    for(unsigned int i = 0; i < enemies.len(); i++){
        Enemy* e = enemies.at(i);
        if(e != nullptr){
            delete e;
        }
        e = nullptr;
    }

    for(unsigned int i = 0; i < powerUps.len(); i++){
        PowerUp* pu = powerUps.at(i);
        if(pu != nullptr){
            delete pu;
        }
        pu = nullptr;        
    }
    
    this->bombs.clear();
    this->enemies.clear();
    this->powerUps.clear();
}

unsigned short Level::getSpawnX() const{
    return this->spawnX;
}

unsigned short Level::getSpawnY() const{
    return this->spawnY;
}

void Level::onExit(){
    for (unsigned int i = 0; i < this->bombs.len(); ) {
        Bomb* b = this->bombs.at(i);
        if (b != nullptr) {
            if (b->isPlaced()) {
                unsigned short bx = b->getX();
                unsigned short by = b->getY();
                try {
                    // remove the bomb tile used
                    // for bomb collision
                    if (this->board.getCell(bx, by) 
                    == Board::CellType::BOMB) {
                        this->board.setCell(
                            bx, by, 
                            Board::CellType::EMPTY
                        );
                    }
                } catch (...) { }
            }
            delete b; // delete the bomb
        }
        this->bombs.remove(i); // and remove it from list
    }  

    this->player->resetCurrentBombs();  
}

// this method is used only when DEBUG_MODE is true
// it's the K key
void Level::killAllEnemies() {
    for (unsigned int i = 0; i < enemies.len(); i++) {
        Enemy* e = enemies.at(i);
        if (e != nullptr) {
            delete e;
        }
    }
    enemies.clear();

    // level is now completed
    completed = true;
}

// another DEBUG_MODE method
void Level::destroyAllDestructibles() {
    for (unsigned short y = 0; y < this->board.getHeight(); y++) {
        for (unsigned short x = 0; x < this->board.getWidth(); x++) {
            Board::CellType c = this->board.getCell(x, y);
            if (c == Board::CellType::WALL_DESTRUCTIBLE) {
                this->board.setCell(x, y, Board::CellType::EMPTY);
            }
        }
    }
}

// DEBUG_MODE
void Level::toggleImmortality() {
    if(this->player->isInvulnerable()){
        this->player->removeInvulnerability();
    } else {
        this->player->setInvulnerability(65535);
    }
}

// DEBUG_MODE
void Level::toggleUltraspeed(){
    if(this->player->getSpeed() != 0){
        this->player->setSpeed(0);
    } else {
        this->player->setSpeed(4);
    }
}

// DEBUG_MODE
void Level::suicide(){
    this->player->takeDamage();
}
