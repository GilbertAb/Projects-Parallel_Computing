// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include <iostream>
#include "Forest.hpp"

const char row_dis[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const char col_dis[8] = {0, 1, 1, 1, 0, -1, -1, -1};

Forest::Forest(size_t rows, size_t columns, std::string map_name) {
  map = NULL;
  this->map_name = map_name;
  init_forest(rows, columns);
  this->rows = rows;
  this->columns = columns;
}

Forest::~Forest() {
  if (map != NULL) {
    for (size_t index = 0; index < rows; ++index) {
          delete[] map[index];
        }
    }
  delete[] map;
}

void Forest::init_forest(size_t rows, size_t columns) {
  if (!map) {
    map = new char*[rows];
    for (size_t index = 0; index < rows; ++index) {
      map[index] = new char[columns];
    }
  }
}

void Forest::update_cell(size_t row, size_t column, char** next_day) {
  char tree_count = 0, lake_count = 0, meadow_count = 0;
  check_neighbors(tree_count, lake_count, meadow_count, row, column);
  switch (map[row][column]) {
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

bool Forest::in_bounds(size_t row, size_t column, size_t index) {
  return row + row_dis[index] < rows && column + col_dis[index] < columns;
}

void Forest::check_neighbors(char& tree_count, char& lake_count,
  char& meadow_count, size_t row, size_t column) {
  for (size_t index = 0; index < 8; ++index) {
    if (in_bounds(row, column, index)) {
      switch (map[row+row_dis[index]][column+col_dis[index]]) {
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

void Forest::end_day() {
  char** next_day = new char*[rows];
  for (size_t index = 0; index < rows; ++index) {
    next_day[index] = new char[columns];
  }

  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < columns; ++col) {
      update_cell(row, col, next_day);
    }
  }

  for (size_t index = 0; index < rows; ++index) {
    delete[] map[index];
  }
  delete[] map;
  map = next_day;
}

void Forest::set_cell(size_t row, size_t col, char data) {
  map[row][col] = data;
}

std::string Forest::to_string() {
  std::string forest_str;
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < columns; ++col) {
      forest_str += map[row][col];
    }
    forest_str += '\n';
  }
  return forest_str;
}

std::string Forest::get_map_name() {
  return this->map_name;
}
