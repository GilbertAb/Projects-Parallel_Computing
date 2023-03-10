// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>
#include "TcpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "HttpConnectionHandler.hpp"

class HttpConnectionHandler;

class HttpServer : public TcpServer {
  DISABLE_COPY(HttpServer);

 protected:
  /// Lookup criteria for searching network information about this host
  struct addrinfo hints;
  /// Queue with the connection sockets
  Queue<Socket> socketQueue;
  /// Producer that puts the sockets in the queue
  /// HttpRequestHandler* handler;
  std::vector<HttpConnectionHandler*> consumers;
  /// Concurrent connections
  size_t consumerCount = 10;
  /// Sends the stop condition, set the consumimg queue and starts the thread
  void startConsumers();
  /// Stops the consumers
  void stopConsumers();

 public:
  /// Constructor
  HttpServer();
  /// Destructor
  ~HttpServer();
  /// Infinetelly listen for client connection requests and accept all of them.
  /// For each accepted connection request, the virtual onConnectionAccepted()
  /// will be called. Inherited classes must override that method
  void listenForever(const char* port);
    /// Called each time an HTTP request is received. Web server should analyze
  /// the request object and assemble a response with the response object.
  /// Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
  virtual bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse, size_t threadNumber) = 0;

 protected:
  /// This method is called each time a client connection request is accepted.
  /// @param Recives the socket for the client
  void handleClientConnection(Socket& client) override;
};

#endif  // HTTPSERVER_H
