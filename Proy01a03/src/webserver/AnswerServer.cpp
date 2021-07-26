#include "AnswerServer.hpp"
#include "NetworkAddress.hpp"
#include <iostream>

AnswerServer::AnswerServer(std::vector<Queue<GoldbachSums>*>* answerQueues, const char* port) {
  /// Creates the queue with the answers
  this->answerQueues = answerQueues;
  this->port = port;
}
AnswerServer::~AnswerServer() {
}
// AnswerServer& AnswerServer::getInstance() {
//   static AnswerServer answerServer;
//   return answerServer;
// }

void AnswerServer::handleClientConnection(Socket& client) {
  /// Saves the client in a queue
  std::cout << "Answer server pushing client\n";
  socketQueue.push(client);
}
int AnswerServer::run() {
  /// Starts the handler thread and continues into a cycle of accepting requests
  this->startConsumers();
  this->listenForConnections(this->port);
  const NetworkAddress& address = this->getNetworkAddress();
  std::cout << "answer server listening on " << address.getIP()
    << " port " << address.getPort() << "...\n";
  this->acceptAllConnections();
}
void AnswerServer::getSocketInfo(Socket& client) {
  std::string answer;
  size_t index;
  size_t endPos;
  /// Gets the string message from the socket to get index
  client.readLine(answer, 't');
  std::cout<< "answer thread " << answer << '\n';
  index = std::stoll(answer);
  /// Gets the number of sums and the sums if requested
  while (client.readLine(answer, '.')) {
    std::cout << "line read " << answer << '\n';
    GoldbachSums sums;
    while ( answer.find(",") != std::string::npos ) {
      endPos = answer.find(",");
      sums.sums.push_back(answer.substr(0,endPos));
      answer = answer.substr(endPos+1, answer.length() - endPos -1);
    }
    sums.sums.push_back(answer);
    (*answerQueues)[index]->push(sums);
  }
}

void AnswerServer::startConsumers() {
  Socket stopCondition;
  stopCondition.setSocketFileDescriptor(999);
  this->consumers.resize(this->consumerCount);
  for (size_t index = 0; index < this->consumerCount; ++index) {
    this->consumers[index] = new AnswerHandler(this, stopCondition);
    assert(this->consumers[index]);
    this->consumers[index]->setConsumingQueue(&this->socketQueue);
    this->consumers[index]->startThread();
  }
}

void AnswerServer::stopConsumers() {
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    Socket socket;
    socket.setSocketFileDescriptor(999);
    this->socketQueue.push(socket);
  }
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }
}
