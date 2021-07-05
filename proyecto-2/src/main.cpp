// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

// TODO(KEVIN) add documentation
// TODO(KEVIN) fix linter warnings

#include <unistd.h>
#include "Island.hpp"
#include <exception>
#include <iostream>
#include "Island.hpp"

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (argc == 2 || argc == 3) {
    try {
    int64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 3) {
      if (sscanf(argv[2], "%zu", &thread_count) != 1) {
        std::cerr << "error: invalid thread count\n";
        return error;
      }
    }
    
    Island island;
    island.get_job(argv[1]);
    island.simulate_days(island.create_output_directory(argv[1]),
      thread_count);
    } catch (const std::runtime_error& e) {
      std::cerr << "Error: " << e.what()<<'\n';
      error = EXIT_FAILURE; 
    }
  } else {
    std::cerr << "Invalid number of arguments\nUsage: executable [path to job]\n";
    error = EXIT_FAILURE;
  }
  return error;
}
