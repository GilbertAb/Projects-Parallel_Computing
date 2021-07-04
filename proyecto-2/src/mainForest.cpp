// Copyright 2021 Gilbert Marquez Aldana <gilbert.marquez@ucr.ac.cr>
//typedef basic_string<char> string;
#include <iostream>
#include <fstream>
#include "Forest.hpp"
using namespace std;


int main(int argc, char* argv[]) {
  Forest forest = Forest(7,7);

  for(int c = 0; c < 7; c++) {
    forest.setCell(0,c,'-');
  }
  forest.setCell(1,0,'-');
  forest.setCell(1,1,'l');
  forest.setCell(1,2,'-');
  forest.setCell(1,3,'-');
  forest.setCell(1,4,'l');
  forest.setCell(1,5,'-');
  forest.setCell(1,6,'-');

  forest.setCell(2,0,'-');
  forest.setCell(2,1,'l');
  forest.setCell(2,2,'l');
  forest.setCell(2,3,'-');
  forest.setCell(2,4,'-');
  forest.setCell(2,5,'-');
  forest.setCell(2,6,'-');

  for(int c = 0; c < 7; c++) {
    forest.setCell(3,c,'-');
  }
  forest.setCell(3,1,'l');

  forest.setCell(4,0,'-');
  forest.setCell(4,1,'-');
  forest.setCell(4,2,'-');
  forest.setCell(4,3,'l');
  forest.setCell(4,4,'a');
  forest.setCell(4,5,'a');
  forest.setCell(4,6,'-');

  forest.setCell(5,0,'-');
  forest.setCell(5,1,'a');
  forest.setCell(5,2,'a');
  forest.setCell(5,3,'-');
  forest.setCell(5,4,'a');
  forest.setCell(5,5,'l');
  forest.setCell(5,6,'-');

  forest.setCell(6,0,'a');
  forest.setCell(6,1,'-');
  forest.setCell(6,2,'a');
  forest.setCell(6,3,'-');
  forest.setCell(6,4,'-');
  forest.setCell(6,5,'-');
  forest.setCell(6,6,'-');

  for(int i = 0; i < 10; i++) {
    cout << "map: "<< endl;
    forest.printForest();
    forest.updateForest();
    std::cout << i+1<<":"<< endl;
    forest.printForest();
  }


  return 0;
}
