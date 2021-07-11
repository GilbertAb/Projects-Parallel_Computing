// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef ASSEMBLERCALCULATOR_HPP
#define ASSEMBLERCALCULATOR_HPP

#include <string>
#include <vector>
#include "Assembler.hpp"
#include "GoldbachCalculator.hpp"

struct GoldbachNumber {
  size_t threadNumber;
  int64_t number;
};

struct GoldbachSums {
  size_t threadNumber;
  std::vector<std::string> sums;
};

class AssemblerCalculator
  : public Assembler<GoldbachNumber, GoldbachSums>
  , public GoldbachCalculator {

 public:
  AssemblerCalculator():Assembler(), GoldbachCalculator(){}
  ~AssemblerCalculator();
  void consume(const GoldbachNumber& data) override;
  //void produce(const GoldbachSums& data) override;
  int run() override;

};

#endif // ASSEMBLERCALCULATOR_HPP

