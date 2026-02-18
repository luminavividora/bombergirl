/**
 * @file parser.hpp
 * @author Martina Lisa Saffo Ramponi (0001220008)
 * @brief bonus level file parser
 * @version 1.0
 * @date 2026-02-02
 * 
 * @copyright Copyright (c) 2026
 * 
 * this file defines the Parser class used by bonus mode
 * 
 * it loads a text file and fills a 
 * Map instance with the tiles described inside
 * 
 * the parser is strict about bounds and 
 * basic validity to avoid unsafe maps
 * 
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include "maps.hpp"

/**
 * @brief parses a bonus level file and fills a Map
 *
 * the input format is a csv-like text file with fields separated by ';'
 * each valid row describes one tile to place in the map grid
 *
 * the function writes a short error message in err when it fails
 */
class Parser {
public:
    /**
     * @brief load a bonus file and populate outMap
     *
     * this function clears outMap first, 
     * then applies all tiles found in the file
     * 
     * it also performs basic validation 
     * (bounds, duplicates, required spawn)
     *
     * @param path file path to read
     * @param outMap destination map that will be filled
     * @param err output buffer for an error message (can be null)
     * @param errCap capacity of err (ignored if err is null)
     * @return true on success, false on error
     */
    static bool loadBonusFile(const char* path, Map& outMap,
    char* err, int errCap);

private:
    /**
     * @brief copy an error message into the user buffer
     *
     * this always writes a null terminator if the buffer is valid
     *
     * @param err output buffer (can be null)
     * @param cap buffer capacity
     * @param msg message to copy
     */
    static void setErr(char* err, int cap, const char* msg);
    
    /**
     * @brief parse a non-negative integer from a c string
     *
     * parsing is strict: it ignores leading spaces, 
     * reads digits, then expects end of string
     *
     * @param s input string
     * @param ok set to true if parsing 
     * succeeds, false otherwise
     * @return parsed value as int, or 0 if parsing fails
     */
    static int parseInt(const char* s, bool& ok);
};


#endif