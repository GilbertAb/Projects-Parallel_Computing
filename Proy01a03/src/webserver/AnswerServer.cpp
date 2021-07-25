#include "AnswerServer.hpp"
AnswerServer::AnswerServer(std::vector<Queue<GoldbachSums>*>* answerQueues) {
  /// Creates the queue with the answers
  this->answerQueues = answerQueues;
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
void AnswerServer::getSocketInfo(Socket& client) {
  std::string answer;
  size_t index;
  size_t endPos;
  /// Gets the string message from the socket to get index
  client.readLine(&answer, 't');
  index = std::stoll(answer);
  /// Gets the number of sums and the sums if requested
  while (client.readLine(&answer, '.')) {
    GoldbachSums sums;
    while ( answer.find(",") != std::string::npos ) {
      endPos = answer.find(",");
      sums.sums.push_back(answer.substr(0,endPos));
      answer = answer.substr(endPos+1, answer.lenght() - endPos -1);
    }
    sums.sums.push_back(answer);
    answerQueues[index]->push(sums);
  }
}
