// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "AssemblerCalculator.hpp"

AssemblerCalculator::AssemblerCalculator() {}
AssemblerCalculator::~AssemblerCalculator() {}

int AssemblerCalculator::run() {
  this->consumeForever();

  return EXIT_SUCCESS;
}

void AssemblerCalculator::consume(const GoldbachNumber& data) {
  GoldbachSums sums;
  sums.threadNumber = data.threadNumber;
  (data.number % 2 == 0) ? sums.sums = strongGoldbach(data.number) : weakGoldbach(data.number);
  produce(sums); 
}
