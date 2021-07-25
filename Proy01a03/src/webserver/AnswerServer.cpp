#include "AnswerServer.hpp"
AnswerServer::AnswerServer() {
  /// Creates the queue with the answers
  this->answerQueue = new Queue<std::string>();
  connectionHandler = new AnswerHandler(this->getInstance());
  /// Makes the handler create its own socket queue and saves it
  connectionHandler->createOwnQueue();
  this->socketQueue = connectionHandler->getConsumingQueue();
}
AnswerServer::~AnswerServer() {
}
AnswerServer& AnswerServer::getInstance() {
  static AnswerServer answerServer;
  return answerServer;
}
}
void AnswerServer::handleClientConnection(Socket& client) {
  /// Saves the client in a queue
  socketQueue->push(client);
}
void AnswerServer::run() {
  /// Starts the handler thread and continues into a cycle of accepting requests
  connectionHandler->startThread();
  this->listenForever(this->port);
}
Queue<std::string>* AnswerServer::getQueue() {
  return this->answerQueue;
}
void AnswerServer::getSocketInfo(Socket& client) {
  std::string answer;
  /// Gets the string message from the socket
  client.readLine(&answer, '\0');
  answerQueue.push(answer);
}
