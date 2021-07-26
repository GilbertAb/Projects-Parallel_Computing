/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>

/// Declare four of the rule-of-the-five methods
#define DECLARE_RULE4(Class, action) \
  Class(const Class& other) = action; \
  Class(Class&& other) = action; \
  Class& operator=(const Class& other) = action; \
  Class& operator=(Class&& other) = action

/// Disable default methods for copying objects
#define DISABLE_COPY(Class) \
  DECLARE_RULE4(Class, delete)

/// For data transmissions
typedef unsigned char byte;

/// Struct for the answer
struct GoldbachSums {
  size_t threadNumber;
  size_t index;
  std::vector<std::string> sums;

  bool operator==(const GoldbachSums& other) const {
  return threadNumber == other.threadNumber;
  }
};

#endif  // COMMON_HPP
