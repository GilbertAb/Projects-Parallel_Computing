// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>
//typedef basic_string<char> string;
#include <iostream>
#include <fstream>
#include "MapGenerator.hpp"
using namespace std;


int main(int argc, char* argv[]) {
  int error = 0;
  if (argc == 2) {
    MapGenerator mapGenerator = MapGenerator();
    error = mapGenerator.run(argv[1]);
  } else {
    // ask fileName
  }
  return error;
}
