#include <iostream>
#include <unistd.h>

#include "GoldbachServer.hpp"
#include "NetworkAddress.hpp"

const char* const usage =
  "Usage: goldbachServer [response_server] [response_port] [port] [max_connections]\n"
  "\n"
  "  response_server  address of server to send the sums response\n"
  "  response_port    address of server to send the sums response\n"
  "  port             Network port to listen incoming number requests, default "
    DEFAULT_PORT "\n"
  "  max_connections  Maximum number of allowed client connections, default 10\n";

GoldbachServer& GoldbachServer::getInstance() {
  static GoldbachServer goldbachServer;
  return goldbachServer;
}

GoldbachServer::GoldbachServer() {
  GoldbachSums stopCondition;
  stopCondition.threadNumber = INT64_MAX;
  dispatcher = new SumsDispatcher(stopCondition);
  this->dispatcher->createOwnQueue();
  this->dispatcher->startThread();
}

GoldbachServer::~GoldbachServer() {
  delete dispatcher;
}

int GoldbachServer::start(int argc, char* argv[]) {
  try {
    if (this->analyzeArguments(argc, argv)) {
      this->startConsumers();
      this->startCalculators();
      this->registerQueues();
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      std::cout << "goldbach server listening on " << address.getIP()
        << " port " << address.getPort() << "...\n";
      this->acceptAllConnections();
    } else {
      this->stopDispatcher();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    /// Clean exit if error detected
    stopConsumers();
  }

  return EXIT_SUCCESS;
}

bool GoldbachServer::analyzeArguments(int argc, char* argv[]) {
  /// Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos || argc < 3 || argc > 5) {
      std::cout << usage;
      return false;
    }
  }
  this->response_server = argv[1];
  this->response_port = argv[2];

  if (argc > 3) {
    this->port = argv[3];
    if (argc > 4) {
      try {
        this->consumerCount = std::stoll(argv[4]);
      } catch (const std::exception& error) {
        std::cerr << "error: invalid consumer count\n";
        return false;
      }
    }
  }
  return true;
}

void GoldbachServer::handleClientConnection(Socket& client) {
  this->socketQueue.push(client);
}

void GoldbachServer::handleSumsRequest(std::string& sumsRequested, size_t thread_number) {
  size_t num_count = 0;
  // extract thread number of httpHandler from webserver
  size_t client_thread_num = stoll(sumsRequested);
  while ((sumsRequested[0] <= '9' && sumsRequested[0] >= '0')) {
    // remove thread number from string
    sumsRequested = sumsRequested.substr(1, sumsRequested.size() -1);
  }
  //remove 't' from string
  sumsRequested = sumsRequested.substr(1, sumsRequested.size() -1);

  while(!sumsRequested.empty()) {
    GoldbachNumber number;
    number.number = std::stoll(sumsRequested);
    number.threadNumber = thread_number;
    number.index = num_count;
    // push number to be processed by calculators
    numberQueue.push(number);
    ++num_count;
    // remove extracted number from string
    while ((sumsRequested[0] <= '9' && sumsRequested[0] >= '0')
      || sumsRequested[0] == '-') {
      sumsRequested = sumsRequested.substr(1, sumsRequested.size() -1);
    }
    // remove comma if multiple numbers where requested (%2C)
    if (sumsRequested.size() > 3) {
      sumsRequested = sumsRequested.substr(3, sumsRequested.size() -3);
    } else {
      sumsRequested.clear();
    }
  }
  sendResults(client_thread_num, num_count, thread_number);

}

void GoldbachServer::startCalculators() {
  /// Gets the number of processors
  size_t threadCount = sysconf(_SC_NPROCESSORS_ONLN);
  GoldbachNumber stopCondition;
  stopCondition.threadNumber = INT64_MAX;
  calculators.resize(threadCount);
  /// Gives each calculator the information needed to start
  for (size_t index = 0; index < threadCount; ++index) {
    calculators[index] = new AssemblerCalculator(stopCondition);
    calculators[index]->setConsumingQueue(&this->numberQueue);
    calculators[index]->setProducingQueue(this->dispatcher
      ->getConsumingQueue());
    calculators[index]->startThread();
  }
}

void GoldbachServer::stopProcessing() {
  size_t threadCount = calculators.size();
  GoldbachNumber numberStopCondition;
  numberStopCondition.threadNumber = INT64_MAX;
  /// Sends the stop condition to each calculator
  for (size_t index = 0; index < threadCount; ++index) {
    numberQueue.push(numberStopCondition);
  }
  this->stopDispatcher();
}

void GoldbachServer::registerQueues() {
  sumQueues.resize(consumerCount);
  /// Gives a queue to extract the answers from for each connection thread
  for (size_t index = 0; index < consumerCount; ++index) {
    sumQueues[index] = new Queue<GoldbachSums>();
    dispatcher->registerRedirect(index, sumQueues[index]);
  }
}

void GoldbachServer::startConsumers() {
  Socket stopCondition;
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new GoldbachConnectionHandler(this, stopCondition,
      index);
    assert(this->consumers[index]);
    this->consumers[index]->setConsumingQueue(&this->socketQueue);
    this->consumers[index]->startThread();
  }
}

void GoldbachServer::stopConsumers() {
  Socket socket;
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->socketQueue.push(socket);
  }
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }
}

void GoldbachServer::stopDispatcher() {
  GoldbachSums dispatcherStopCondition;
  dispatcherStopCondition.threadNumber = INT64_MAX;
  /// Sends the stop condition to the dispatchers consuming queue
  dispatcher->getConsumingQueue()->push(dispatcherStopCondition);
  dispatcher->waitToFinish();
}

void GoldbachServer::sendResults(size_t& client_thread_num, size_t& num_count, size_t& thread_number) {
  // vector to store results
  std::vector<std::vector<std::string>> sums;
  sums.resize(num_count);
  for (size_t index = 0; index < num_count; ++index) {
    // sleep until all num_count results are extracted from queue
    GoldbachSums goldbach_sums = sumQueues[thread_number]->pop();
    sums[goldbach_sums.index] = goldbach_sums.sums;
  }
  TcpClient client;
  Socket answer_socket = client.connect(response_server, response_port);
  //example answer format: 1t25: 5 sums.-9: 2 sums, 2 + 2 + 5, 3 + 3 + 3.
  answer_socket << client_thread_num << "t";
  for (size_t index = 0; index < num_count; ++index) {
    answer_socket << sums[index][0];
    for (size_t sum = 1; sum < sums[index].size(); ++sum) {
      answer_socket << "," << sums[index][sum];
    }
    answer_socket << ".";
  }
  answer_socket.send();
  answer_socket.close();
}
