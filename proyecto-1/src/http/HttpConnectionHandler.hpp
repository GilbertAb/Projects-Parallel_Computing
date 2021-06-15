// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPCONNECTIONHANDLER_HPP
#define HTTPCONNECTIONHANDLER_HPP

#include "Consumer.hpp"
#include "Socket.hpp"
#include "HttpServer.hpp"

class HttpServer;

class HttpConnectionHandler : public Consumer<Socket> {
  DISABLE_COPY(HttpConnectionHandler);

 protected:
  HttpServer* httpServer;

 public:
  /** Constructor. Initializes httpServer.
  */
  explicit HttpConnectionHandler(HttpServer* httpServer, Socket stopCondition);

  /**
   * @brief Start the forever loop to consume all the connections that arrive.
   * @details Start the forever loop to consume all the sockets that arrive.
   * If the forever loop finished, no more client will arrive 
  */
  int run() override;

  /**
   * @brief Handles a received HTTP client request and creates an object 
   * for the server responds to that client's request.
   * @details Creates an object that parses the HTTP request from the 
   * socket, responds to the client request if it's valid.
   * @param client The socket.
  */
  void consume(const Socket& client) override;
};

#endif  // HTTPCONNECTIONHANDLER_HPP
