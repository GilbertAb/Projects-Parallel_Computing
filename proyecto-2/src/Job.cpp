// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include <fstream>
#include <iostream>
#include <omp.h>
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

int run(int argc, char* argv[]){
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
  const size_t last_slash_pos = sfilename.rfind('/');
  if (std::string::npos != last_slash_pos) {
    directory = sfilename.substr(0, last_slash_pos + 1);
  }
  std::string map_name;
  int64_t days = 0;
  std::fstream fstream(filename, std::ios::in);
  if (is_open(fstream, filename)) {
    std::stringstream buffer; 
    buffer << fstream.rdbuf();
    fstream.close();
    while (buffer.rdbuf()->in_avail()) {
      buffer >> map_name;
      buffer >> days;
      map_name = map_name.substr(0, map_name.rfind("."));
      create_map(directory + map_name + ".txt", map_name, days);
    }
  }
}

void Job::create_map(std::string map_path, std::string map_name,
  int64_t days) {
  std::fstream fstream(map_path, std::ios::in);
  if (is_open(fstream, map_path)) {
    size_t rows = 0, columns = 0;
    char cell_char;
    std::stringstream buffer;
    buffer << fstream.rdbuf();
    fstream.close();
    buffer >> rows;
    buffer >> columns;
    Map* map = new Map(rows, columns, map_name);
    if(map) {
      for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < columns; ++col) {
        buffer >> cell_char;
        map->set_cell(row, col, cell_char);
        this->map.push_back(map);
        this->days.push_back(days);
        }
      }
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
    size_t output_at_day = 0;  // create output file if current day is equal or
                               // greater

    if (days[index] < 0) {
      days[index] = -days[index];
      output_at_day = days[index];
    }

    for (size_t day = 0; day < (size_t)days[index]; ++day) {
      std::string output_path = output_directory_path +
        map[index]->get_map_name() + '-';
      map[index]->end_day();
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
  system(shell_command.c_str());  // create folder executing shell command
                                  // (not portable)
  return output_directory;
}

bool Job::is_open(std::fstream& fstream, std::string file) {
  bool opened = fstream.is_open();
  if (!opened) {
    std::cerr << "Error: could not open " << file <<"\n";
  }
  return opened;
}
