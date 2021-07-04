// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>

#include "MapGenerator.hpp"
using namespace std;

MapGenerator::MapGenerator() {
  
}
  /// Destructor
MapGenerator::~MapGenerator() {

}
/// Start the simulation
int MapGenerator::run(std::string jobFileName) {
  int error = 0;
  FileManager fileManager = FileManager();
  std::vector<FileManager::job_order_t> jobs = fileManager.readJobs(jobFileName);
  
  for(int index = 0; index < jobs.size(); index++) {
    Forest* forest = fileManager.readMap(jobs[index].mapFileName);
    std::cout<< "map: " << index << endl;
    forest->printForest();
    
  }

  return error;
}
