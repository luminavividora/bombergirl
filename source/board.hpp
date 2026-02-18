/**
 * @file board.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief defines the Board class used 
 * to represent and manage the game grid
 * @version 1.0
 * @date 2026-01-28
 * 
 * @copyright Copyright (c) 2026
 * 
 * the board stores the current state of each cell 
 * (walls, empty spaces, bombs,explosions and gates)
 * 
 * 
 */

#ifndef BOARD_HPP
#define BOARD_HPP

#include <exception>
/**
 * @class Board
 * @brief manages the level grid and its cell state
 *
 * the Board is the authoritative representation of the 
 * level map during gameplay
 * 
 * it stores a 2D grid of CellType values and offers methods to:
 * 
 * - read/write cells with bounds checking 
 * 
 * - test movement validity (walkability)
 * 
 * - manage explosions with a time to live mechanism
 *
 * many gameplay systems (player movement, bombs, 
 * enemies, rendering) rely on the
 * Board as the shared source of truth
 * 
 * @note it does not contains enemy, powerup and player data
 *
 * @warning accessing an uninitialized board or 
 * using invalid coordinates may throw
 * exceptions defined in this class
 */
class Board{
public:
    /**
     * @enum CellType
     * @brief possible types of cells in the game grid
     * 
     */
    enum CellType {
        EMPTY, // air
        WALL_SOLID, // indestructible wall
        WALL_DESTRUCTIBLE, // destructible wall
        EXPLOSION, // explosion effect
        GATE_NEXT, // door for next level
        GATE_PREV, // door for prev level
        BOMB // bomb solid hitbox
    };
    /**
     * @class BoardOutOfBoundsException
     * @brief thrown when accessing a cell 
     * outside the board bounds
     */
    class BoardOutOfBoundsException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "board access out of bounds";
            }
    };

    /**
     * @class BoardNotInitializedException
     * @brief thrown when accessing a board that
     *  has not been initialized
     */
    class BoardNotInitializedException : public std::exception {
        public:
            const char* what() const noexcept override {
                return "access to a not initialized board";
            }           
    };
private:
    unsigned short width;
    unsigned short height;
    Board::CellType** grid; // board matrix

    // explosion effects time to live (ticks)
    unsigned short** explosionTtl;

public:
    /**
     * @brief default constructor, creates
     * and uninitialized board
     *
     */
    Board();
    /**
     * @brief constructs a board with given dimensions.
     * @param w width
     * @param h height
     */
    Board(unsigned short w, unsigned short h);
    /**
     * @brief destructor
     *
     * frees all allocated memory
     */
    ~Board();

    /**
     * @brief resets all cells to EMPTY state
     *
     * 
     */
    void clear();

    /**
     * @brief sets the type of a cell
     * @param x X coordinate
     * @param y Y coordinate
     * @param type new cell type
     *
     * @throws BoardOutOfBoundsException 
     * if coordinates are invalid
     * @throws BoardNotInitializedException 
     * when not initialized
     */
    void setCell(unsigned short x, unsigned short y,
    Board::CellType type);

    /**
     * @brief returns the type of a cell.
     * @param x X coordinate
     * @param y Y coordinate
     * @return CellType
     *
     * @throws BoardOutOfBoundsException 
     * if coordinates are invalid
     * @throws BoardNotInitializedException 
     * when not initialized
     */
    Board::CellType getCell(unsigned short x,
    unsigned short y) const;
    /**
     * @brief returns the board width
     * @return width in cells
     */
    unsigned short getWidth() const;

    /**
     * @brief returns the board height
     * @return height in cells
     */
    unsigned short getHeight() const;

    /**
     * @brief checks whether a cell is walkable
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if the cell can be walked on
     * 
     * @throws BoardOutOfBoundsException 
     * if coordinates are invalid
     * @throws BoardNotInitializedException 
     * when not initialized
     */
    bool isWalkable(unsigned short x,
    unsigned short y) const;  

    /**
     * @brief checks whether a cell contains
     * a damaging explosion
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if damage should be applied
     * 
     * @throws BoardOutOfBoundsException 
     * if coordinates are invalid
     * @throws BoardNotInitializedException 
     * when not initialized
     */ 
    bool damageTaken(unsigned short x, 
    unsigned short y) const;

    /**
     * @brief removes all explosions from the board
     * 
     * @throws BoardNotInitializedException 
     * when not initialized
     */
    void clearExplosions(); 

    /**
     * @brief updates active explosions
     *
     * decreases explosion time to live
     * 
     * @throws BoardNotInitializedException 
     * when not initialized
     */
    void updateExplosions();
};

#endif
