// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include "Job.hpp"

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

    Job job;
    job.get_job(argv[1]);
    job.simulate_days(job.create_output_directory(),
      thread_count);
    } catch (const std::runtime_error& e) {
      std::cerr << "Error: " << e.what() <<'\n';
      error = EXIT_FAILURE;
    }
  } else {
    std::cerr << "Invalid number of arguments\nUsage:"
      "executable[path to job]\n";
    error = EXIT_FAILURE;
  }
  return error;
}
