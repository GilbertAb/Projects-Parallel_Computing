// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0
// Concurrent goldbach sums calculator server for distributed use

#ifdef GOLDBACHSERVER

#include <signal.h>
#include "GoldbachServer.hpp"

/// Tells the program how to handle the signal for program termination
void handleSignal(int signal) {
  /* adapted from https://stackoverflow.com/questions/48378213/how-to
   -deal-with-errno-and-signal-handler-in-linux */
  int temp_errno = errno;
  GoldbachServer::getInstance().stopProcessing();
  GoldbachServer::getInstance().stopListening();
  errno = temp_errno;
}

int main(int argc, char* argv[]) {
  /// Tells the program what signals to handle
  signal(SIGINT, handleSignal);
  signal(SIGTERM, handleSignal);

  return GoldbachServer::getInstance().start(argc, argv);
}

#endif  // GOLDBACHSERVER