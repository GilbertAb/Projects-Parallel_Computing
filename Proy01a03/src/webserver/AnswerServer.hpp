#ifndef ANSWER_SERVER
#define ANSWER_SERVER

#include <string>

#include "AnswerHandler.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Thread.hpp"

#define DEFAULT_PORT "2525"

class AnswerServer : public TcpServer, public Thread {
  DISABLE_COPY(AnswerServer);
  
  protected:
   /// Port that will be listened to
   const char* port = DEFAULT_PORT;
   /// The consumer that will process the requests
   AnswerHandler* connectionHandler;
   /// Queue with the socket connection
   Queue<Socket>* socketQueue;
   /// Queue where the answers are going to be stored
   Queue<std::string>* answerQueue;
   /// Saves the string from the socket in the queue
   void handleClientConnection(Socket& client);
   /// Constructor
   AnswerServer();
   /// Destructor
   ~AnswerServer();
  public:
   /// Gets the only instance of this class created
   /// @return A reference to the only instance of the server
   static AnswerServer& getInstance();
   /// Puts the client socket into the queue
   /// Makes the server start receiving requests from the default port
   int run();
   /// Gives a reference to the queue where the answer are being stored so they
   /// can be delivered to the thread that requested them
   /// @return A reference to the answer queue
   Queue<std::string>* getQueue();
   /// Reads what is on the socket to save it in the answer queue
   /// This method is incharged of telling the connection handler how to handle
   /// a socket
   void getSocketInfo(Socket& client);
};

#endif  //ANSWER_SERVER
