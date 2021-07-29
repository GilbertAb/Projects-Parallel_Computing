// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "Job.hpp"
#include <algorithm>
#include <unistd.h>
#include <omp.h>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

const size_t MESSSAGE_LEN = 1;

const char* const usage =
  "Usage: forest [job] [thread_count]\n"
  "\n"
  "  path_of_job    text file with the requested work\n"
  "  thread_count   amount of threads that will be created, default cpu core count\n";


Job::Job() {}

Job::~Job() {}

int Job::run(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (analyze_arguments(argc, argv)) {
      try {
        create_output_directory();
        get_job(argv[1]);
      } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() <<'\n';
        return EXIT_FAILURE;
      }
    }
    MPI_Finalize();
  }
  return EXIT_SUCCESS;
}

void Job::get_job(const char* filename) {
  std::ifstream job(filename);                      // job textfile
  int64_t message = 0;                              // number to send mpi messages
  int rank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (is_open(job, filename)) {
    if(rank == 0) {
      int process_count = 0;
      size_t map_count = 0;
      MPI_Status status;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);
      // adapted from https://stackoverflow.com/a/3072840
      map_count = 1 + std::count(std::istreambuf_iterator<char>(job), std::istreambuf_iterator<char>(), '\n');
      for(int64_t map_index = 0; map_index < map_count; ++map_index) {
        MPI_Recv(&message, MESSSAGE_LEN, MPI_INT64_T, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&map_index, MESSSAGE_LEN, MPI_INT64_T, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
      }
      for (size_t proc_index = 0; proc_index < process_count - 1; ++proc_index) {
        MPI_Recv(&message, MESSSAGE_LEN, MPI_INT64_T, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        message = -1; //stop condition
        MPI_Send(&message, MESSSAGE_LEN, MPI_INT64_T, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
      }
    } else {
      std::string directory, map_name, sfilename = filename;
      int64_t days = 0;
      size_t line_index = 0;
      // Get path/directory
      const size_t last_slash_pos = sfilename.rfind('/');
      if (std::string::npos != last_slash_pos) {
        directory = sfilename.substr(0, last_slash_pos + 1);
      }
      while(true) {
        message = 0; 
        MPI_Send(&message, MESSSAGE_LEN, MPI_INT64_T, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&message, MESSSAGE_LEN, MPI_INT64_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "received message " << message <<'\n';
        if (message == -1) {
          break;
        }
        //skip_lines = message - skip_lines;
        for (line_index; line_index <= message; ++line_index) {
          job >> map_name;
          job >> days;
        }
        std::cout << "Process " << rank << ": " << map_name << '\n';
        Map map;
        create_map(map, directory + map_name, map_name);
        simulate_days(map, days);
      }
    } 
  }
}

void Job::create_map(Map& map, std::string map_path, std::string map_name) {
  // Open map file
  std::ifstream map_file(map_path);
  if (is_open(map_file, map_path)) {
    size_t rows = 0, columns = 0;
    char cell_char;
    std::stringstream buffer;
    buffer << map_file.rdbuf();
    map_file.close();
    // Read and separate first line, cause this is supposed
    // to have the amount of rows and columns
    buffer >> rows;
    buffer >> columns;
    // Create a map
    map = Map(rows, columns, map_name);
    // Fill the map with the info of the file
    for (size_t row = 0; row < rows; ++row) {
      for (size_t col = 0; col < columns; ++col) {
      buffer >> cell_char;
      map.set_cell(row, col, cell_char);
      }
    }
  }
}

void Job::simulate_days(Map& map, int64_t days) {
  std::ofstream output_map;
  // create output file if current day is equal or greater
  size_t output_at_day = 0;

  // If the number of days is negative, then convert to positive and
  // make output_at_day equal to the number of days so all days will
  // have an output file
  if (days < 0) {
    days = -days;
    output_at_day = days;
  }
  // Update map during days
  for (size_t day = 0; day < (size_t)days; ++day) {
    // Start creating output path
    std::string output_path = output_directory + map.get_map_name() + '-';
    // Update map one day
    map.end_day(thread_count);
    // If days input was negative, then all days updates will have an output
    // file, else just the last day will have an output file
    if (day+1 >= output_at_day) {
      output_path += std::to_string(day+1) + ".txt";
      output_map.open(output_path);
      output_map << map.to_string();
      output_map.close();
    }
  }
}

void Job::create_output_directory() {
  output_directory = "output/";
  std::string shell_command = "mkdir -p " + output_directory;
  // create folder executing shell command (not portable)
  system(shell_command.c_str());
}

bool Job::is_open(std::ifstream& ifstream, std::string file) {
  bool opened = ifstream.is_open();
  if (!opened) {
    std::cerr << "Error: could not open " << file <<"\n";
  }
  return opened;
}

bool Job::analyze_arguments(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    std::cout << usage;
    return false;
  }
  if (argc == 3) {
    try {
      thread_count = std::stoll(argv[2]);
    } catch (const std::exception& error) {
      std::cerr << "error: invalid thread count\n";
      return false;
    }
  }
  return true;
}

