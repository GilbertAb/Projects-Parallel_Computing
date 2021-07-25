#include "AnswerHandler.hpp"
AnswerHandler::AnswerHandler(AnswerServer* answerServer) {
  this->server = answerServer;
}
int AnswerHandler::run() {
  /// Starts the cycle for consuming from the connection queue
  this->consumeForever();
  return EXIT_SUCESS;
}
void AnswerHandler::consume(const Socket& socket) {
  /// Calls the method that knows how to process a socket
  server->getSocketInfo(socket);
}
