// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>

#ifndef FOREST_H
#define FOREST_H

#include <iostream>
#include <fstream>
#include <stdlib.h> 

class Forest {

 private:
  char** map;
  char** mapCopy;
  int rowsCount;
  int columnsCount;

  bool flood(int row, int column);
  bool overcrowding(int row, int column);
  bool drought(int row, int column);
  bool reforestation(int row, int column);
  char** createMatrix(int rowsCount, int columnsCount);
  void freeMatrix(int rowsCount, char** matrix);
 public:
  /// Constructor
  Forest();
  Forest(int rows, int columnsCount);
  ~Forest();
  
  void setCell(int row, int column, char element);
  void updateForest(); // one midnight
  void updateCell(int row, int column);
  void makeMapCopy();
  int countNeighborCells(int row, int column, char type);
  void printForest();
  int getRowsCount();
  int getColumnsCount();
};

#endif  // FOREST_H
