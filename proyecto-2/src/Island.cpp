#include "Island.hpp"
#include <fstream>
#include <iostream>

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
      directory = sfilename.substr(0, last_slash_pos);
  }

  // TODO(any) make string instead of char buffer
  char* map_name = new char[256];
  int64_t days;


  std::fstream fstream;
  fstream.open(filename, std::ios::in);
  std::string job;
  while (std::getline(fstream, job)) {
    std::sscanf(job.c_str(), "%s%ld", map_name, &days);
    std::string map_path(directory);
    map_path += "/";
    map_path += map_name;
    create_forest(map_path.c_str(), days);
  }
  delete[] map_name;
}

void Island::create_forest(const char* map_name, int64_t days) {
  std::fstream fstream;
  fstream.open(map_name, std::ios::in);
  size_t rows, columns, index = 0;
  std::string map;
  std::getline(fstream, map);
  std::sscanf(map.c_str(), "%zu %zu", &rows, &columns);
  Forest* forest = new Forest(rows, columns, map_name);
  for (size_t row = 0; row < rows; ++row, index = 0) {
    std::getline(fstream, map);
    for (size_t col = 0; col < columns; ++col) {
      forest->set_cell(row, col, map[index++]);
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
    std::string output_path = output_directory_path + forest[index]->get_map_name() + '-';
    for (size_t day = 0; day < (size_t)days[index]; ++day) {
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
  output_directory.substr(0, output_directory.rfind("input") - 1);
  output_directory += "output/";
  return output_directory;
}

int main(int argc, char* argv[]) {
  Island island;
  island.get_job(argv[1]);
  island.simulate_days(island.create_output_directory(argv[1]));
}