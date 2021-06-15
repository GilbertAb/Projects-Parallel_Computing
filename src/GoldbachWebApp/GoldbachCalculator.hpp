// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHCALCULATOR_HPP
#define GOLDBACHCALCULATOR_HPP

#include <string>
#include <vector>

class GoldbachCalculator {
 private:
    bool isPrime(int64_t number);
    std::vector<std::string> strongGoldbach(int64_t number);
    std::vector<std::string> weakGoldbach(int64_t number);

 public:
    GoldbachCalculator();
    ~GoldbachCalculator();
    std::vector<std::vector<std::string>> getGoldbachSums(
      std::vector<int64_t> numbers);
};
#endif  // GOLDBACHCALCULATOR_HPP
