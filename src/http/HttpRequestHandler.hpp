#ifndef HTTPREQUESTHANDLER_HPP
#define HTTPREQUESTHANDLER_HPP

#include "Producer.hpp"
#include "Socket.hpp"

class HttpRequestHandler : public Producer<Socket> {
  public:
   ///Constructor
   HttpRequestHandler(Queue<Socket>* producingQueue);
   int run();
};

#endif
