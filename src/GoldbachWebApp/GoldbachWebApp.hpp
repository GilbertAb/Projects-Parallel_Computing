// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include <string>
#include <vector>
#include "HttpServer.hpp"
#include "GoldbachCalculator.hpp"

#define SUMS 0
#define HOME_PAGE -1
#define NOT_FOUND -2



class GoldbachWebApp {
 private:
  GoldbachCalculator calculator;
  
  void serveGoldbachSums(HttpResponse& httpResponse, std::vector<int64_t> numbers);
  void serveHomepage(HttpResponse& httpResponse);
  void serveNotFound(HttpResponse& httpResponse);

 public:
    GoldbachWebApp();
    ~GoldbachWebApp();
    bool serve(HttpResponse& httpResponse, int serve
      , const std::vector<int64_t>& numbers = std::vector<int64_t>());
};
#endif  // GOLDBACHWEBAPP_HPP
