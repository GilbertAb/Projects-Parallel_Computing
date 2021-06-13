// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "WebServer.hpp"

int main(int argc, char* argv[]) {
  WebServer* webServer = WebServer::getInstance();
  return webServer->start(argc, argv);
}

#endif  // WEBSERVER
