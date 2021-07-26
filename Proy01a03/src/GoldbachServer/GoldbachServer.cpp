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
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    /// Clean exit if error detected
    stopConsumers();
  }
  std::cout <<"terminating\n";
  dispatcher->waitToFinish();

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
  std::cout << "pushing client\n";
  this->socketQueue.push(client);
}

void GoldbachServer::handleSumsRequest(std::string& sumsRequested, size_t thread_number) {
  std::cout << "handling request " << sumsRequested << '\n';
  size_t num_count = 0;
  size_t client_thread_number = stoll(sumsRequested);       // extract thread number of httpHandler
  while ((sumsRequested[0] <= '9' && sumsRequested[0] >= '0')) {
    sumsRequested = sumsRequested.substr(1, sumsRequested.size() -1); // remove thread number
  }
  sumsRequested = sumsRequested.substr(1, sumsRequested.size() -1); //remove 't'
  std::cout << "handling remaining request " << sumsRequested << '\n';
  while(!sumsRequested.empty()) {
    GoldbachNumber number;
    number.number = std::stoll(sumsRequested);
    number.threadNumber = thread_number;
    number.index = num_count;
    std::cout << "pushing number " << number.number << '\n';
    numberQueue.push(number);
    ++num_count;
    while ((sumsRequested[0] <= '9' && sumsRequested[0] >= '0')
      || sumsRequested[0] == '-') {
      sumsRequested = sumsRequested.substr(1, sumsRequested.size() -1);
    }
    if (sumsRequested.size() > 3) {
      sumsRequested = sumsRequested.substr(3, sumsRequested.size() -3);
    } else {
      sumsRequested.clear();
    }
  }

  std::vector<std::vector<std::string>> sums;
  sums.resize(num_count);
  for (size_t index = 0; index < num_count; ++index) {
    GoldbachSums goldbach_sums = sumQueues[thread_number]->pop();
    sums[goldbach_sums.index] = goldbach_sums.sums;
  }
  for (size_t index = 0; index < sums.size(); ++index) {
    for (size_t index2 = 0; index2 < sums.size(); ++index2) {
      std::cout << sums[index][index2] << '\n';
    } 
  }
  TcpClient client;
  Socket answer_socket = client.connect(response_server, response_port);
  answer_socket << client_thread_number << 't';
  for (size_t index = 0; index < num_count; ++index) {
    answer_socket << sums[index][0];
    for (size_t sum = 1; sum < sums[index].size(); ++index) {
      answer_socket << ',' << sums[index][sum];
    }
    answer_socket << '.';
  }
  answer_socket.send();
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
  GoldbachSums dispatcherStopCondition;
  dispatcherStopCondition.threadNumber = INT64_MAX;
  /// Sends the stop condition to the dispatchers consuming queue
  dispatcher->getConsumingQueue()->push(dispatcherStopCondition);
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
  stopCondition.setSocketFileDescriptor(999);
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
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    Socket socket;
    this->socketQueue.push(socket);
  }
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }
}
