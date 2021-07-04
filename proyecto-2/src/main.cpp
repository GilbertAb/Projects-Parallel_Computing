// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "Island.hpp"


int main(int argc, char* argv[]) {
  (void)argc;
  Island island;
  island.get_job(argv[1]);
  island.simulate_days(island.create_output_directory(argv[1]));
}
