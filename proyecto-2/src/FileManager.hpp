// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "MapGenerator.hpp"
#include "Forest.hpp"
class FileManager {
  
 private:
  

 public:
  struct job_order_t {
    std::string mapFileName;
    std::string numMidnights;
  };
  /// Constructor
  FileManager();
  /// Destructor
  ~FileManager();
  std::vector<FileManager::job_order_t> readJobs(std::string jobFileName);
  Forest* readMap(std::string mapFileName); //reads the file map00x.txt, then creates and returns a map for it
  std::array<std::string,2> separateStringLine(std::string stringLine);
};

#endif  // FILE_MANAGER_H
