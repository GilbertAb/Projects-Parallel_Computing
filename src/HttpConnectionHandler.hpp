// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPCONNECTIONHANDLER_HPP
#define HTTPCONNECTIONHANDLER_HPP

#include "Consumer.hpp"
#include "Socket.hpp"

class HttpConnectionHandler : public Consumer<Socket> {
  DISABLE_COPY(HttpConnectionHandler);
    
  public:
    // Constructor
    explicit HttpConnectionHandler();
    // Consume the messages in its own execution thread
    int run() override;
    // Override this method to process any data extracted from the queue
    void consume(const Socket client) override;
};

#endif  // HTTPCONNECTIONHANDLER_HPP
