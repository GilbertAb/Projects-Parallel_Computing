#ifndef ANSWER_HANDLER
#define ANSWER_HANDLER

#include "AnswerServer.hpp"
#include "Consumer.hpp"
#include "Socket.hpp"

class AnswerHandler : public Consumer<Socket> {
  protected:
   /// Server that have the method to consume the client socket
   AnswerServer* server;
  public:
   /// Constructor
   /// @param answerServer Server that has the intruction on how to consume the
   /// client connection
   AnswerHandler(AnswerServer* answerServer);
   /// Method that will keep the handler consuming fron the socket queue
   /// @return The tipe of exit
   int run();
   /// Method that calls the instructions on how to process s socket
   /// @param socket Client connection socket
   void consume(const Socket& socket);
};

#endif  //ANSWER_HANDLER
