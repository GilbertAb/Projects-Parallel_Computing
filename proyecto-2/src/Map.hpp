// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef MAP_H
#define MAP_H

#include <cstdlib>
#include <string>
/**
 * @brief Contains a Map and its related logic.
 * @details The class builds a map based on the
 * parameters received and has methods to advance the map to its next day state.
 * Map cells have one of the following characters:
 *  - 'a' represents a tree cell
 *  - 'l' represents a lake cell
 *  - '-' represents a meadow cell
 */ 
class Map {
 private:
  std::string map_name; /**< Name of the map without .txt extension */
  size_t rows;          /**< Number of rows of the map */
  size_t columns;       /**< Number of columns of the map */
  char** map;           /**< Matrix of chars representing the map */

 public:
  /**
   * Constructor.
   * Initializes class members using given parameters;
   * @param rows Number of rows of the map
   * @param columns Number of columns of the map
   * @param map_name map file name without file extension
   */
  Map(size_t rows, size_t columns, std::string map_name);

  /**
   * Destructor.
   * Frees memory used by map;
   */
  ~Map();

  /**
   * @brief Initializes the cells of the map matrix.
   * @param rows number of rows of the matrix
   * @param columns number of columns of the matrix
   */
  void init_map(size_t rows, size_t columns);

  /**
   * @brief Updates cell to its next day state.
   * @details Neighbors cells are verified and a char is written to the
   * next_day state map cell with the same index. The char written depends
   * on these conditions:
   *  - Flood: if cell is a tree and 4 neighbors are lake then the cell becomes
   *    lake
   *  - Drought: if cell is lake and has less than 3 lake neighbors then cell
   *    becomes meadow
   *  - Remapation: if cell is meadow and has at least 3 tree neighbors then
   *    cell becomes tree
   *  - Overcrowding: if cell is tree and has more than 4 tree neighbors then
   *    cell becomes meadow
   *  - Stability: the cell remains the same if none of the previous conditions
   *    occur.
   * @param row row index of cell
   * @param column column index of cell
   * @param next_day matrix to store the updated cell
   */
  void update_cell(size_t row, size_t column, char** next_day);

  /**
   * @brief Count adjacent neighbors.
   * @details Three counters are updated according to the char contained in the
   * neighbor cells. All 8 adjacent cells are checked and a counter is increased
   *  according to the char stored.
   * @param tree_count Tree counter
   * @param lake_count Lake counter
   * @param meadow_count Meadow counter
   * @param row Row index of cell
   * @param column Column index of cell
   */
  void check_neighbors(char& tree_count, char& lake_count, char& meadow_count,
  size_t row, size_t column);

  /**
   * @brief Verifies if the cell is in a valid position of the map.
   * @param row Row index of cell
   * @param column Column index of cell
   * @param index Index of neighbor being checked (0-7)
   */
  bool in_bounds(size_t row, size_t column, size_t index);

  /**
   * @brief Updates all cells to their next day state.
   * @see update_cell()
   */
  void end_day();

  /**
   * @brief Sets a new character to the cell.
   * @param row Row index of cell
   * @param column Column index of cell
   * @param data Char to be assigned to cell
   */
  void set_cell(size_t row, size_t col, char data);

  /**
   * @brief Getter for map_name attribute.
   * @see map_name
   * @return A string copy of map name
   */
  std::string get_map_name();

  /**
   * @brief Get a string representation of the map.
   * @return A string containing all cell chars representing the map
   */
  std::string to_string();
};

#endif  // MAP_H
