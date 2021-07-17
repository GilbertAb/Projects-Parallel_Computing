// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include <iostream>
#include "Map.hpp"

const char row_dis[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const char col_dis[8] = {0, 1, 1, 1, 0, -1, -1, -1};

Map::Map(size_t rows, size_t columns, std::string map_name) {
  current_day_map = NULL;
  next_day_map = NULL;
  this->map_name = map_name;
  init_map(rows, columns);
  this->rows = rows;
  this->columns = columns;
}

Map::~Map() {
  if (current_day_map != NULL) {
    for (size_t index = 0; index < rows; ++index) {
          delete[] current_day_map[index];
          delete[] next_day_map[index];
        }
    }
  delete[] current_day_map;
  delete[] next_day_map;
}

void Map::init_map(size_t rows, size_t columns) {
  if (!current_day_map) {
    current_day_map = new char*[rows];
    next_day_map = new char*[rows];
    for (size_t index = 0; index < rows; ++index) {
      current_day_map[index] = new char[columns];
      next_day_map[index] = new char[columns];
    }
  }
}

void Map::update_cell(size_t row, size_t column, char** next_day) {
  char tree_count = 0, lake_count = 0, meadow_count = 0;
  check_neighbors(tree_count, lake_count, meadow_count, row, column);
  switch (current_day_map[row][column]) {
    case 'a':
      if (lake_count >=4)
        next_day[row][column] = 'l';
      else if (tree_count > 4)
        next_day[row][column] = '-';
      else
        next_day[row][column] = 'a';
      break;

    case 'l':
      if (lake_count < 3)
        next_day[row][column] = '-';
      else
        next_day[row][column] = 'l';
      break;

    case '-':
      if (tree_count >= 3)
        next_day[row][column] = 'a';
      else
        next_day[row][column] = '-';
      break;
  }
}

bool Map::in_bounds(size_t row, size_t column, size_t index) {
  return row + row_dis[index] < rows && column + col_dis[index] < columns;
}

void Map::check_neighbors(char& tree_count, char& lake_count,
  char& meadow_count, size_t row, size_t column) {
  for (size_t index = 0; index < 8; ++index) {
    if (in_bounds(row, column, index)) {
      switch (current_day_map[row+row_dis[index]][column+col_dis[index]]) {
        case 'a':
          ++tree_count;
          break;

        case '-':
          ++meadow_count;
          break;

        case 'l':
          ++lake_count;
          break;
      }
    }
  }
}

void Map::end_day() {
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < columns; ++col) {
      update_cell(row, col, next_day_map);
    }
  }
  for (size_t row = 0; row < rows; ++row)
    std::copy(&next_day_map[row][0], &next_day_map[row][0]+columns, &current_day_map[row][0]);
}

void Map::set_cell(size_t row, size_t col, char data) {
  current_day_map[row][col] = data;
  next_day_map[row][col] = data;
}

std::string Map::to_string() {
  std::string map_str;
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < columns; ++col) {
      map_str += current_day_map[row][col];
    }
    map_str += '\n';
  }
  return map_str;
}

std::string Map::get_map_name() {
  return this->map_name;
}
