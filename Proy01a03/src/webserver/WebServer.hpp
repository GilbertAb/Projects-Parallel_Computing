// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <vector>
#include "AssemblerCalculator.hpp"
#include "GoldbachWebApp.hpp"
#include "HttpServer.hpp"
#include "Queue.hpp"
#include "SumsDispatcher.hpp"

#define DEFAULT_PORT "8080"

class WebServer : public HttpServer {
  DISABLE_COPY(WebServer);

 private:
  /// Constructor
  WebServer();
  /// Destructor
  ~WebServer();
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  /// Web app that handles the calculation of goldbach
  GoldbachWebApp webApp;
  /// Vector with the calculators that will work concurrently
  std::vector<AssemblerCalculator*> calculators;
  /// Consuming queue for the calculators
  Queue<GoldbachNumber> numberQueue;
  /// Queues where the answer for the requested sums are for each thread
  std::vector<Queue<GoldbachSums>*> sumQueues;
  /// Dispatcher incharge of delivering the answers to the respective queue
  /// depending on the thread
  SumsDispatcher* dispatcher;

 public:
  /// Get access to the unique instance of this Singleton class
  static WebServer& getInstance();
  /// Start the simulation
  /// @param Recives the arguments and the number of them written by the user
  int start(int argc, char* argv[]);
  /// Sends the stop signal to the Dispatcher and the consuming queues of the
  /// calculators to make them stop
  void stopProcessing();

 protected:
  /// Analyze the command line arguments
  /// @param Recives the arguments and the number of them written by the user
  /// @return true if program can continue execution, false otherwise
  bool analyzeArguments(int argc, char* argv[]);
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @param Gets the users request and the response that will be modified by
  /// the program.
  /// @return true on success and the server will continue handling requests
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse, size_t threadNumber) override;

 protected:
  /// Route, that provide an answer according to the URI value
  /// For example, home page is handled different than a number
  /// @param Gets the users request and the response that will be modified by
  /// the program.
  /// @return true on success and the server will continue handling requests
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse,
    size_t threadNumber);

 protected:
  /// Makes the number of calculators equal to the amount of processors in the
  /// machine, tells them from what queue to consume and produce, and finally
  /// makes each calculator thread start
  void startCalculators();
  /// Asigns the queues that have the answer so that the dispatcher knows the
  /// queue for each connection thread
  void registerQueues();
};

#endif  // WEBSERVER_H
