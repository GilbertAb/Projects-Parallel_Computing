// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "Job.hpp"
#include "mpi.h"
int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
  Job job;
  int process_count = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  return job.run(argc, argv, process_count, rank);
  MPI_Finalize();
  }
}
