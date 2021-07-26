// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "GoldbachConnectionHandler.hpp"
#include "Log.hpp"
#include "Socket.hpp"
#include <regex>

GoldbachConnectionHandler::GoldbachConnectionHandler(GoldbachServer* goldbachServer
  , Socket stopCondition, size_t threadNumber)
  : Consumer(nullptr, stopCondition, false), goldbachServer(goldbachServer),
  threadNumber(threadNumber) {}

int GoldbachConnectionHandler::run() {
  // Start the forever loop to consume all the connections that arrive
  this->consumeForever();

  // If the forever loop finished, no more client will arrive
  return EXIT_SUCCESS;
}

void GoldbachConnectionHandler::consume(const Socket& client) {
  Socket cpySocket = client;
  std::string sumsRequested;
  cpySocket.readLine(sumsRequested, '\0');
  std::cout << "Socket connection with value" << sumsRequested << '\n';
  cpySocket.close();

  // If the request is not valid or an error happened
  if (!validateRequest(sumsRequested)) {
    goldbachServer->handleSumsRequest(sumsRequested, threadNumber);
  }
}

bool GoldbachConnectionHandler::validateRequest(std::string sumsRequested) {
  //request format: (threadNumber)t(num1)%2C(num2)%2C(num3)...
  std::regex validRequest("\\d+t(-?\\d+(%2C-?\\d+)*)$");
  return std::regex_match(sumsRequested, validRequest);
}
