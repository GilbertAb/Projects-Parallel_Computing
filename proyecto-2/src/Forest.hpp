// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef FOREST_H
#define FOREST_H

#include <cstdlib>
#include <string>

class Forest {
 private:
  // TODO(any) change map_name for map_path wherever needed
  std::string map_name;
  size_t rows;
  size_t columns;
  char** map;

 public:
  Forest(size_t rows, size_t columns, std::string map_name);
  ~Forest();
  void init_forest(size_t rows, size_t columns);
  void update_cell(size_t row, size_t column, char** next_day);
  void check_neighbors(char& tree_count, char& lake_count, char& meadow_count, size_t row, size_t column);
  bool in_bounds(size_t row, size_t column, size_t index);
  void end_day();
  void set_cell(size_t row, size_t col, char data);
  std::string get_map_name();
  std::string to_string();
};

#endif // FOREST_H