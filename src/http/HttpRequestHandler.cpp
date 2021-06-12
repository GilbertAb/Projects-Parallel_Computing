#include "HttpRequestHandler.hpp"
#include "Socket.hpp"

HttpRequestHandler::HttpRequestHandler(Queue<Socket>* producingQueue)
: Producer<Socket>(producingQueue){
}

int HttpRequestHandler::run(){
  return 0;
}
