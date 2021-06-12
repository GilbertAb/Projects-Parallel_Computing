// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef CONSUMERCLIENT_HPP
#define CONSUMERCLIENT_HPP

#include "Consumer.hpp"
#include "Socket.hpp"

class ConsumerClient : public Consumer<Socket> {
  DISABLE_COPY(ConsumerClient);

  protected:

  public:
    // Constructor
    explicit ConsumerTest(int consumerDelay);
    // Consume the messages in its own execution thread
    int run() override;
    // Override this method to process any data extracted from the queue
    void consume(const Socket data) override;
};

#endif  // CONSUMERCLIENT_HPP
