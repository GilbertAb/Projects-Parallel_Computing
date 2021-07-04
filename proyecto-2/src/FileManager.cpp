// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>
#include "FileManager.hpp"
#include "Forest.hpp"
using namespace std;


FileManager::FileManager() {
  
}
  /// Destructor
FileManager::~FileManager() {

}
/// Start the simulation
std::vector<FileManager::job_order_t> FileManager::readJobs(std::string jobFileName) {
  int error = 0;
  std::vector<FileManager::job_order_t> jobs;

  ifstream file(jobFileName.c_str());
  std::string line;
  while(getline(file, line)) {
    std::array<std::string, 2> args = separateStringLine(line);
    
    //TODO: Analize arguments
  
    FileManager::job_order_t job_order;
    job_order.mapFileName = args[0];
    job_order.numMidnights = args[1];
    jobs.push_back(job_order);
  }
  return jobs;
}

//Crea el mapa
Forest* FileManager::readMap(std::string mapFileName) {
  int error = 0;
  std::string route = "test_set_1/input/"+ mapFileName;
  
  ifstream file(route.c_str());
  std::string mapLine;
  
  std::string rowsAndColumns;
  getline(file, rowsAndColumns);
  
  std::array<std::string, 2> args = separateStringLine(rowsAndColumns);

  int rows = stoi(args[0]);
  int columns = stoi(args[1]);
  Forest* forest = new Forest(rows,columns);
  
  // reads map content and copies into forest's map
  int row = 0;
  while(getline(file, mapLine)) {
    std::string::iterator itBegin = mapLine.begin();
    std::string::iterator itEnd = mapLine.end();
    std::string::iterator iter = itBegin;
    int column = 0;
    while(iter != itEnd){
      if(*iter != ' ') {
        forest->setCell(row, column, *iter);
      }
      iter++;
      column++;
    }
    row++;
  }
  return forest;
}

std::array<std::string,2> FileManager::separateStringLine(std::string stringLine) {
  std::array<std::string,2> arguments;
  std::string::iterator itBegin = stringLine.begin();
  std::string::iterator itEnd = stringLine.end();
  std::string::iterator iter = itBegin;

  while (*iter != ' ') {
    arguments[0].push_back(*iter);
    iter++;
  }
  while (iter != itEnd) {
    arguments[1].push_back(*iter);
    iter++;
  }
  return arguments;
}
