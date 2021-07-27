// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHSERVER_H
#define GOLDBACHSERVER_H

#include <vector>
#include "TcpServer.hpp"
#include "TcpClient.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "GoldbachConnectionHandler.hpp"
#include "AssemblerCalculator.hpp"
#include "SumsDispatcher.hpp"

#define DEFAULT_PORT "8080"

class GoldbachConnectionHandler;

class GoldbachServer : public TcpServer {
  DISABLE_COPY(GoldbachServer);

 protected:
  /// Lookup criteria for searching network information about this host
  struct addrinfo hints;
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  const char* response_port;
  const char* response_server;
  /// Queue with the connection sockets
  Queue<Socket> socketQueue;
  /// Producer that puts the sockets in the queue
  /// HttpRequestHandler* handler;
  std::vector<GoldbachConnectionHandler*> consumers;
  /// Vector with the calculators that will work concurrently
  std::vector<AssemblerCalculator*> calculators;
  /// Consuming queue for the calculators
  Queue<GoldbachNumber> numberQueue;
  /// Queues where the answer for the requested sums are for each thread
  std::vector<Queue<GoldbachSums>*> sumQueues;
  /// Dispatcher incharge of delivering the answers to the respective queue
  /// depending on the thread
  SumsDispatcher* dispatcher;
  /// Concurrent connections
  size_t consumerCount = 10;

 public:
  /// Constructor
  GoldbachServer();
  /// Destructor
  ~GoldbachServer();
  /// Infinetelly listen for client connection requests and accept all of them.
  /// For each accepted connection request, the virtual onConnectionAccepted()
  /// will be called. Inherited classes must override that method
  // void listenForever(const char* port);

 public:
  /// Get access to the unique instance of this Singleton class
  static GoldbachServer& getInstance();
  /// Start the simulation
  /// @param Recives the arguments and the number of them written by the user
  int start(int argc, char* argv[]);
  /// Sends the stop signal to the Dispatcher and the consuming queues of the
  /// calculators to make them stop
  void stopProcessing();

  void handleSumsRequest(std::string& sumsRequested, size_t thread_number);

 protected:
  /// This method is called each time a client connection request is accepted.
  /// @param Recives the socket for the client
  void handleClientConnection(Socket& client) override;

 protected:
  /// Makes the number of calculators equal to the amount of processors in the
  /// machine, tells them from what queue to consume and produce, and finally
  /// makes each calculator thread start
  void startCalculators();
  /// Asigns the queues that have the answer so that the dispatcher knows the
  /// queue for each connection thread
  void registerQueues();

  bool analyzeArguments(int argc, char* argv[]);
  /// Sends the stop condition, set the consumimg queue and starts the thread
  void startConsumers();
  /// Stops the consumers
  void stopConsumers();
  /// Stops the dispatcher
  void stopDispatcher();
  /// send answers to response server
  void sendResults(size_t& client_thread_num, size_t& num_count, size_t& thread_number);
};

#endif  // GOLDBACHSERVER_H
