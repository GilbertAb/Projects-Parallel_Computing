// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <unistd.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "WebServer.hpp"
#include "TcpClient.hpp"

const char* const usage =
  "Usage: webserv [hosts] [port] [max_connections] [answer_port] [max_connections_ans]\n"
  "\n"
  "  hosts                text file with the address and ports of goldbach servers\n"
  "  port                 Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  max_connections      Maximum number of allowed client connections\n"
  "  answer_port          Network port to listen incoming results from goldbach servers, default "
    DEFAULT_PORT2 "\n"
  "  max_connections_ans  Maximum number of allowed concurrent goldbach servers connections\n";

WebServer& WebServer::getInstance() {
  static WebServer webServer;
  return webServer;
}

WebServer::WebServer() {
  srand(time(0));
}
WebServer::~WebServer() {}

int WebServer::start(int argc, char* argv[]) {
  try {
    if (this->analyzeArguments(argc, argv)) {
      this->startConsumers();
      sumQueues.resize(consumerCount);
      for (size_t index = 0; index < consumerCount; ++index) {
        sumQueues[index] = new Queue<GoldbachSums>();
      }

      answerServer.start(&sumQueues, answerPort, answerConsumerCount);
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      std::cout << "web server listening on " << address.getIP()
        << " port " << address.getPort() << "...\n";
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    /// Clean exit if error detected
    stopWorkers();
    stopConsumers();
  }

  return EXIT_SUCCESS;
}

bool WebServer::analyzeArguments(int argc, char* argv[]) {
  /// Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos || argc > 6 || argc < 2) {
      std::cout << usage;
      return false;
    }
  }
  std::ifstream is;
  is.open(argv[1]);
  if (is.is_open()) {
    std::stringstream buffer;
    buffer << is.rdbuf();
    is.close();
    buffer >> hostCount;
    hosts.resize(hostCount);
    for (size_t index = 0; index < hostCount; ++index) {
      hosts[index].resize(2);
      buffer >> hosts[index][0];
      buffer >> hosts[index][1];
    }
  } else {
    std::cerr << "error: could not open the specified file\n";
    return false;
  }
  if (argc > 2) {
    this->port = argv[2];
  }
  if (argc > 3) {
    try {
      this->consumerCount = std::stoll(argv[3]);
    } catch (const std::exception& error) {
      std::cerr << "error: invalid consumer count\n";
      return false;
    }
  }
  if (argc > 4) {
    this->answerPort = argv[4];
  }
  if (argc > 5) {
    try {
      this->answerConsumerCount = std::stoll(argv[5]);
    } catch (const std::exception& error) {
      std::cerr << "error: invalid answer server consumer count\n";
      return false;
    }
  }
  return true;
}

bool WebServer::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse, size_t threadNumber) {
  /// Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  std::cout << "connection established with client " << address.getIP()
    << " port " << address.getPort() << std::endl;

  /// Print HTTP request
  std::cout << "  " << httpRequest.getMethod()
    << ' ' << httpRequest.getURI()
    << ' ' << httpRequest.getHttpVersion() << std::endl;

  return this->route(httpRequest, httpResponse, threadNumber);
}

bool WebServer::route(HttpRequest& httpRequest, HttpResponse& httpResponse,
  size_t threadNumber) {
  /// If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return webApp.serveHomepage(httpResponse);
  }
  std::smatch matches;

  /// If a number was asked in the form "/goldbach/1223"
  /// or "/goldbach?number=1223%2C500"
  try {
    std::regex inQuery("^/goldbach(/|\\?numbers=)(-?\\d+(%2C-?\\d*)*)$");
    if (std::regex_search(httpRequest.getURI(), matches, inQuery)) {
      assert(matches.length() >= 3);
      std::string numberstr = httpRequest.getURI();
      TcpClient client;
      // random mapping to choose server
      size_t server = rand() % hostCount;
      std::cout << "connection to " << hosts[server][0].c_str() << " " << hosts[server][1].c_str() << '\n';
      Socket socket = client.connect(hosts[server][0].c_str(), hosts[server][1].c_str());
      socket << threadNumber << "t";
      
      size_t startPos = numberstr.find("=");
      socket << numberstr.substr(startPos+1, numberstr.length() - startPos - 1);

      std::regex comma("%2C");
      size_t numCount = 1;
      while(std::regex_search(numberstr, matches,comma)) {
        numberstr = matches.suffix().str();
        numCount++;
      }
      
      socket.send();

      std::vector<std::vector<std::string>> sums;
      sums.resize(numCount);
      // Consume the goldbach sums and store them in a vector
      // (pop sums from the queue)
      for (size_t index = 0; index < numCount; ++index) {
        GoldbachSums goldbach_sums = sumQueues[threadNumber]->pop();
        sums[index] = goldbach_sums.sums;
      }
      // Send sums to response
      return webApp.serveGoldbachSums(httpResponse, sums);
    }
    /// Number requested too big (2^63 or greater)
  } catch (const std::out_of_range& oor) {
      return webApp.serveNotFound(httpResponse);
  }
  /// Unrecognized request
  return webApp.serveNotFound(httpResponse);
}
void WebServer::stopWorkers() {
  // used to generate a connection error in answer server
  this->answerServer.stopListening();
    TcpClient dummyClient;
  Socket dummySocket;
  dummyClient.connect("0.0.0.0", "8082");
  dummySocket.close();
  this->answerServer.waitToFinish();
}
