/**
 * @file parser.cpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief parser.hpp implementation
 * @version 1.0
 * @date 2026-02-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "parser.hpp"

#include <fstream>

void Parser::setErr(char* err, int cap, const char* msg) {
    // same as strncpy
    if (!err || cap <= 0){
        return;
    }
    int i = 0;
    for (; msg[i] != '\0' && i < cap - 1; i++){
        err[i] = msg[i];
    }
    err[i] = '\0';
}

static const char* skipSpaces(const char* s) {
    if (!s){
        return s;
    }
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'){
        s++; // skipping spaces, tabs, \n and \r (windows carriage return)
    }
    return s;
}

static bool split8(const char* line, char fields[8][32], int& outCount) {
    outCount = 0;
    if (!line){
        return false;
    }

    int fi = 0; //field
    int ci = 0; // char


    for (int i = 0; i < 8; i++){
        fields[i][0] = '\0';
    }

    for (int i = 0; ; i++) {
        char ch = line[i];
        if (ch == ';' || ch == '\0' || ch == '\n' || ch == '\r') {
            if (fi >= 8){
                return false;
            }
            fields[fi][ci] = '\0';

            fi++;
            outCount = fi;

            if (ch == '\0' || ch == '\n' || ch == '\r'){
                break;
            }

            //next field
            if (fi >= 8) {
                return false;
            }

            ci = 0;
            continue;
        }

        if (fi >= 8){
            return false;
        }

        if (ci >= 31){
            return false; 
        }
        fields[fi][ci++] = ch;
    }

    return true;
}

// casting strings into integers
int Parser::parseInt(const char* s, bool& ok) {
    ok = false;
    if (s == NULL){
        return 0;
    }

    s = skipSpaces(s);
    if (*s == '\0'){
        return 0;
    }

    // no negative values (no value can be neg)
    if (*s == '-'){
        return 0;
    }

    unsigned int v = 0;
    int digits = 0;

    while (*s >= '0' && *s <= '9') {
        v = v * 10u + (unsigned int)(*s - '0');
        digits++;
        s++;
    }

    if (digits == 0){
        return 0;
    }

    s = skipSpaces(s);

    if (*s != '\0'){
        return 0;
    }
    ok = true;
    return (int)v;
}

bool Parser::loadBonusFile(const char* path, 
Map& outMap, char* err, int errCap) {
    Parser::setErr(err, errCap, "");

    std::ifstream in(path);
    if (!in.is_open()) {
        setErr(err, errCap, "Cannot open bonus CSV.");
        return false;
    }

    // reset a EMPTY
    for (unsigned short y = 0; y < MAP_HEIGHT; y++) {
        for (unsigned short x = 0; x < MAP_WIDTH; x++) {
            _Tile t(_TileType::EMPTY, 0, 0, 0, 0);
            outMap.set(x, y, t);
        }
    }

    bool used[MAP_HEIGHT][MAP_WIDTH];
    for (int yy = 0; yy < (int)MAP_HEIGHT; yy++) {
        for (int xx = 0; xx < (int)MAP_WIDTH; xx++) {
            used[yy][xx] = false;
        }
    }

    int spawnCount = 0;
    int spawnX = 0;
    int spawnY = 0;

    bool containsGate = false;

    char line[256];
    while (in.getline(line, sizeof(line))) {
        const char* pLine = skipSpaces(line);

        if (!pLine || pLine[0] == '\0'){
            continue; // skipping empty lines
        }

        //that's a comment, skip
        if (pLine[0] == '#'){
            continue;
        }

        // split (using ;)
        char fields[8][32];
        int nfields = 0;
        if (!split8(pLine, fields, nfields)) {
            // invalid format, line ignored
            continue;
        }

        if (nfields != 8) {
            continue;
        }

        // integer parsing 
        // probably bool ok[8] would be better
        bool ok0, ok1, ok2, ok3, ok4, ok5, ok6, ok7;
        int typeI = parseInt(fields[0], ok0);
        int x = parseInt(fields[1], ok1);
        int y = parseInt(fields[2], ok2);
        int sx = parseInt(fields[3], ok3);
        int sy = parseInt(fields[4], ok4);
        int ex = parseInt(fields[5], ok5);
        int ey = parseInt(fields[6], ok6);
        int speed = parseInt(fields[7], ok7);

        //if something is missing ignore
        if (!ok0 || !ok1 || !ok2 || !ok3 || 
        !ok4 || !ok5 || !ok6 || !ok7) {
            continue;
        }

        // validate type
        if (typeI < (int)_TileType::EMPTY || 
            typeI > (int)_TileType::GATE_PREV) {
            setErr(err, errCap, "Invalid _TileType value in CSV.");
            in.close();
            return false;
        }

        // validate coords
        if (x < 0 || y < 0 || 
        x >= (int)MAP_WIDTH || y >= (int)MAP_HEIGHT) {
            setErr(err, errCap, 
            "Tile position out of bounds in CSV.");
            in.close();
            return false;
        }

        // can't use the same coordinates twice
        if (used[y][x]) {
            setErr(err, errCap, 
            "Duplicate (x,y) entry in CSV.");
            in.close();
            return false;
        }

        used[y][x] = true;

        // clamp speed
        if (speed < 0){
            speed = 0;
        }
        if (speed > 65535){
            speed = 65535;
        }

        _Tile t((_TileType)typeI, (unsigned short)sx, 
        (unsigned short)sy, (unsigned short)ex, 
        (unsigned short)ey, speed);



        outMap.set((unsigned short)x, (unsigned short)y, t);

        if (t.getType() == _TileType::SPAWN){
            spawnCount++;
            spawnX = x;
            spawnY = y;
        }

        bool onBorder = (x == 0) || (y == 0) ||
        (x == (int)MAP_WIDTH - 1) || (y == (int)MAP_HEIGHT - 1);

        if(t.getType() == _TileType::GATE_NEXT ||
        t.getType() == _TileType::GATE_PREV){
            if(onBorder == false){
                containsGate = true;
            } else {
                setErr(err, errCap, 
                "Gate tiles must not be placed on the map border.");
                in.close();
                return false;    
            }
        }
    }

    // check for spawn (and block invalid files)
    if (spawnCount != 1) {
        setErr(err, errCap, 
        "CSV must contain exactly one SPAWN tile.");
        in.close();
        return false;
    }

    if (containsGate == false){
        setErr(err, errCap, 
        "CSV must contain at least a gate tile.");
        in.close();
        return false;        
    }

    if (spawnX <= 0 || spawnY <= 0 ||
        spawnX >= (int)MAP_WIDTH - 1 || 
        spawnY >= (int)MAP_HEIGHT - 1) {
        setErr(err, errCap, "SPAWN must not be on the border.");
        in.close();
        return false;
    }

    // solid wall protection (we don't want an OutOfBoundException)
    for (unsigned short x = 0; x < MAP_WIDTH; x++) {
        outMap.set(x, 0, _Tile(_TileType::SOLID));
        outMap.set(x, MAP_HEIGHT - 1, _Tile(_TileType::SOLID));
    } for (unsigned short y = 0; y < MAP_HEIGHT; y++) {
        outMap.set(0, y, _Tile(_TileType::SOLID));
        outMap.set(MAP_WIDTH - 1, y, _Tile(_TileType::SOLID));
    }

    in.close();

    return true;
}
