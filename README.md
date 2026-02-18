# BOMBERGIRL

**Bombergirl** is a terminal-based game inspired by classic Bomberman,  
developed as a university project using **C++** and **ncurses**.

## Features

- Multiple levels with forward and backward navigation
- Enemies with different behaviors (walker, patroller, chaser)
- Bombs with blast propagation and destructible walls
- Power-ups 
- Score system and persistent leaderboard
- Name entry system inspired by classic arcade games
- Bonus mode with external level loading from file
- Debug mode with additional testing commands
- Ncurses-based rendering with colors and resize handling

## Controls

### Menu
- **UP / DOWN** : navigate menu  
- **ENTER** : select  
- **Q** : quit  
- **C** : credits  
- Secret input `"BOMBERGIRL"` : bonus mode 

### In Game
- **Arrow keys** : move Bombergirl  
- **SPACE** : place bomb  
- **Q** : quit to menu  

### Leaderboard
- **UP / DOWN / LEFT / RIGHT** : select number or change page  
- **ENTER / Q** : go back  

## Build and Requirements

### Requirements
- a C++17 compiler 
- make
- ncurses development library

### Linux (Debian-based)
```bash
sudo apt-get install build-essential libncurses-dev
```

### macOS (homebrew)
```bash
install: brew install ncurses
```

### Compile:

type this command from the project folder:
  `make`

for build debug (enables DEBUG_MODE):
  `make debug`

### Run:

type this command from the project folder:
  `./bombergirl`

to run debug:
  `./bombergirl_debug`

## DEBUG MODE

When compiled with DEBUG_MODE enabled, additional debug 
commands are available:

- K : **kill** all enemies in current level
- D : **destroy** all destructible walls
- I : toggle **immortality**
- U : enables **ultra-speed** (no movement cooldown)
- L : **lose** a life immediately

Debug mode also increases starting lives (99)
and world time (9999) to simplify testing.

## BONUS MODE

Bonus mode can be activated from the menu by typing the secret keyword:
`BOMBERGIRL`

In bonus mode, a single level is loaded from an external file using a
safe CSV-based format. This allows testing custom levels without modifying
the main game maps.

### Bonus file format:

- one tile per line
- separator is ;
- comments start with #
- empty or invalid lines are ignored

each line has exactly 8 fields:
  `type;x;y;sx;sy;ex;ey;speed`

`type` is an integer that matches `_TileType`

`x`,`y` are the tile coordinates in the map grid

`sx`,`sy`,`ex`,`ey`,`speed` are extra parameters used by enemies
(for tiles that do not need them, write 0)

`sx`,`sy`: enemy start position (every enemy has a starting position)

`ex`,`ey`: patroller end position

`speed`: enemy movement cooldown

### TILE TYPE LEGEND (`_TileType`)
these are the numeric values used in the file:

| Value | Name         |
|-------|--------------|
| 0     | `EMPTY `       |
| 1     | `SOLID `       |
| 2     | `DESTRUCTIBLE` |
| 3     | `SPAWN `       |
| 4     | `WALKER `      |
| 5     | `PATROLLER`    |
| 6     | `CHASER `      |
| 7     | `GATE_NEXT`    |
| 8     | `GATE_PREV`    |


### the bonus file is rejected if:
- it does not contain exactly one SPAWN
- it does not contain at least one gate (GATE_NEXT or GATE_PREV)
- a gate is placed on the border (borders are reserved for SOLID walls)
- spawn is placed on the border
- coordinates appear twice in the file
- coordinates are out of bounds
- _TileType value is invalid

also, the parser forces a `SOLID` border around the map to prevent 
out-of-bounds exceptions

## NOTE ON TEMPLATES (list.hpp)

The list container is a template class.

According to the C++ compilation model, template implementations must be
visible at compile time, therefore the entire implementation is provided
in the header file (.hpp).

This is intentional and required for correct template instantiation.

## IMPLEMENTATION NOTES

- Game logic is kept separate from rendering and input handling
- Levels keep their internal state when revisited, except for active bombs
- Victory depends on completing all levels, in any order

## DEMO VIDEO

https://drive.google.com/file/d/192L8F6iuIS1mhh1FUeksYgXYBaS8WMW7/view

## AUTHORS

Developed as a university project  
University of Bologna

Group members:
| First Name | Last Name         | ID         |
|------------|-------------------|------------|
| Martina Lisa Saffo| Ramponi | 0001220008|
| Martina | Nazzareni | 0001223089 |

## CONTACTS
martinalisasaffo.ramponi@studio.unibo.it

martina.nazzareni@studio.unibo.it

## LICENSE

This project is licensed under the MIT License.

 See the LICENSE file for details.
