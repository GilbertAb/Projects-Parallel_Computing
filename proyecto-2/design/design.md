Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

The program is made so that the class Island is in control of the functionality, it recieves the input from main and makes the output, gets the information needed to work with and creates the forests requested with the days said in the input and the simulation of passing time, for this its dependant on the class Forest to work with each one of the requested forests applying the rules to update the cells with the passing days. The handling of files and input is not shown on the pseudocode because is meant to solve the problem of simulating the forest according to its rules.

Pseudocode:
void Island::create_forest(char** file_forest, string map_name, int64_t days, int64_t rows, int64_t columns) {
	size_t index := 0;
    Forest* forest := new Forest(rows, columns, map_name);
    for (size_t row := 0; row < rows; ++row) {
      for (size_t col := 0; col < columns; ++col) {
        forest->set_cell(row, col, file_forest[row][col]);
      }
    }
    this->forest.push_back(forest);
    this->days.push_back(days);
  }
}

void Island::simulate_days() {
  for (size_t index := 0; index < forest.size(); ++index) {
    size_t output_at_day := 0;
    if (days[index] < 0) {
      days[index] := -days[index];
      output_at_day := days[index];
    }
    for (size_t day := 0; day < days[index]; ++day) {
      forest[index]->end_day();
      if (day+1 >= output_at_day) {
      fstream << forest[index]->to_string();
      }
    }
  }
}

Forest::Forest(size_t rows, size_t columns, string map_name) {
  map = NULL;
  this->map_name := map_name;
  init_forest(rows, columns);
  this->rows := rows;
  this->columns := columns;
}

Forest::~Forest() {
  if(map != NULL) {
    for (size_t index := 0; index < rows; ++index) {
          delete[] map[index];
        }
    }
  delete[] map;
}

void Forest::init_forest(size_t rows, size_t columns) {
  if (!map) {
    map := new char*[rows];
    for (size_t index := 0; index < rows; ++index) {
      map[index] := new char[columns];
    }
  }
}

void Forest::update_cell(size_t row, size_t column, char** next_day) {
  char tree_count := 0, lake_count := 0, meadow_count := 0;
  check_neighbors(tree_count, lake_count, meadow_count, row, column);
  switch (map[row][column]) {
    case 'a':
      if (lake_count >=4)
        next_day[row][column] := 'l';
      else if (tree_count > 4)
        next_day[row][column] := '-';
      else
        next_day[row][column] := 'a';
      break;
    
    case 'l':
      if (lake_count < 3)
        next_day[row][column] := '-';
      else
        next_day[row][column] := 'l';
      break;
    
    case '-':
      if (tree_count >= 3)
        next_day[row][column] := 'a';
      else
        next_day[row][column] := '-';
      break;
  }
}

bool Forest::in_bounds(size_t row, size_t column, size_t index, char* row_dis, char* col_dis) {
  return row + row_dis[index] < rows && column + col_dis[index] < columns;
}

void Forest::check_neighbors(char& tree_count, char& lake_count, char& meadow_count, size_t row, size_t column) {
  char row_dis[8] := {-1, -1, 0, 1, 1, 1, 0, -1};
  char col_dis[8] := {0, 1, 1, 1, 0, -1, -1, -1};
  for (size_t index := 0; index < 8; ++index) {
    if (in_bounds(row, column, index, row_dis, col_dis)) {
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
  char** next_day := new char*[rows];
  for (size_t index := 0; index < rows; ++index) {
    next_day[index] := new char[columns];
  }

  for(size_t row := 0; row < rows; ++row) {
    for (size_t col := 0; col < columns; ++col) {
      update_cell(row, col, next_day);
    }
  }

  for (size_t index := 0; index < rows; ++index) {
    delete[] map[index];
  }
  delete[] map;
  map := next_day;
}

void Forest::set_cell(size_t row, size_t col, char data) {
  map[row][col] := data;
}

string Forest::to_string() {
  string forest_str;
  for(size_t row := 0; row < rows; ++row) {
    for (size_t col := 0; col < columns; ++col) {
      forest_str += map[row][col];
    }
    forest_str += '\n';
  }
  return forest_str;
}

string Forest::get_map_name() {
  return this->map_name;
}
