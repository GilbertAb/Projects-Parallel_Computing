// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "WebServer.hpp"
#include <signal.h>

void handleFin(int signal){
  WebServer::getInstance()->stopListening();
}

int main(int argc, char* argv[]) {
  signal(SIGINT, handleFin);
  signal(SIGTERM, handleFin);
  WebServer* webServer = WebServer::getInstance();
  return webServer->start(argc, argv);
}

#endif  // WEBSERVER
