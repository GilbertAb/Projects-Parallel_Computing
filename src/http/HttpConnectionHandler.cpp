// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "HttpConnectionHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "Socket.hpp"

HttpConnectionHandler::HttpConnectionHandler(HttpServer* httpServer)
  : Consumer(nullptr, Socket(), false){
  this->httpServer = httpServer;
}

int HttpConnectionHandler::run() {
  // Start the forever loop to consume all the messages that arrive
  this->consumeForever();
  printf("stop\n");

  // If the forever loop finished, no more messages will arrive
  // Print statistics
  /*Log::append(Log::INFO, "Consumer", std::to_string(this->receivedMessages)
    + " menssages consumed");*/  
  
  return EXIT_SUCCESS;
}

void HttpConnectionHandler::consume(const Socket& client) {
  Socket cpyClient = client;
  while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest httpRequest(cpyClient);

    // If the request is not valid or an error happened
    if (!httpRequest.parse()) {
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }
    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse httpResponse(cpyClient);
    //match http version used in client request
    httpResponse.setHttpVersion(httpRequest.getHttpVersion());

    // Give subclass a chance to respond the HTTP request
    
    const bool handled = httpServer->handleHttpRequest(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      cpyClient.close();
      break;
    }

  }
  
  // If the forever loop finished, no more messages will arrive
  // Print statistics
  /*Log::append(Log::INFO, "Consumer", std::to_string(this->receivedMessages)
    + " menssages consumed");*/
}
