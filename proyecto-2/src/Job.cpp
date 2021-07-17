// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include <omp.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "Job.hpp"



Job::Job() {}

Job::~Job() {
  for (size_t index = 0; index < map.size(); ++index) {
    delete map[index];
  }
}

void Job::get_job(const char* filename) {
  std::string sfilename(filename);
  std::string directory;
  const size_t last_slash_pos = sfilename.rfind('/');
  if (std::string::npos != last_slash_pos) {
    directory = sfilename.substr(0, last_slash_pos + 1);
  }
  std::string map_name;
  int64_t days;
  std::fstream fstream(filename, std::ios::in);
  if (is_open(fstream, filename)) {
    std::string job_line;
    char buffer[256];
    while (std::getline(fstream, job_line)) {
      std::sscanf(job_line.c_str(), "%s%ld", buffer, &days);
      map_name.assign(buffer, strlen(buffer));
      map_name = map_name.substr(0, job_line.rfind("."));
      create_map(directory + map_name + ".txt", map_name, days);
    }
    fstream.close();
  }
}

void Job::create_map(std::string map_path, std::string map_name,
  int64_t days) {
  std::fstream fstream(map_path, std::ios::in);
  if (is_open(fstream, map_path)) {
    size_t rows, columns, index = 0;
    std::string map_line;
    std::getline(fstream, map_line);
    std::sscanf(map_line.c_str(), "%zu %zu", &rows, &columns);
    Map* map = new Map(rows, columns, map_name);
    for (size_t row = 0; row < rows; ++row, index = 0) {
      std::getline(fstream, map_line);
      for (size_t col = 0; col < columns; ++col) {
        map->set_cell(row, col, map_line[index++]);
      }
    }
    this->map.push_back(map);
    this->days.push_back(days);
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
    // OpenMP test
    #pragma omp critical
    std::cout<< "thread: " << omp_get_thread_num() << ", map= "
      << map[index]->get_map_name() << std::endl;

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
