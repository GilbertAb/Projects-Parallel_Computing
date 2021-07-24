// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef ASSEMBLERCALCULATOR_HPP
#define ASSEMBLERCALCULATOR_HPP

#include <string>
#include <vector>
#include "Assembler.hpp"
#include "common.hpp"
#include "GoldbachCalculator.hpp"

struct GoldbachNumber {
  size_t threadNumber;
  int64_t number;
  size_t index;

  bool operator==(const GoldbachNumber& other) const {
  return threadNumber == other.threadNumber;
  }
};

struct GoldbachSums {
  size_t threadNumber;
  size_t index;
  std::vector<std::string> sums;

  bool operator==(const GoldbachSums& other) const {
  return threadNumber == other.threadNumber;
  }
  void operator=(const GoldbachSums& other){
    threadNumber = other.threadNumber;
    index = other.index;
    sums = other.sums;
  }
};

class AssemblerCalculator
  : public Assembler<GoldbachNumber, GoldbachSums>
  , public GoldbachCalculator {
  DISABLE_COPY(AssemblerCalculator);

 public:
  explicit AssemblerCalculator(GoldbachNumber stopCondition);
  ~AssemblerCalculator();
  void consume(const GoldbachNumber& data) override;
  int run() override;
};

#endif  // ASSEMBLERCALCULATOR_HPP
