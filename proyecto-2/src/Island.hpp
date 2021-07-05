// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef ISLAND_H
#define ISLAND_H

#include <fstream>
#include <string>
#include <vector>
#include "Forest.hpp"

class Island {
 private:
  std::vector<Forest*> forest;
  std::vector<int64_t> days;

 public:
  Island();
  ~Island();
  void get_job(const char* filename);
  void create_forest(std::string map_path, std::string map_name, int64_t days);
  void simulate_days(std::string output_path);
  std::string create_output_directory();
  bool is_open(std::fstream& fstream, std::string file);
  
};

#endif // ISLAND_H