// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include <omp.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include "Job.hpp"



Job::Job() {}

Job::~Job() {
  for (size_t index = 0; index < map.size(); ++index) {
    delete map[index];
  }
}

int Job::run(int argc, char* argv[]) {
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
      get_job(argv[1]);
      simulate_days(create_output_directory(),
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

void Job::get_job(const char* filename) {
  std::string sfilename(filename);
  std::string directory;

  // Get route/directory
  const size_t last_slash_pos = sfilename.rfind('/');
  if (std::string::npos != last_slash_pos) {
    directory = sfilename.substr(0, last_slash_pos + 1);
  }

  std::string map_name;
  int64_t days = 0;

  // Operates on job file
  std::fstream fstream(filename, std::ios::in);
  if (is_open(fstream, filename)) {
    std::stringstream buffer;
    buffer << fstream.rdbuf();
    fstream.close();
    // Reads every line of job file
    while (buffer.rdbuf()->in_avail()) {
      // Separate line in map_name and days
      buffer >> map_name;
      buffer >> days;
      // Remove ".txt" part of map_name
      map_name = map_name.substr(0, map_name.rfind("."));
      // Create map and store in map vector
      create_map(directory + map_name + ".txt", map_name, days);
    }
  }
}

void Job::create_map(std::string map_path, std::string map_name,
  int64_t days) {
  // Open map file
  std::fstream fstream(map_path, std::ios::in);
  if (is_open(fstream, map_path)) {
    size_t rows = 0, columns = 0;
    char cell_char;
    std::stringstream buffer;
    buffer << fstream.rdbuf();
    fstream.close();
    // Read and separate first line, cause this is supposed
    // to have the amount of rows and columns
    buffer >> rows;
    buffer >> columns;
    // Create a map
    Map* map = new Map(rows, columns, map_name);
    if (map) {
      // Fill the map with the info of the file
      for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < columns; ++col) {
        buffer >> cell_char;
        map->set_cell(row, col, cell_char);
        }
      }
      // Push map to maps vector
      this->map.push_back(map);
      // Push days to days vector
      this->days.push_back(days);
    } else {
      std::cerr << "Could not create map\n";
    }
    fstream.close();
  }
}

void Job::simulate_days(std::string output_directory_path,
  size_t thread_count) {
  #pragma omp parallel for num_threads(thread_count) default(none) \
    shared(std::cout, output_directory_path) schedule(static, 1)
  for (size_t index = 0; index < map.size(); ++index) {
    std::fstream fstream;
    // create output file if current day is equal or greater
    size_t output_at_day = 0;

    // If the number of days is negative, then convert to positive and
    // make output_at_day equal to the number of days so all days will
    // have an output file
    if (days[index] < 0) {
      days[index] = -days[index];
      output_at_day = days[index];
    }
    // Update map during days
    for (size_t day = 0; day < (size_t)days[index]; ++day) {
      // Start creating output path
      std::string output_path = output_directory_path +
        map[index]->get_map_name() + '-';
      // Update map one day
      map[index]->end_day();
      // If days input was negative, then all days updates will have an output
      // file, else just the last day will have an output file
      if (day+1 >= output_at_day) {
        output_path += std::to_string(day+1) + ".txt";
        fstream.open(output_path, std::ios::out);
        fstream << map[index]->to_string();
        fstream.close();
      }
    }
  }
}

std::string Job::create_output_directory() {
  std::string output_directory = "output/";
  std::string shell_command = "mkdir -p " + output_directory;
  // create folder executing shell command (not portable)
  system(shell_command.c_str());
  return output_directory;
}

bool Job::is_open(std::fstream& fstream, std::string file) {
  bool opened = fstream.is_open();
  if (!opened) {
    std::cerr << "Error: could not open " << file <<"\n";
  }
  return opened;
}
