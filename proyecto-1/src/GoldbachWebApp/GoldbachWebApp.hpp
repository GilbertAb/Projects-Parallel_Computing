// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include <string>
#include <vector>
#include "HttpServer.hpp"
#include "GoldbachCalculator.hpp"


#define SUMS 0        /** Goldbach sums requested by user*/
#define HOME_PAGE -1  /** Homepage requested by user*/
#define NOT_FOUND -2  /** Invalid request from user*/
// TODO(any): warn class cant have attributes

/**
 * @brief Is the only part of the program that the user can directly
 * interact with and gives the right interface response according to user interaction.
 * 
 * Class corresponding to the View in the MVC design pattern. Communicates directly with the Model
 * class through the calculator field. Is the only part of the program that the user can directly
 * interact with and gives the right interface response according to user interaction.
 * Validates user input and sends it to model class if processing is needed.
 */
class GoldbachWebApp {
 private:
  /**
  * @brief Builds a respond page with the goldbach sums asked by user. 
  * First gets the goldbach sums from the model class instance and then parses them
  * into the http response.
  * @param httpResponse The response to be built and sent to the client
  * @param numbers Dynamic array containing the numbers whose goldbach sums were asked by user
  */
  void serveGoldbachSums(HttpResponse& httpResponse
    , const std::vector<std::vector<std::string>>& sums);

    /**
  * @brief Builds a respond page with the homepage of the aplication. 
  * Builds the homepage of the web application into http response body. The homepage
  * is shown by default when no numbers are requested.
  * @param httpResponse The response to be built and sent to the client
  */
  void serveHomepage(HttpResponse& httpResponse);

  /**
  * @brief Builds a respond page with the not found page. 
  * If the user makes an invalid request to the server, this page will
  * be built into the response body. This page is shown when the status code
  * is 404 to inform the user that an invalid request was made.
  * @param httpResponse The response to be built and sent to the client
  */
  void serveNotFound(HttpResponse& httpResponse);

 public:
  /** Default Constructor.*/
  GoldbachWebApp();
  /** Default Destructor.*/
  ~GoldbachWebApp();

    /**
  * @brief Builds the response header and calls the according method to build the body 
  * The header of the response is set and then the body is built according to the request
  * made by the user. After the body is built by the according serve method, the response
  * is sent to the client.
  * @param httpResponse The response to be built and sent to the client
  * @param serve The body build method to be called. Use the according Macro
  * @param numbers Array with the numbers whose goldbach sums where asked by user.
  * It is empty if no numbers were requested.
  */
  bool serve(HttpResponse& httpResponse, int serve
    , const std::vector<std::vector<std::string>>& sums
    = std::vector<std::vector<std::string>>());
};
#endif  // GOLDBACHWEBAPP_HPP
