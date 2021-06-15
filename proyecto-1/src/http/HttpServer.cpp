// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>
#include <iostream>

#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Socket.hpp"

HttpServer::HttpServer() {}
HttpServer::~HttpServer() {}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::handleClientConnection(Socket& client) {
  socketQueue.push(client);
}

void HttpServer::startConsumers() {
  Socket stopCondition;
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new HttpConnectionHandler(this, stopCondition);
    assert(this->consumers[index]);
    this->consumers[index]->setConsumingQueue(&this->socketQueue);
    this->consumers[index]->startThread();
  }
}

void HttpServer::stopConsumers() {
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    Socket socket;
    this->socketQueue.push(socket);
  }
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }
}
