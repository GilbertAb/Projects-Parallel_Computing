// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHCALCULATOR_HPP
#define GOLDBACHCALCULATOR_HPP

#include <string>
#include <vector>

class GoldbachCalculator {
 protected:
  /**
  * @brief Verifies if a number is prime or not, then returns the result.
  * @details Verifies if a number is prime checking if it has more than 2 
  * divisors.
  * @param number The number that will be verified.
  * @return true if the number is prime or false if it's not.
  */
  bool isPrime(int64_t number);

  /**
   * @brief Calculates the amount of goldbach sums and returns a vector of 
   * strings filled (or not) with the Goldbach sums of the given number.
   * @details To fill the vector with the Goldbach sums of the number, it 
   * checks if the two numbers of the sum are prime. Also, is verified if 
   * the number is negative to determine if the sums will be showed.
   * @param number The number whose goldbach sums will be calculated 
   * @return a vector of strings filled with the goldbach sums of 
   * the given number.
  */
  std::vector<std::string> strongGoldbach(int64_t number);

  /**
   * @brief Calculates the amount of goldbach sums and returns a vector of 
   * strings filled (or not) with the Goldbach sums of the given number.
   * @details To fill the vector with the Goldbach sums of the number, it 
   * checks if the three numbers of the sum are prime. Also, is verified if 
   * the number is negative to determine if the sums will be showed.
   * @param number The number whose goldbach sums will be calculated 
   * @return a vector of strings filled with the goldbach sums of 
   * the given number.
  */
  std::vector<std::string> weakGoldbach(int64_t number);

 public:
  /** Default Constructor.
  */
  GoldbachCalculator();

  /** Default Destructor.
  */
  ~GoldbachCalculator();

  /**
   * @brief Returns a vector of strings filled with the Goldbach sums of 
   * the given numbers.
   * @details To fill the vector with the Goldbach sums of the numbers 
   * every number is checked to see if it has no sums, it's even or odd, 
   * then the goldbach sums will be calculated by strongGoldbach or 
   * weakGoldbach.
   * @param numbers The vector of numbers whose goldbach sums will be 
   * calculated.
   * @return a vector of strings filled with the goldbach sums of 
   * the given numbers.
  */
  std::vector<std::vector<std::string>> getGoldbachSums(
    std::vector<int64_t> numbers);
};
#endif  // GOLDBACHCALCULATOR_HPP
