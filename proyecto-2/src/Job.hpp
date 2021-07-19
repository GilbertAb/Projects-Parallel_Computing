// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "common.hpp"
#include "Map.hpp"
/**
 * @brief Saves the Maps received and the days requested for each.
 * @details Recieves the input from the user and transforms the
 * text files into functional maps where operations can be performed and
 * simulates the passing of the resquested days generating the number of output
 * files requested by the user
 */
class Job {
  DISABLE_COPY(Job);

 private:
  /**< Array with the Map requested */
  std::vector<Map*> map;
  /**< Array with the days aligned in order with Map */
  std::vector<int64_t> days;

 public:
  /**
   * Constructor.
   */
  Job();

  /**
   * Destructor.
   * Frees the memory of the Map array;
   */
  ~Job();

  /**
   * @brief Gets the arguments to start running the program.
   * @param argc Number of arguments introduced by the user
   * @param argv Array of arguments intruduced by the user
   * @param process_count Number of processes working at the same time
   * @param rank Number identifying the actual process
   * @return if it was a succes or a fail
   */
  int run(int argc, char* argv[], int process_count, int rank);

  /**
   * @brief Gets the map path, maps, days and map name from the file and send
   * it to create the map and the corresponding days requested.
   * @details Opens the job file to get the name of the map and the days asked 
   * and sends the location of the map file so that information can be acquired,
   * repeats the process until every map mentioned in job is sent, assigns a
   * certain number of maps calculated by the total number of processes and the
   * current process rank
   * @param filename Path introduced by the user where job is supossed to be
   * @param process_count Total number of processes working at the same time
   * @param rank Number identifying the current process id
   */
  void get_job(const char* filename, int process_count, int rank);

  /**
   * @brief Creates and saves the Maps with the days requested
   * @details Retrieves the map size and map content from the map file
   * to make a copy, save and relate it to the maps name and days requested
   * @param map_path The path where the file containing the map is
   * @param map_name Name related to the map that will be saved
   * @param days Number of days related to the map
   */
  void create_map(std::string map_path, std::string map_name, int64_t days);

  /**
   * @brief Simulates the requested number of days to put in the output
   * @details Goes through each Map saved and apply the number of days saved
   * related to it generating output files depending on the request made
   * (prints all the results or just the last day), and its able to distribute
   * the processing of a map between a requested amount of threads
   * @param output_path The file path where the results are going to be stored
   * @param thread_count Number of threads in which the work will be distributed
   */
  void simulate_days(std::string output_path, size_t thread_count);

  /**
   * @brief Creates the directory for the outputs
   * @return The path to the newly created directory
   */
  std::string create_output_directory();

  /**
   * @brief Checks if the fstream file is open and if it isnt throws an error
   * @param fstream the fstream that will be checked
   * @param file the name of the file that was tried to open
   * @return a boolean result meaning if the file was succesfuly opened
   */
  bool is_open(std::fstream& fstream, std::string file);

  int analyze_arguments(int argc, char* argv[]);

  bool is_number(char* s);
};

#endif  // JOB_H
