// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "WebServer.hpp"
#include <signal.h>
#include <iostream>

void handleFin(int signal){
  WebServer::getInstance()->handleSignal();
  std::cout << "Se cierra el programa\n";
  exit(signal);
}

int main(int argc, char* argv[]) {
  struct sigaction handle;
  handle.sa_handler = handleFin;
  sigaction(SIGINT, &handle, NULL);
  sigaction(SIGTERM, &handle, NULL);
  WebServer* webServer = WebServer::getInstance();
  return webServer->start(argc, argv);
}

#endif  // WEBSERVER
