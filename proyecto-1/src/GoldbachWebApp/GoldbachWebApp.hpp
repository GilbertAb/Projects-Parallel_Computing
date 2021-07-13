// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include <string>
#include <vector>
#include "HttpServer.hpp"
#include "GoldbachCalculator.hpp"

// WARNING: This class should never have attributes because race condition
// problems will appear.

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
  * @brief Builds the response header.
  * @param httpResponse The response to be built and sent to the client
  */
  void setHeader(HttpResponse& httpResponse);

 public:
  /** Default Constructor.*/
  GoldbachWebApp();
  /** Default Destructor.*/
  ~GoldbachWebApp();

  /**
  * @brief Builds a respond page with the goldbach sums asked by user. 
  * First gets the goldbach sums from the model class instance and then parses them
  * into the http response.
  * @param httpResponse The response to be built and sent to the client
  * @param numbers Dynamic array containing the numbers whose goldbach sums were asked by user
  */
  bool serveGoldbachSums(HttpResponse& httpResponse
    , const std::vector<std::vector<std::string>>& sums);

    /**
  * @brief Builds a respond page with the homepage of the aplication. 
  * Builds the homepage of the web application into http response body. The homepage
  * is shown by default when no numbers are requested.
  * @param httpResponse The response to be built and sent to the client
  */
  bool serveHomepage(HttpResponse& httpResponse);

  /**
  * @brief Builds a respond page with the not found page. 
  * If the user makes an invalid request to the server, this page will
  * be built into the response body. This page is shown when the status code
  * is 404 to inform the user that an invalid request was made.
  * @param httpResponse The response to be built and sent to the client
  */
  bool serveNotFound(HttpResponse& httpResponse);
};
#endif  // GOLDBACHWEBAPP_HPP
