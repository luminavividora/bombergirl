/**
 * @file maps.cpp
 * @author Martina Nazzareni (0001223089)
 * @brief maps.hpp implementation
 * @version 1.0
 * @date 2026-01-29
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "maps.hpp"

_Tile::_Tile(_TileType type){
    this->type = type;
}

_Tile::_Tile() {
    this->type = _TileType::EMPTY;
}

_TileType _Tile::getType() const {
    return this->type;
}

_Tile::_Tile(_TileType type, unsigned short x, 
unsigned short y){

        this->type = type;
        this->sx = x;
        this->sy = y;

}

_Tile::_Tile(_TileType type, unsigned short sx,
unsigned short sy, unsigned short ex, unsigned short ey){
        this->type = type;

        this->sx = sx;
        this->ex = ex;

        this->sy = sy;
        this->ey = ey;
}

_Tile::_Tile(_TileType type, unsigned short sx,
unsigned short sy, unsigned short ex,
unsigned short ey, unsigned short speed){
    this->ex = ex;
    this->ey = ey;
    this->speed = speed;
    this->sy = sy;
    this->sx = sx;
    this->type = type;
}

unsigned short _Tile::getStartX() const{
    return this->sx;
}

unsigned short _Tile::getEndX() const{
    return this->ex;
}

unsigned short _Tile::getStartY() const{
    return this->sy;
}

unsigned short _Tile::getEndY() const{
    return this->ey;
}

Map::Map(){
    for (unsigned short y = 0; y < MAP_HEIGHT; y++) {
        for (unsigned short x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = _Tile(_TileType::EMPTY);
        }
    }
}

void Map::spawnPatroller(unsigned short x ,unsigned short y ,
unsigned short sx, unsigned short sy,unsigned short ex, 
unsigned short ey, unsigned short speed){
    _Tile t(
        _TileType::PATROLLER,
        sx,sy,
        ex,ey,
        speed
    );

    this->grid[y][x] = t;
}

void Map::spawnChaser(unsigned short x, unsigned short y,
unsigned short speed) {
    _Tile t(
        _TileType::CHASER,
        x,y,
        0,0,
        speed
    );

    this->grid[y][x] = t;
}

void Map::spawnWalker(unsigned short x, unsigned short y,
unsigned short speed){
    _Tile t(
        _TileType::WALKER,
        x,y,
        0,0,
        speed
    );

    this->grid[y][x] = t;
}

void Map::placeTile(unsigned short x, unsigned short y, _Tile t){
    this->grid[y][x] = t;
}

bool Map::isValidFillType(_TileType t){
    return (t == _TileType::SOLID ||
    t == _TileType::DESTRUCTIBLE ||
    t == _TileType::EMPTY);
}

void Map::row(unsigned short y,
unsigned short x1, unsigned short x2,
_TileType t){
    if (!Map::isValidFillType(t)){
        return;
    }
    if (y >= MAP_HEIGHT){
        return;
    }

    if (x1 > x2) {
        unsigned short tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if (x2 >= MAP_WIDTH){
        x2 = MAP_WIDTH - 1;
    }

    for (unsigned short x = x1; x <= x2; x++) {
        this->grid[y][x] = _Tile(
            t,x,y
        );
    }
}

void Map::column(unsigned short x,
unsigned short y1, unsigned short y2,
_TileType t){
    if (!Map::isValidFillType(t)){
        return;
    }

    if (x >= MAP_WIDTH)
        return;

    if (y1 > y2) {
        unsigned short tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    if (y1 >= MAP_HEIGHT){
        return;
    }
    if (y2 >= MAP_HEIGHT){
        y2 = MAP_HEIGHT - 1;
    }

    for (unsigned short y = y1; y <= y2; ++y) {
        this->grid[y][x] = _Tile(
            t,x,y
        );
    }
}

void Map::fill(unsigned short x1, unsigned short y1,
unsigned short x2, unsigned short y2, _TileType t) {
    if (!Map::isValidFillType(t)){
        return;
    }

    if (x1 > x2) {
        unsigned short tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if (y1 > y2) {
        unsigned short tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    if (x1 >= MAP_WIDTH || y1 >= MAP_HEIGHT){
        return;
    }

    if (x2 >= MAP_WIDTH){
        x2 = MAP_WIDTH - 1;
    }
    if (y2 >= MAP_HEIGHT){
        y2 = MAP_HEIGHT - 1;
    }

    for (unsigned short y = y1; y <= y2; ++y) {
        for (unsigned short x = x1; x <= x2; ++x) {
            this->grid[y][x] = _Tile(
                t,x,y
            );
        }
    }
}

void Map::borderWalls(){
    //top and bottom
    for (unsigned short x = 0; x < MAP_WIDTH; x++) {
        this->placeTile(x, 0, _Tile(
            _TileType::SOLID,x,0
        ));
        this->placeTile(x, MAP_HEIGHT - 1, _Tile(
            _TileType::SOLID,x,MAP_HEIGHT -1
        )); 
    }

    // left and right
    for (unsigned short y = 0; y < MAP_HEIGHT; y++) {
        this->placeTile(0, y, _Tile(
            _TileType::SOLID,0,y
        ));
        this->placeTile(MAP_WIDTH - 1, y, _Tile(
            _TileType::SOLID, MAP_WIDTH - 1, y
        ));
    }
}

void Map::setSpawn(unsigned short x, unsigned short y){
    this->grid[y][x] = _Tile(
        _TileType::SPAWN,x,y
    );
}

// debug map
Map MapBuilder::outsand() {
    Map m;

    m.placeTile(MAP_WIDTH-2, MAP_HEIGHT-2, _Tile(
        _TileType::GATE_NEXT,
        MAP_WIDTH-2,
        MAP_HEIGHT-2
    ));

    m.spawnPatroller(12,6,12,6,15,6,5);

    m.spawnChaser(9,17,15);

    m.setSpawn(2,2);

 
    m.placeTile(2,4,_Tile(_TileType::DESTRUCTIBLE, 2,4));
    m.placeTile(3,4,_Tile(_TileType::DESTRUCTIBLE, 3,4));
    m.placeTile(4,4,_Tile(_TileType::DESTRUCTIBLE, 4,4));

    m.placeTile(4,2,_Tile(_TileType::DESTRUCTIBLE, 4,2)); 
    m.placeTile(4,3,_Tile(_TileType::DESTRUCTIBLE, 4,3));  

    unsigned short midY = MAP_HEIGHT / 2;

    for (unsigned short start = 2; start < MAP_WIDTH - 2; ) {
        unsigned short end = start;

        while (end < MAP_WIDTH - 2 && (end % 4) != 0) {
            end++;
        }

        if (end > start) {
            m.row(midY, start, (unsigned short)(end - 1), _TileType::DESTRUCTIBLE);
        }

        if (end >= MAP_WIDTH - 2) break;

        start = (unsigned short)(end + 1);
    }



    unsigned short colX = MAP_WIDTH - 6;

    for (unsigned short start = 3; start < MAP_HEIGHT - 3; ) {
        unsigned short end = start;

        while (end < MAP_HEIGHT - 3 && (end % 5) != 0) {
            end++;
        }

        if (end > start) {
            m.column(colX, start, (unsigned short)(end - 1), _TileType::DESTRUCTIBLE);
        }

        if (end >= MAP_HEIGHT - 3) break;

        start = (unsigned short)(end + 1);
    }


    m.spawnWalker(9,12,16);

    m.borderWalls();

    return m;
}

Map MapBuilder::crossroads() {
    Map m;

    const unsigned short W = m.width();
    const unsigned short H = m.height();

    m.fill(0, 0, W - 1, H - 1, _TileType::EMPTY);

    m.setSpawn(2, 2);

    const unsigned short cx = W / 2;
    const unsigned short cy = H / 2;

    m.column(cx, 1, H - 2, _TileType::SOLID);
    m.row(cy, 1, W - 2, _TileType::SOLID);

    m.placeTile(cx, 2, _TileType::EMPTY);
    m.placeTile(cx, H - 3, _TileType::EMPTY);
    m.placeTile(2, cy, _TileType::EMPTY);
    m.placeTile(W - 3, cy, _TileType::EMPTY);


    m.fill(3, 3, 5, 5, _TileType::SOLID);
    m.fill(W - 6, 3, W - 4, 5, _TileType::SOLID);
    m.fill(3, H - 6, 5, H - 4, _TileType::SOLID);
    m.fill(W - 6, H - 6, W - 4, H - 4, _TileType::SOLID);


    m.row(cy - 2, 2, W - 3, _TileType::DESTRUCTIBLE);

    m.placeTile(cx - 2, cy - 2, _TileType::EMPTY);
    m.placeTile(cx + 2, cy - 2, _TileType::EMPTY);

    m.row(cy + 2, 2, W - 3, _TileType::DESTRUCTIBLE);
    m.placeTile(cx - 2, cy + 2, _TileType::EMPTY);
    m.placeTile(cx + 2, cy + 2, _TileType::EMPTY);


    m.fill(2, 6, 6, 8, _TileType::DESTRUCTIBLE);
    m.fill(W - 7, 6, W - 3, 8, _TileType::DESTRUCTIBLE);
    m.fill(2, H - 9, 6, H - 7, _TileType::DESTRUCTIBLE); 
    m.fill(W - 7, H - 9, W - 3, H - 7, _TileType::DESTRUCTIBLE);

    m.placeTile(2, 3, _TileType::EMPTY);
    m.placeTile(3, 2, _TileType::EMPTY);
    m.placeTile(3, 3, _TileType::EMPTY);

    m.spawnPatroller( cx, 3,   3, 3,   W - 4, 3,   6);

    m.spawnWalker(cx - 1, cy - 1, 10);

    m.spawnChaser(cx, H - 4, 8);

    m.placeTile(W - 3, H - 3, _TileType::GATE_NEXT);

    m.borderWalls();

    return m;
}

unsigned short Map::width() const {
    return MAP_WIDTH;
}

unsigned short Map::height() const {
    return MAP_HEIGHT;
}

const _Tile& Map::at(unsigned short x, unsigned short y) const {
    return this->grid[y][x];
}

void Map::set(unsigned short x, unsigned short y, _Tile tile) {
    this->grid[y][x] = tile;
}

unsigned short _Tile::getSpeed() const {
    return this->speed;
}

Map MapBuilder::motherboard(){
    Map m;

    m.setSpawn(1,20);

    m.fill(1,17,2,18, _TileType::SOLID);
    m.fill(2,13,3,16, _TileType::DESTRUCTIBLE);
    m.fill(1,11,2,12,_TileType::SOLID);
    m.fill(2,9,5,10,_TileType::DESTRUCTIBLE);
    m.fill(2,2,5,6,_TileType::DESTRUCTIBLE);
    m.column(6,1,7,_TileType::SOLID);
    m.fill(7,6,8,7,_TileType::SOLID);
    m.fill(6,16,7,17,_TileType::DESTRUCTIBLE);
    m.row(19,6,7,_TileType::DESTRUCTIBLE);
    m.row(13,6,12,_TileType::DESTRUCTIBLE);
    m.row(14,9,12,_TileType::DESTRUCTIBLE);
    m.fill(25,12,24,13,_TileType::DESTRUCTIBLE);
    m.column(7,2,5,DESTRUCTIBLE);
    m.column(8,3,5,_TileType::DESTRUCTIBLE);
    m.column(9,3,7,_TileType::DESTRUCTIBLE);
    m.fill(11,2,12,8,_TileType::DESTRUCTIBLE);
    m.fill(14,2,16,8,_TileType::DESTRUCTIBLE);
    m.column(18,2,7,_TileType::DESTRUCTIBLE);
    m.fill(20,2,23,4,_TileType::DESTRUCTIBLE);
    m.fill(15,11,22,14,_TileType::DESTRUCTIBLE);
    m.fill(15,16,22,18,_TileType::DESTRUCTIBLE);
    m.row(15,20,22,_TileType::DESTRUCTIBLE);
    m.fill(21,7,22,10,_TileType::DESTRUCTIBLE);
    m.fill(24,14,25,20,_TileType::SOLID);
    m.placeTile(25,14, _Tile(
        _TileType::SOLID,25,14
    ));
    m.row(20,6,10,_TileType::SOLID);
    m.row(19,8,10,_TileType::SOLID);
    m.row(18,8,9,_TileType::SOLID);
    m.row(17,8,10,_TileType::SOLID);
    m.row(16,8,10,_TileType::SOLID);
    m.row(19,15,22,_TileType::SOLID);
    m.row(15,15,19,_TileType::SOLID);
    m.column(20,7,10,_TileType::SOLID);
    m.column(23,5,7,_TileType::SOLID);
    m.row(5,21,22,_TileType::SOLID);
    m.column(17,1,4,_TileType::SOLID);
    m.row(1,18,19,_TileType::SOLID);

    m.placeTile(
        25,1, _Tile(
            _TileType::GATE_NEXT,25,1
        )
    );


    m.spawnPatroller(13,11,13,11,5,11,17);
    m.spawnWalker(1,8,20);

    m.borderWalls();

    return m;
}


Map MapBuilder::ram(){


    Map m;

    m.setSpawn(1,2);

    m.placeTile(1,1, _Tile(_TileType::GATE_PREV));

    m.fill(3,1,8,2,_TileType::DESTRUCTIBLE);
    m.fill(1,7,2,11,_TileType::DESTRUCTIBLE);
    m.row(15,1,4,_TileType::DESTRUCTIBLE);
    m.row(14,3,7,_TileType::DESTRUCTIBLE);
    m.fill(3,16,4,20,_TileType::DESTRUCTIBLE);
    m.row(17,1,2,_TileType::DESTRUCTIBLE);
    m.row(19,1,2,_TileType::DESTRUCTIBLE);
    m.fill(23,5,25,9,_TileType::DESTRUCTIBLE);
    m.fill(11,15,12,17,_TileType::DESTRUCTIBLE);
    m.fill(7,7,8,13,_TileType::DESTRUCTIBLE);
    m.row(10,4,6,_TileType::DESTRUCTIBLE);
    m.row(1,17,25,_TileType::DESTRUCTIBLE);
    m.row(10,20,24,_TileType::DESTRUCTIBLE);
    m.row(14,20,25,_TileType::DESTRUCTIBLE);
    m.fill(15,7,16,14,_TileType::DESTRUCTIBLE);
    m.fill(19,2,20,3,_TileType::DESTRUCTIBLE);
    m.fill(11,9,12,12,_TileType::DESTRUCTIBLE);
    m.fill(5,18,6,19,_TileType::DESTRUCTIBLE);

    m.row(13,23,25,_TileType::DESTRUCTIBLE);
    m.row(20,12,16,_TileType::DESTRUCTIBLE);
    m.row(3,11,13, _TileType::DESTRUCTIBLE);
    m.row(6,21,22,_TileType::DESTRUCTIBLE);
    m.row(10,17,18,_TileType::DESTRUCTIBLE);
    m.row(18,12,14,_TileType::DESTRUCTIBLE);
    m.row(3,7,8,_TileType::DESTRUCTIBLE);
    m.row(19,13,14,_TileType::DESTRUCTIBLE);
    m.row(13,12,13,_TileType::DESTRUCTIBLE);
    m.row(18,21,22,_TileType::DESTRUCTIBLE);
    m.row(20,24,25,_TileType::DESTRUCTIBLE);
    m.row(4,23,24,_TileType::DESTRUCTIBLE);
    m.row(2,11,12,_TileType::DESTRUCTIBLE);
    
    m.column(4,3,4,_TileType::DESTRUCTIBLE);

    m.fill(15,2,16,3,_TileType::DESTRUCTIBLE);
    m.fill(9,18,10,19,_TileType::DESTRUCTIBLE);
    m.fill(17,18,18,19,_TileType::DESTRUCTIBLE);

    m.placeTile(7,6, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(9,7, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(9,9, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(10,11, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(10,13, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(6,20, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(9,20, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(12,1, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(15,1, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(13,5, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(15,6, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(14,7, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(17,7, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(12,8, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(14,9, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(17,9, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(13,11, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(17,11, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(17,13, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(14,15, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(14,17, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(16,19, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(25,19, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(25,15, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(25,2, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(23,3, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(21,20, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(18,20, _Tile(_TileType::DESTRUCTIBLE));


    m.fill(5,3,6,5,_TileType::SOLID);
    m.fill(5,7,6,9,_TileType::SOLID);
    m.fill(5,11,6,13,_TileType::SOLID);   
    m.fill(5,15,6,17,_TileType::SOLID);

    m.fill(9,3,10,5,_TileType::SOLID);  
    m.fill(9,15,10,17,_TileType::SOLID);  

    m.fill(17,3,18,5,_TileType::SOLID);      
    m.fill(17,15,18,17,_TileType::SOLID);   

    m.fill(21,15,22,17,_TileType::SOLID); 
    m.fill(21,11,22,13,_TileType::SOLID);
    m.fill(21,7,22,9,_TileType::SOLID);
    m.fill(21,3,22,5,_TileType::SOLID);

    m.column(1,4,6,_TileType::SOLID);
    m.column(14,3,5,_TileType::SOLID);
    m.column(13,7,9,_TileType::SOLID);
    m.column(14,11,13,_TileType::SOLID);
    m.column(10,7,9,_TileType::SOLID);
    m.column(9,11,13,_TileType::SOLID);
    m.column(18,7,9,_TileType::SOLID);
    m.column(18,11,13,_TileType::SOLID);
    m.column(13,15,17,_TileType::SOLID);

    m.row(1,13,14,_TileType::SOLID);
    m.row(16,1,2,_TileType::SOLID);
    m.row(18,1,2,_TileType::SOLID);
    m.row(20,1,2,_TileType::SOLID);
    m.row(20,7,8,_TileType::SOLID);
    m.row(20,19,20,_TileType::SOLID);

    m.placeTile(9,8,_TileType::SOLID);
    m.placeTile(10,12,_TileType::SOLID);   
    m.placeTile(5,20,_TileType::SOLID);    
    m.placeTile(10,20,_TileType::SOLID);
    m.placeTile(17,20,_TileType::SOLID);
    m.placeTile(22,20,_TileType::SOLID); 
    m.placeTile(14,16,_TileType::SOLID); 
    m.placeTile(13,12,_TileType::SOLID);  
    m.placeTile(17,12,_TileType::SOLID);
    m.placeTile(17,8,_TileType::SOLID);
    m.placeTile(14,8,_TileType::SOLID);
    m.placeTile(13,4,_TileType::SOLID);
    m.placeTile(11,1,_TileType::SOLID);
    m.placeTile(16,1,_TileType::SOLID);


    m.placeTile(21,20,_TileType::DESTRUCTIBLE);

    m.placeTile(25,12,_TileType::GATE_NEXT);

    m.spawnWalker(12,7,12);
    m.spawnPatroller(19,4,19,4,19,17,9);
    m.spawnWalker(15,15);

    m.borderWalls();

    return m;
}

Map MapBuilder::storage(){
    Map m;

    m.setSpawn(1,11);
    m.placeTile(1,12,_TileType::GATE_PREV);
    m.placeTile(25,1,_TileType::GATE_NEXT);

    m.fill(15,1,17,5,_TileType::DESTRUCTIBLE);
    m.fill(1,19,6,20,_TileType::DESTRUCTIBLE);
    m.fill(24,7,25,10,_TileType::DESTRUCTIBLE);
    m.fill(21,10,22,13,_TileType::DESTRUCTIBLE);
    m.fill(10,8,12,10,_TileType::DESTRUCTIBLE);
    m.fill(1,1,3,2,_TileType::DESTRUCTIBLE);
    m.fill(13,13,14,20,_TileType::DESTRUCTIBLE);

    m.row(16,8,12,_TileType::DESTRUCTIBLE);
    m.row(16,15,19,_TileType::DESTRUCTIBLE);
    m.row(9,3,9,_TileType::DESTRUCTIBLE);
    m.row(5,5,8,_TileType::DESTRUCTIBLE);
    m.row(16,1,5,_TileType::DESTRUCTIBLE);
    m.row(12,16,18,_TileType::DESTRUCTIBLE);    
    m.row(6,14,16,_TileType::DESTRUCTIBLE);
    m.row(20,19,21,_TileType::DESTRUCTIBLE);
    m.row(14,6,8,_TileType::DESTRUCTIBLE);
    m.row(10,4,6,_TileType::DESTRUCTIBLE);
    m.row(4,1,3,_TileType::DESTRUCTIBLE);
    m.row(11,5,6,_TileType::DESTRUCTIBLE);
    m.row(7,15,16,_TileType::DESTRUCTIBLE);
    m.row(20,7,8,_TileType::DESTRUCTIBLE);  
    m.row(11,17,18,_TileType::DESTRUCTIBLE);      
    m.row(1,9,10,_TileType::DESTRUCTIBLE);
    m.row(7,7,8,_TileType::DESTRUCTIBLE);

    m.column(5,1,4,_TileType::DESTRUCTIBLE);
    m.column(7,1,4,_TileType::DESTRUCTIBLE);
    m.column(19,1,4,_TileType::DESTRUCTIBLE);
    m.column(21,1,4,_TileType::DESTRUCTIBLE);
    m.column(23,2,4,_TileType::DESTRUCTIBLE);
    m.column(25,13,20,_TileType::DESTRUCTIBLE);
    m.column(19,17,19,_TileType::DESTRUCTIBLE);
    m.column(18,5,8,_TileType::DESTRUCTIBLE);
    m.column(13,1,5,_TileType::DESTRUCTIBLE);
    m.column(10,5,7,_TileType::DESTRUCTIBLE);
    m.column(14,10,12,_TileType::DESTRUCTIBLE);
    m.column(1,13,15,_TileType::DESTRUCTIBLE);
    m.column(1,17,18,_TileType::DESTRUCTIBLE);
    m.column(5,17,18,_TileType::DESTRUCTIBLE);
    m.column(22,8,9,_TileType::DESTRUCTIBLE);
    m.column(12,11,12,_TileType::DESTRUCTIBLE);

    m.placeTile(1,3, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(3,3, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(6,12, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(8,13, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(9,10, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(25,6, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(18,10, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(16,8, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(14,5, _Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(23,1, _Tile(_TileType::DESTRUCTIBLE));


    m.fill(9,13,12,15,_TileType::SOLID);
    m.fill(9,17,12,20,_TileType::SOLID);
    m.fill(15,17,18,20,_TileType::SOLID);
    m.fill(15,13,18,15,_TileType::SOLID);
    m.fill(2,17,4,18,_TileType::SOLID);
    m.fill(1,5,2,7,_TileType::SOLID);
    m.fill(24,11,25,12,_TileType::SOLID);
    m.fill(20,18,21,19,_TileType::SOLID);

    m.column(4,1,4,_TileType::SOLID);
    m.column(6,1,4,_TileType::SOLID);
    m.column(8,1,4,_TileType::SOLID);
    m.column(10,1,4,_TileType::SOLID);
    m.column(14,1,4,_TileType::SOLID);
    m.column(18,1,4,_TileType::SOLID);
    m.column(20,1,4,_TileType::SOLID);
    m.column(22,1,4,_TileType::SOLID);
    m.column(20,15,17,_TileType::SOLID);
    m.column(21,8,9,_TileType::SOLID);
    m.column(11,11,12,_TileType::SOLID);
    m.column(4,14,15,_TileType::SOLID);
    m.column(13,6,7,_TileType::SOLID);
    m.column(14,7,8,_TileType::SOLID);

    m.row(12,8,10,_TileType::SOLID);
    
    m.placeTile(15,8,_Tile(_TileType::SOLID));

    m.spawnChaser(23,19,10);
    m.spawnWalker(12,7,16);
    m.spawnPatroller(9,1,9,1,9,8,12);

    m.borderWalls();

    return m;
}

Map MapBuilder::cpu(){
    Map m;

    m.setSpawn(1,2);
    m.placeTile(1,1, _Tile(_TileType::GATE_PREV));
    m.placeTile(12,10,_Tile(_TileType::GATE_NEXT));

    m.fill(23,7,24,11,_TileType::DESTRUCTIBLE);
    m.fill(13,13,17,14,_TileType::DESTRUCTIBLE);
    m.fill(2,12,3,13,_TileType::DESTRUCTIBLE);
    m.fill(4,1,5,3,_TileType::DESTRUCTIBLE);
    m.fill(13,4,15,5,_TileType::DESTRUCTIBLE);
    m.fill(16,19,18,20,_TileType::DESTRUCTIBLE);
    m.fill(14,16,15,17,_TileType::DESTRUCTIBLE);
    m.fill(11,7,12,8, _TileType::DESTRUCTIBLE);
    m.fill(24,2,25,3,_TileType::DESTRUCTIBLE);

    m.column(3,4,7, _TileType::DESTRUCTIBLE);
    m.column(5,4,7, _TileType::DESTRUCTIBLE);
    m.column(6,9,11, _TileType::DESTRUCTIBLE);
    m.column(20,8,15, _TileType::DESTRUCTIBLE);
    m.column(1,15,17, _TileType::DESTRUCTIBLE);
    m.column(21,15,17, _TileType::DESTRUCTIBLE);
    m.column(17,7,12, _TileType::DESTRUCTIBLE);
    m.column(14,7,12, _TileType::DESTRUCTIBLE);
    m.column(8,12,14, _TileType::DESTRUCTIBLE);
    m.column(5,15,17, _TileType::DESTRUCTIBLE);
    m.column(11,16,17, _TileType::DESTRUCTIBLE);
    m.column(18,5,6, _TileType::DESTRUCTIBLE);
    m.column(21,5,6, _TileType::DESTRUCTIBLE);
    m.column(8,4,5, _TileType::DESTRUCTIBLE);

    m.row(20,6,7,_TileType::DESTRUCTIBLE);
    m.row(17,6,8,_TileType::DESTRUCTIBLE);
    m.row(1,7,11,_TileType::DESTRUCTIBLE);
    m.row(17,17,20,_TileType::DESTRUCTIBLE);
    m.row(17,12,13,_TileType::DESTRUCTIBLE);
    m.row(20,23,25,_TileType::DESTRUCTIBLE);
    m.row(18,22,24,_TileType::DESTRUCTIBLE);
    m.row(16,23,24,_TileType::DESTRUCTIBLE);
    m.row(14,23,24,_TileType::DESTRUCTIBLE);
    m.row(11,15,16,_TileType::DESTRUCTIBLE);
    m.row(9,15,16,_TileType::DESTRUCTIBLE);
    m.row(7,15,16,_TileType::DESTRUCTIBLE);
    m.row(4,9,10,_TileType::DESTRUCTIBLE);
    m.row(5,11,12,_TileType::DESTRUCTIBLE);
    m.row(3,19,20,_TileType::DESTRUCTIBLE);
    m.row(20,14,15,_TileType::DESTRUCTIBLE);
    m.row(20,19,20,_TileType::DESTRUCTIBLE);

    m.placeTile(1,7,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(4,18,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(7,15,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(13,7,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(17,5,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(10,6,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(12,15,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(18,15,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(21,13,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(21,11,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(21,9,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(7,3,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(24,1,_Tile(_TileType::DESTRUCTIBLE));
    m.placeTile(2,4,_Tile(_TileType::DESTRUCTIBLE));


    m.column(3,1,3,_TileType::SOLID);
    m.column(4,4,17,_TileType::SOLID);
    m.column(7,4,14,_TileType::SOLID);
    m.column(18,7,14,_TileType::SOLID);
    m.column(22,7,17,_TileType::SOLID);
    m.column(20,4,7,_TileType::SOLID);
    m.column(10,7,13,_TileType::SOLID);
    m.column(11,11,12,_TileType::SOLID);
    m.column(23,1,4,_TileType::SOLID);
    m.column(2,10,11,_TileType::SOLID);
    m.column(6,12,13,_TileType::SOLID);


    m.row(18,5,21,_TileType::SOLID);
    m.row(3,8,18,_TileType::SOLID);
    m.row(15,8,11,_TileType::SOLID);
    m.row(15,13,17,_TileType::SOLID);
    m.row(6,11,17,_TileType::SOLID);
    m.row(2,13,14,_TileType::SOLID);    
    m.row(4,11,12,_TileType::SOLID);
    m.row(16,12,13,_TileType::SOLID);
    m.row(20,12,13,_TileType::SOLID);
    m.row(20,21,22,_TileType::SOLID);
    m.row(17,23,24,_TileType::SOLID);
    m.row(15,23,24,_TileType::SOLID);
    m.row(13,23,24,_TileType::SOLID);
    m.row(12,15,16,_TileType::SOLID);
    m.row(10,15,16,_TileType::SOLID);
    m.row(8,15,16,_TileType::SOLID);

    m.placeTile(6,1,_Tile(_TileType::SOLID));
    m.placeTile(6,1,_Tile(_TileType::SOLID));
    m.placeTile(12,11,_Tile(_TileType::SOLID));
    m.placeTile(19,19,_Tile(_TileType::SOLID));
    m.placeTile(24,4,_Tile(_TileType::SOLID));
    m.placeTile(20,16,_Tile(_TileType::SOLID));
    m.placeTile(21,14,_Tile(_TileType::SOLID));
    m.placeTile(21,12,_Tile(_TileType::SOLID));    
    m.placeTile(21,10,_Tile(_TileType::SOLID));

    m.column(21,7,8,_TileType::SOLID);   

    m.spawnPatroller(13,19,13,19,4,19,10);
    m.spawnPatroller(9,13,9,13,9,7,10);
    m.spawnChaser(6,14,9);

    m.borderWalls();

    return m;

}

Map MapBuilder::gpu(){
    Map m;
    
    m.fill(7,11,10,12,_TileType::DESTRUCTIBLE);
    m.fill(1,1,3,2,_TileType::DESTRUCTIBLE);
    m.fill(7,11,10,12,_TileType::DESTRUCTIBLE);
    m.fill(24,1,25,4,_TileType::DESTRUCTIBLE);
    m.fill(5,14,6,20,_TileType::DESTRUCTIBLE);
    m.fill(3,12,4,14,_TileType::DESTRUCTIBLE);
    m.fill(13,16,14,19,_TileType::DESTRUCTIBLE);
    m.fill(12,1,13,3,_TileType::DESTRUCTIBLE);
    m.fill(20,8,22,10,_TileType::DESTRUCTIBLE);
    m.fill(23,14,24,15,_TileType::DESTRUCTIBLE);
    m.fill(7,5,8,6,_TileType::DESTRUCTIBLE);
    m.fill(1,7,2,8,_TileType::DESTRUCTIBLE);
    m.fill(2,9,3,10,_TileType::DESTRUCTIBLE);
    m.fill(5,8,6,9,_TileType::DESTRUCTIBLE);
    m.fill(19,17,20,20,_TileType::DESTRUCTIBLE);
    m.fill(17,15,18,17,_TileType::DESTRUCTIBLE);
    m.fill(7,19,8,20,_TileType::DESTRUCTIBLE);

    m.column(25,7,20,_TileType::DESTRUCTIBLE);
    m.column(1,14,20,_TileType::DESTRUCTIBLE);
    m.column(4,2,4,_TileType::DESTRUCTIBLE);
    m.column(20,4,7,_TileType::DESTRUCTIBLE);
    m.column(14,8,12,_TileType::DESTRUCTIBLE);
    m.column(10,13,14,_TileType::DESTRUCTIBLE);
    m.column(7,17,18,_TileType::DESTRUCTIBLE);
    m.column(6,6,7,_TileType::DESTRUCTIBLE);
    m.column(16,4,5,_TileType::DESTRUCTIBLE);
    m.column(22,5,7,_TileType::DESTRUCTIBLE);
    m.column(21,15,16,_TileType::DESTRUCTIBLE);
    m.column(22,19,20,_TileType::DESTRUCTIBLE);

    m.row(16,2,4,_TileType::DESTRUCTIBLE);
    m.row(18,2,4,_TileType::DESTRUCTIBLE);
    m.row(20,2,4,_TileType::DESTRUCTIBLE);
    m.row(1,7,8,_TileType::DESTRUCTIBLE);
    m.row(2,8,9,_TileType::DESTRUCTIBLE);
    m.row(3,9,10,_TileType::DESTRUCTIBLE);
    m.row(4,10,11,_TileType::DESTRUCTIBLE);
    m.row(6,11,13,_TileType::DESTRUCTIBLE);
    m.row(8,9,10,_TileType::DESTRUCTIBLE);
    m.row(10,9,10,_TileType::DESTRUCTIBLE);
    m.row(13,8,9,_TileType::DESTRUCTIBLE);
    m.row(16,10,12,_TileType::DESTRUCTIBLE);
    m.row(3,1,2,_TileType::DESTRUCTIBLE);
    m.row(8,16,17,_TileType::DESTRUCTIBLE);
    m.row(9,17,18,_TileType::DESTRUCTIBLE);
    m.row(11,17,18,_TileType::DESTRUCTIBLE);
    m.row(12,16,17,_TileType::DESTRUCTIBLE);
    m.row(12,12,13,_TileType::DESTRUCTIBLE);
    m.row(11,21,22,_TileType::DESTRUCTIBLE);
    m.row(1,19,23,_TileType::DESTRUCTIBLE);
    m.row(20,23,24,_TileType::DESTRUCTIBLE);

    m.placeTile(2,4,_Tile(_TileType::DESTRUCTIBLE));    
    m.placeTile(22,12,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(22,13,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(21,20,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(18,19,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(16,16,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(12,17, _Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(9,20,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(2,14,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(4,11,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(12,11,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(16,13,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(18,10,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(16,7,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(15,5,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(19,7,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(18,14,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(20,14,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(21,6,_Tile(_TileType::DESTRUCTIBLE)); 
    m.placeTile(23,2,_Tile(_TileType::DESTRUCTIBLE)); 


    m.fill(23,16,24,19,_TileType::SOLID);
    m.fill(23,11,24,13,_TileType::SOLID);
    m.fill(23,5,24,8,_TileType::SOLID);
    m.fill(16,2,21,3,_TileType::SOLID);
    m.fill(1,11,2,13,_TileType::SOLID);
    m.fill(7,7,8,8,_TileType::SOLID);
    m.fill(9,6,10,7,_TileType::SOLID);
    m.fill(3,3,4,4,_TileType::SOLID);
    m.fill(19,15,20,16,_TileType::SOLID);
    m.fill(10,1,11,2,_TileType::SOLID);

    m.row(15,2,4,_TileType::SOLID);
    m.row(17,2,4,_TileType::SOLID);
    m.row(19,2,4,_TileType::SOLID);
    m.row(15,12,14,_TileType::SOLID);
    m.row(14,8,9,_TileType::SOLID);
    m.row(12,20,21,_TileType::SOLID);
    m.row(7,11,15,_TileType::SOLID);
    m.row(13,11,15,_TileType::SOLID);
    m.row(18,11,12,_TileType::SOLID);
    m.row(17,10,11,_TileType::SOLID);
    m.row(18,17,18,_TileType::SOLID);

    m.column(1,4,6,_TileType::SOLID);
    m.column(1,9,10,_TileType::SOLID);
    m.column(11,9,12,_TileType::SOLID);
    m.column(15,8,12,_TileType::SOLID);
    m.column(6,9,11,_TileType::SOLID);
    m.column(19,4,6,_TileType::SOLID);
    m.column(17,4,5,_TileType::SOLID);
    m.column(14,2,3,_TileType::SOLID);
    m.column(4,1,2,_TileType::SOLID);
    m.column(25,6,6,_TileType::SOLID);
    m.column(23,3,4,_TileType::SOLID);
    m.column(21,18,19,_TileType::SOLID);
    m.column(8,17,18,_TileType::SOLID);

    m.placeTile(3,8,_Tile(_TileType::SOLID)); 
    m.placeTile(3,11,_Tile(_TileType::SOLID)); 
    m.placeTile(4,5,_Tile(_TileType::SOLID)); 
    m.placeTile(7,9,_Tile(_TileType::SOLID)); 
    m.placeTile(9,1,_Tile(_TileType::SOLID)); 
    m.placeTile(11,3,_Tile(_TileType::SOLID)); 
    m.placeTile(21,5,_Tile(_TileType::SOLID)); 
    m.placeTile(21,7,_Tile(_TileType::SOLID)); 
    m.placeTile(9,19,_Tile(_TileType::SOLID));     
    m.placeTile(17,10,_Tile(_TileType::SOLID));     
    m.placeTile(20,11,_Tile(_TileType::SOLID)); 
    m.placeTile(12,19,_Tile(_TileType::SOLID)); 
    m.placeTile(19,14,_Tile(_TileType::SOLID)); 

    m.setSpawn(12,9);

    m.placeTile(12,10,_Tile(_TileType::GATE_PREV)); 
    m.placeTile(24,10,_Tile(_TileType::GATE_NEXT)); 

    m.spawnChaser(16,9,8);
    m.spawnChaser(7,3,8);

    m.borderWalls();

    return m;
}
