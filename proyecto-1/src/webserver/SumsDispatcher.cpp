// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0
#include "SumsDispatcher.hpp"

SumsDispatcher::SumsDispatcher(){}

int SumsDispatcher::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

size_t SumsDispatcher::extractKey(const GoldbachSums& data) const {
  return data.threadNumber;
}
