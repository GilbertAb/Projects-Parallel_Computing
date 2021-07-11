// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef SUMSDISPATCHER_HPP
#define SUMSDISPATCHER_HPP

#include "AssemblerCalculator.hpp"
#include "common.hpp"
#include "Dispatcher.hpp"

/**
 * @brief A Dispatcher class example
 */
class SumsDispatcher : public Dispatcher<size_t, GoldbachSums> {
  DISABLE_COPY(SumsDispatcher);

 protected:

 public:
  /// Constructor
  explicit SumsDispatcher();
  /// Start redirecting network messages
  int run() override;
  /// This method extracts the key from a data stored in the fromQueue
  size_t extractKey(const GoldbachSums& data) const override;
};

#endif  // SUMSDISPATCHER_HPP
