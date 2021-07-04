// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0
// TODO(KEVIN) add missing copyrights 
// TODO(KEVIN) add documentation
// TODO(KEVIN) fix linter warnings
  // TODO(DAVID.ATIAS) handle exceptions with try catch
#include "Island.hpp"


int main(int argc, char* argv[]) {
  (void)argc;
  Island island;
  island.get_job(argv[1]);
  island.simulate_days(island.create_output_directory(argv[1]));
}
