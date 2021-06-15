// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "WebServer.hpp"
#include <signal.h>

/// Tells the program how to handle the signal for program termination
void handleSignal(int signal){
  // adapted from https://stackoverflow.com/questions/48378213/how-to-deal-with-errno-and-signal-handler-in-linux
  int temp_errno = errno;
  WebServer::getInstance()->stopListening();
  errno = temp_errno;
}

int main(int argc, char* argv[]) {
  /// Tells the program what signals to handle
  signal(SIGINT, handleSignal);
  signal(SIGTERM, handleSignal);
  WebServer* webServer = WebServer::getInstance();
  return webServer->start(argc, argv);
}

#endif  // WEBSERVER
