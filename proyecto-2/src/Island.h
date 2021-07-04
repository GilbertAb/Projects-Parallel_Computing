#ifndef ISLAND_H
#define ISLAND_H

#include <string>
#include <vector>
#include "Forest.h"

class Island {
 private:
  std::vector<Forest*> forest;
  std::vector<int64_t> days;

 public:
  Island();
  ~Island();
  // TODO(any) implement analyze_arguments. Verify only 1 argument is entered and print correct usage.
  // bool analyze_arguments(int argc, char* argv[]);
  void get_job(const char* filename);
  void create_forest(const char* map_name, int64_t days);
  void simulate_days(std::string output_path);
  std::string create_output_directory(std::string job_path);
  
};

#endif // ISLAND_H