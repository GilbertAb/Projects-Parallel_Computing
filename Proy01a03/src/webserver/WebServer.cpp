// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "WebServer.hpp"
#include "TcpClient.hpp"

const char* const usage =
  "Usage: webserv [port] [max_connections]\n"
  "\n"
  "  port             Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  max_connections  Maximum number of allowed client connections\n";

WebServer& WebServer::getInstance() {
  static WebServer webServer;
  return webServer;
}

WebServer::WebServer() {

}
WebServer::~WebServer() {
  // Liberar memoria de sums assemblers
}

int WebServer::start(int argc, char* argv[]) {
  try {
    if (this->analyzeArguments(argc, argv)) {
      this->startConsumers();
      sumQueues.resize(consumerCount);
      for (size_t index = 0; index < consumerCount; ++index) {
        sumQueues[index] = new Queue<GoldbachSums>();
      }

      answerServer = new AnswerServer(&sumQueues, "8082");
      answerServer ->startThread();
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      std::cout << "web server listening on " << address.getIP()
        << " port " << address.getPort() << "...\n";
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    /// Clean exit if error detected
    stopConsumers();
  }
  //dispatcher->waitToFinish();

  return EXIT_SUCCESS;
}

bool WebServer::analyzeArguments(int argc, char* argv[]) {
  /// Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos || argc > 3) {
      std::cout << usage;
      return false;
    }
  }
  if (argc >= 2) {
    this->port = argv[1];
  }
  if (argc == 3) {
    try {
      this->consumerCount = std::stoll(argv[2]);
    } catch (const std::exception& error) {
      std::cerr << "error: invalid consumer count\n";
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
      Socket socket = client.connect("0.0.0.0", "8081");
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
  this->answerServer->stopListening();
  this->answerServer->stopConsumers();
  delete answerServer;
  this->stopConsumers();
}
