// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef SUMSASSEMBLER_HPP
#define SUMSASSEMBLER_HPP

#include <string>
#include <vector>
#include <regex>
#include "Assembler.hpp"
#include "common.hpp"
#include "Socket.h"
#include "GoldbachCalculator.hpp"

class SumsAssembler
  : public Assembler<Socket&, GoldbachSums> {
  DISABLE_COPY(SumsAssembler);

 public:
  explicit SumsAssembler(Socket stopCondition);
  ~SumsAssembler();
  void consume(const Socket& data) override;
  int run() override;
};

#endif  // SUMSASSEMBLER_HPP
