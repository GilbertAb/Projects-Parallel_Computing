// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0
#include "DispatcherTest.hpp"

DispatcherTest::DispatcherTest(){}

int DispatcherTest::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

int DispatcherTest::extractKey(const GoldbachSums& data) const {
  return data.threadNumber;
}
