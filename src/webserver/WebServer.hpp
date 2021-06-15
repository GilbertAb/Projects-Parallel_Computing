// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "HttpServer.hpp"
#include "GoldbachWebApp.hpp"

#define DEFAULT_PORT "8080"

class WebServer : public HttpServer {
  DISABLE_COPY(WebServer);

 private:
  /// Singleton instance
  static WebServer* instance;    
  /// Constructor
  WebServer();
  /// Destructor
  ~WebServer();
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  /// Web app that handles the calculation of goldbach
  GoldbachWebApp webApp;

 public:
  /// Get access to the unique instance of this Singleton class
  static WebServer* getInstance();
  
  /// Start the simulation
  int start(int argc, char* argv[]);

 protected:
  /// Analyze the command line arguments
  /// @return true if program can continue execution, false otherwise
  bool analyzeArguments(int argc, char* argv[]);
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) override;

 protected:
  /// Route, that provide an answer according to the URI value
  /// For example, home page is handled different than a number
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // WEBSERVER_H
