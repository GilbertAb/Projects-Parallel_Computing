// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "GoldbachCalculator.hpp"

GoldbachCalculator::GoldbachCalculator() {}
GoldbachCalculator::~GoldbachCalculator() {}

bool GoldbachCalculator::isPrime(int64_t number) {
  bool prime = true;
  if (number % 2 == 0 && number > 2) {
    prime = false;
  }
  for (int64_t i = 3; i*i <= number && prime; i+=2) {
    if (number % i == 0) {
      prime = false;
    }
  }
  return prime;
}

std::vector<std::vector<std::string>> GoldbachCalculator
  ::getGoldbachSums(std::vector<int64_t> numbers) {
  size_t number_count = numbers.size();
  std::vector<std::vector<std::string>> numbers_sums(number_count);

  for (size_t index = 0; index < number_count; ++index) {
    if (numbers[index] > -6 && numbers[index] < 6) {
      numbers_sums[index].push_back(std::to_string(numbers[index]) + ": NA\n");
    } else {
        if (numbers[index] % 2 == 0) {
          numbers_sums[index] = strongGoldbach(numbers[index]);
        } else {
          numbers_sums[index] = weakGoldbach(numbers[index]);
        }
      }
  }
  return numbers_sums;
}

std::vector<std::string> GoldbachCalculator::strongGoldbach(int64_t number) {
  bool show_sums = number < 0;
  size_t sum_count = 0;
  std::vector<std::string> sums(1, std::to_string(number));
  if (show_sums) {
    number = -number;
  }
  for (int64_t lowerIndex = 2, upperIndex = number-2;
  upperIndex >= lowerIndex; upperIndex--, lowerIndex++) {
    if (isPrime(lowerIndex) && isPrime(upperIndex)) {
      ++sum_count;
      if (show_sums) {
        sums.push_back(std::to_string(lowerIndex) + " + "
          + std::to_string(upperIndex));
      }
    }
  }
  sums[0] += ": " + std::to_string(sum_count)+" sums";
  if (show_sums) {
    sums[0] += ":";
  }
  return sums;
}

std::vector<std::string> GoldbachCalculator::weakGoldbach(int64_t number) {
  bool show_sums = number < 0;
  size_t sum_count = 0;
  std::vector<std::string> sums(1, std::to_string(number));
  if (show_sums) {
    number = -number;
  }
  int64_t lowerIndex = 2, midIndex = 2, upperIndex = number - 4;
  for (; upperIndex >= lowerIndex; lowerIndex++, upperIndex--) {
    if (isPrime(lowerIndex)) {
      for (int64_t mi = midIndex, ui = upperIndex; ui >= mi; ui--, mi++) {
        if (isPrime(mi) && isPrime(ui) && mi >= lowerIndex) {
          ++sum_count;
          if (show_sums) {
            sums.push_back(std::to_string(lowerIndex) + " + "
              + std::to_string(mi) + " + " + std::to_string(ui));
          }
        }
      }
    }
  }
  sums[0] += ": " + std::to_string(sum_count)+" sums";
  if (show_sums) {
    sums[0] += ":";
  }
  return sums;
}
