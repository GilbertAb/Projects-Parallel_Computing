// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "SumsAssembler.hpp"

SumsAssembler::SumsAssembler(GoldbachSums stopCondition)
: Assembler(nullptr, nullptr, stopCondition) {
}
SumsAssembler::~SumsAssembler() {}

int SumsAssembler::run() {
  this->consumeForever();

  return EXIT_SUCCESS;
}

void SumsAssembler::consume(const Socket& data) {
  GoldbachSums sums;

  std::smatch matches;

  // Get threadNumber from string
  std::regex threadReg("t");
  std::regex_search(sums,matches,threadReg);
  size_t threadNumber = std::stoi(matches.prefix().str());
  // Save string without threadNumber characters
  sums = matches.suffix().str();

  // Characters to get goldbach sums
  std::regex goldbachReg("(\\d:(\\d(\\+|,| ))+)");
  while(std::regex_search(sums, matches,goldbachReg)) {
    // Store match of
    sums = matches.suffix().str(); 
    /*
    GoldbachSums goldbachSums;
    goldbachSums.threadNumber = threadNumber.
    goldbahSums.sums = s
    */
  }
  produce(GoldbachSums);
}


