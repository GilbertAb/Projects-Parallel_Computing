// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include <signal.h>
#include "WebServer.hpp"

/// Tells the program how to handle the signal for program termination
void handleSignal(int signal) {
  /* adapted from https://stackoverflow.com/questions/48378213/how-to
   -deal-with-errno-and-signal-handler-in-linux */
  int temp_errno = errno;
  //WebServer::getInstance()->stopProcessing();
  //WebServer::getInstance()->stopListening();
  WebServer::getInstance().stopProcessing();
  WebServer::getInstance().stopListening();
  errno = temp_errno;
}

int main(int argc, char* argv[]) {
  /// Tells the program what signals to handle
  signal(SIGINT, handleSignal);
  signal(SIGTERM, handleSignal);
  //WebServer* webServer = WebServer::getInstance();
  //int error = webServer->start(argc, argv);
  //webServer->ResetInstance();
  //WebServer webServer = WebServer::getInstance();
  int error = WebServer::getInstance().start(argc, argv);
  return error;
}

#endif  // WEBSERVER
