// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "Forest.hpp"
#include "FileManager.hpp"
class MapGenerator {

 private:

 public:
  /// Constructor
  MapGenerator();
  /// Destructor
  ~MapGenerator();
  int run(std::string jobFileName);

};

#endif  // MAP_GENERATOR_H
