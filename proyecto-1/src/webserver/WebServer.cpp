// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "WebServer.hpp"

const char* const usage =
  "Usage: webserv [port] [max_connections]\n"
  "\n"
  "  port             Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  max_connections  Maximum number of allowed client connections\n";

/// Singleton instance
WebServer* WebServer::instance = nullptr;

WebServer* WebServer::getInstance() {
  if (instance == NULL) {
    instance = new WebServer();
  }
  return instance;
}

WebServer::WebServer() {}
WebServer::~WebServer() {}

int WebServer::start(int argc, char* argv[]) {
  try {
    if (this->analyzeArguments(argc, argv)) {
      this->startConsumers();
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
    HttpResponse& httpResponse) {
  /// Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  std::cout << "connection established with client " << address.getIP()
    << " port " << address.getPort() << std::endl;

  /// Print HTTP request
  std::cout << "  " << httpRequest.getMethod()
    << ' ' << httpRequest.getURI()
    << ' ' << httpRequest.getHttpVersion() << std::endl;

  return this->route(httpRequest, httpResponse);
}

bool WebServer::route(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  /// If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return webApp.serve(httpResponse, HOME_PAGE);
  }
  std::smatch matches;

  /// If a number was asked in the form "/goldbach/1223"
  /// or "/goldbach?number=1223"
  try {
    std::regex inQuery("^/goldbach(/|\\?numbers=)(-?\\d+(%2C-?\\d*)*)$");
    if (std::regex_search(httpRequest.getURI(), matches, inQuery)) {
      assert(matches.length() >= 3);
      std::vector<int64_t> numbers;
      std::string numberstr = matches.str(2);
      for (size_t i = 0; !numberstr.empty(); ++i) {
        numbers.push_back(std::stoll(numberstr));
        while ((numberstr[0] <= '9' && numberstr[0] >= '0')
          || numberstr[0] == '-') {
          numberstr = numberstr.substr(1, numberstr.size() -1);
        }
        if (numberstr.size() > 3) {
          numberstr = numberstr.substr(3, numberstr.size() -3);
        } else {
          numberstr.clear();
        }
      }
      return webApp.serve(httpResponse, SUMS, numbers);
    }
    /// Number requested too big (2^63 or greater)
  } catch (const std::out_of_range& oor) {
      return webApp.serve(httpResponse, NOT_FOUND);
  }
  /// Unrecognized request
  return webApp.serve(httpResponse, NOT_FOUND);
}