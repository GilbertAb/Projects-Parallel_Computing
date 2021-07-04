#include "Island.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

Island::Island(){}

Island::~Island() {
  for(size_t index = 0; index < forest.size(); ++index) {
    delete forest[index];
  }
}

void Island::get_job(const char* filename) {
  std::string sfilename(filename);
  std::string directory;
  const size_t last_slash_pos = sfilename.rfind('/');
  if (std::string::npos != last_slash_pos)
  {
      directory = sfilename.substr(0, last_slash_pos + 1);
  }

  // TODO(any) make string instead of char buffer
  std::string map_name;
  int64_t days;

  std::fstream fstream;
  fstream.open(filename, std::ios::in);
  std::string job_line;
  char buffer[256];
  
  while (std::getline(fstream, job_line)) {
    std::sscanf(job_line.c_str(), "%s%ld", buffer, &days);
    map_name.assign(buffer, strlen(buffer));
    map_name = map_name.substr(0, job_line.rfind("."));
    create_forest(directory + map_name + ".txt", map_name, days);
  }
}

void Island::create_forest(std::string map_path, std::string map_name, int64_t days) {
  std::fstream fstream;
  fstream.open(map_path, std::ios::in);
  size_t rows, columns, index = 0;
  std::string map_line;
  std::getline(fstream, map_line);
  std::sscanf(map_line.c_str(), "%zu %zu", &rows, &columns);
  Forest* forest = new Forest(rows, columns, map_name);
  for (size_t row = 0; row < rows; ++row, index = 0) {
    std::getline(fstream, map_line);
    for (size_t col = 0; col < columns; ++col) {
      forest->set_cell(row, col, map_line[index++]);
    }
  }
  this->forest.push_back(forest);
  this->days.push_back(days);
}

void Island::simulate_days(std::string output_directory_path) {
  // TODO(any) make this loop concurrent
  for (size_t index = 0; index < forest.size(); ++index) {
    std::fstream fstream;
    // TODO(any) find a more according name for print_all flag
    size_t print_all = 0;

    if (days[index] < 0) {
      days[index] = -days[index];
      print_all = days[index];
    }

    for (size_t day = 0; day < (size_t)days[index]; ++day) {
      std::string output_path = output_directory_path + forest[index]->get_map_name() + '-';
      forest[index]->end_day();
      if (day+1 >= print_all) {
      output_path += std::to_string(day+1) + ".txt"; 
      fstream.open(output_path, std::ios::out);
      fstream << forest[index]->to_string();
      fstream.close();
      }
    }
  }
}

// TODO(any) receive path as command argument

std::string Island::create_output_directory(std::string job_path) {
  std::string output_directory = job_path;
  output_directory = output_directory.substr(0, output_directory.rfind("input"));
  output_directory += "output/";
  return output_directory;
}
