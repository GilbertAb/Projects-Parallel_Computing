// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>

#include "Forest.hpp"

using namespace std;

const std::string DIRECTIONS[] = {"N","NE","E","SE","S","SO","O","NO"};
const int SUMROW[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int SUMCOL[] = {0, 1, 1, 1, 0, -1, -1, -1};
#define NUMDIRECTIONS 8

Forest::Forest() {
  this->rowsCount = 0;
  this->columnsCount = 0;
}
Forest::Forest(int rowsCount, int columnsCount) {
  this->rowsCount = rowsCount;
  this->columnsCount = columnsCount;

  map = createMatrix(rowsCount, columnsCount);
  mapCopy = createMatrix(rowsCount, columnsCount);
}

Forest::~Forest() {
  freeMatrix(rowsCount, map);
  freeMatrix(rowsCount, mapCopy);
}
  
void Forest::setCell(int row, int column, char element) {
  this->map[row][column] = element;
}

void Forest::updateForest() { // one midnight
  makeMapCopy();
  for(int row = 0; row < this->rowsCount; row++) {
    for(int column = 0; column < this->columnsCount; column++) {
      updateCell(row,column);
    }
  }
}
// Updates cell for the map
void Forest::updateCell(int row, int column) {
  switch(map[row][column]) {
    case 'a':
      if(flood(row, column)) {
        map[row][column] = 'l';
      } else {
        if(overcrowding(row, column)) {
          map[row][column] = '-';
        }
      }
    break;

    case 'l':
      if(drought(row, column)) {
        map[row][column]= '-';
      }
    break;

    case '-': 
      if(reforestation(row, column)) {
        map[row][column]= 'a';
      }

    break;
  }
  // Estabilidad: Cualquier otra situación, la celda permanece como está.

}

void Forest::makeMapCopy() {
  for(int row = 0; row < this->rowsCount; row++) {
    for(int column = 0; column < this->columnsCount; column++) {
      mapCopy[row][column] = map[row][column];
    }
  }
}

int Forest::countNeighborCells(int row, int column, char type) {
  int numNeighborsOfType = 0;
  for (int direction = 0; direction < NUMDIRECTIONS; direction++) {
    int dirRow = row + SUMROW[direction];
    int dirColumn = column + SUMCOL[direction];

    if(dirRow >= 0 && dirRow < this->rowsCount && dirColumn >= 0 && dirColumn < this->columnsCount) {
      if(mapCopy[dirRow][dirColumn] == type) {
        numNeighborsOfType++;
      }
    }
  }
  return numNeighborsOfType;
}

void Forest::printForest() {
  for(int row = 0; row < this->rowsCount; row++) {
    for(int column = 0; column < this->columnsCount; column++) {
      std::cout << map[row][column];
    }
    std::cout << endl;
  }
}

/*Inundación: Si la celda tiene un árbol y al menos 4 vecinos que son lago encantado('l'),
  entonces el lago ahoga el árbol, y pasa a ser lago encantado('l').*/
bool Forest::flood(int row, int column) {
  return countNeighborCells(row, column, 'l') >= 4;
}

/*Hacinamiento: Si la celda es un árbol y tiene más de 4 vecinos árbol, el exceso de sombra
  evita que crezca y entonces pasa a ser pradera.*/
bool Forest::overcrowding(int row, int column) {
  return countNeighborCells(row, column, 'a') > 4;
}

/* Sequía: Si la celda es lago encantado y tiene menos de 3 vecinos que sean lago encantado,
  entonces el lago se seca y pasa a ser pradera.*/
bool Forest::drought(int row, int column) {
  return countNeighborCells(row, column, 'l') < 3;
}

/*Reforestación: Si la celda es pradera y tiene al menos 3 vecinos árboles, las semillas 
  tendrán espacio para crecer y la celda se convierte en árbol.*/
bool Forest::reforestation(int row, int column) {
  return countNeighborCells(row, column, 'a') >= 3;
}

int Forest::getRowsCount() {
  return rowsCount;
}
int Forest::getColumnsCount() {
  return columnsCount;
}

char** Forest::createMatrix(int rowsCount, int columnsCount) {
  char** matrix = (char**) calloc(rowsCount, sizeof(char*));
  if (matrix == NULL){
    return NULL;  
  }
  
  for (int row = 0; row < rowsCount; ++row) {
    if((matrix[row] = (char*) calloc(columnsCount, sizeof(char))) == NULL) {
      freeMatrix(rowsCount, matrix);
      return NULL;
    }
  }

  return matrix;
}

void Forest::freeMatrix(const int rowsCount, char** matrix) {
  if (matrix) {
    for (size_t row = 0; row < rowsCount; ++row){
      free(matrix[row]);
    }
  }
  free(matrix);
}