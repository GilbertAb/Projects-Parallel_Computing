#include "AnswerHandler.hpp"

AnswerHandler::AnswerHandler(AnswerServer* answerServer
  , Socket stopCondition)
  : Consumer(nullptr, stopCondition, false), server(answerServer) {}

int AnswerHandler::run() {
  /// Starts the cycle for consuming from the connection queue
  this->consumeForever();
  return EXIT_SUCCESS;
}
void AnswerHandler::consume(const Socket& socket) {
  Socket cpySocket = socket;
  /// Calls the method that knows how to process a socket
  server->getSocketInfo(cpySocket);
}
