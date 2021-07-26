#ifndef ANSWER_SERVER
#define ANSWER_SERVER

#include <string>

#include "AnswerHandler.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Thread.hpp"

#define DEFAULT_PORT2 "8082"

class AnswerHandler;

class AnswerServer : public TcpServer, public Thread {
  DISABLE_COPY(AnswerServer);
  
  protected:
   /// Lookup criteria for searching network information about this host
   struct addrinfo hints;
   /// Port that will be listened to
   const char* port = DEFAULT_PORT2;

   size_t consumerCount = 10;
   /// The consumer that will process the requests
   std::vector<AnswerHandler*> consumers;
   /// Queue with the socket connection
   Queue<Socket> socketQueue;
   /// Queue where the answers are going to be stored
   std::vector<Queue<GoldbachSums>*>* answerQueues;
   /// Saves the string from the socket in the queue
   void handleClientConnection(Socket& client);
   /// Sends the stop condition, set the consumimg queue and starts the thread
   void startConsumers();
  public:
   /// Gets the only instance of this class created
   /// @return A reference to the only instance of the server
  //  static AnswerServer& getInstance();
   /// Puts the client socket into the queue
   /// Makes the server start receiving requests from the default port
   int run();
   /// Reads what is on the socket to save it in the answer queue
   /// This method is incharged of telling the connection handler how to handle
   /// a socket
   void getSocketInfo(Socket& client);
   /// Stops the consumers
   void stopConsumers();

  /// Constructor
   AnswerServer(std::vector<Queue<GoldbachSums>*>* answerQueues, const char* port);
   /// Destructor
   ~AnswerServer();
};

#endif  //ANSWER_SERVER
