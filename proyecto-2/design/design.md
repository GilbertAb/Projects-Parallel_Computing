<!--- Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0 -->

The program is made so that the class Job is in control of the functionality, it receives the input from main and makes the output, gets the information needed to work with and creates the maps requested with the days said in the input and the simulation of passing time, for this its dependant on the class Map to work with each one of the requested maps applying the rules to update the cells with the passing days. File handling is not considered in the design

The program distributes each map dynamically between the processes and each process has a set of threads that work concurrently on the same map. Map are assigned to each process one by one by another process.

UML design:

 ![UML](../img/class_diagram.svg "UML design")

Class interaction:

 ![interaction](../img/colaboration_diagram.svg "Colaboration Diagram")

Pseudocode:

```cpp
// Proccess communication through network messages
// Notation used for network messages is:
// send(message, target_process),
// receive(message, source_process)

// Process 0 assigns work
// Remaining processes work with the maps assigned by process 0

get_job(job_file /*job textfile*/):
  if proccess_rank = 0
    map_count := count_lines(job_file) // each line is a map
    for map_index := 0 to map_count do
      receive(work_request, ANY_PROCESS)   // process asks for work
      send(map_index, ASKING_PROCESS)      // assign work to asking process
    end for
    receive(work_request, ALL_PROCESSES)  // wait for job to be finished
    send(stop_condition, ALL_PROCESSES)
  end if
  if process_rank != 0
    while true do
      send(work_request, 0)
      receive(map_index, 0)
      if map_index = stop_condition
        break
      end if
      map := create_map(job_file.line(map_index))  // create map based on map_indexth job line
      days := job_file.line(map_index)  // get day count from job line
      simulate_days(address of map, days);
    end while
  end if
```
```cpp
simulate_days(map, days):
  for day := 0 to days do
    map.end_day()
  end for
```
```cpp
end_day():
  static mapping parallel for row := 0 to row_count do
    for column := 0 to column_count do
      update_cell(row, col, address of next_day_map)
    end for
  end for
  current_day_map := next_day_map
```
```cpp
update_cell(row, column, next_day_map):
  tree_count := tree_neighbors()
  lake_count := lake_neighbors()
  meadow_count := meadow_neighbors()

  switch (map[row][columns])
    case 'a':
      if lake_count >= 4 then
        next_day_map[row][column] := 'l'
      else if tree count > 4 then
        next_day_map[row][column] := '-'
      else
        next_day_map[row][column] := 'a'
      end if
      break switch

    case 'l':
      if lake_count < 3 then
        next_day_map[row][column] := '-'
      else
        next_day_map[row][column] := 'l'
      end if
      break switch

    case '-':
      if tree_count >= 3 then
        next_day_map[row][column] := 'a'
      else
        next_day_map[row][column] := '-'
      end if
      break switch
```
