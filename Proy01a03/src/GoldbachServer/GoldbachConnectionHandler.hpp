// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDGoldbachCONNECTIONHANDLER_HPP
#define GOLDGoldbachCONNECTIONHANDLER_HPP

#include "Consumer.hpp"
#include "Socket.hpp"
#include "GoldbachServer.hpp"
#include "AssemblerCalculator.hpp"

class GoldbachServer;

class GoldbachConnectionHandler : public Consumer<Socket> {
  DISABLE_COPY(GoldbachConnectionHandler);

 protected:
  GoldbachServer* goldbachServer;
  size_t threadNumber;

 public:
  /** Constructor. Initializes httpServer.
  */
  explicit GoldbachConnectionHandler(GoldbachServer* goldbachServer, Socket stopCondition,
    size_t threadNumber);

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

  bool validateRequest(std::string sumsRequested);
};

#endif  // GOLDGoldbachCONNECTIONHANDLER_HPP
