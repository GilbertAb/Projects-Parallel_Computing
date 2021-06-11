// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "GoldbachWebApp.hpp"

GoldbachWebApp::GoldbachWebApp() {}
GoldbachWebApp::~GoldbachWebApp() {}

bool GoldbachWebApp::serve(HttpResponse& httpResponse, int serve
  , const std::vector<int64_t>& numbers) {

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  switch (serve) {
  case SUMS:
    serveGoldbachSums(httpResponse, numbers);
    break;

  case HOME_PAGE:
    serveHomepage(httpResponse);
    break;

  case NOT_FOUND:
    httpResponse.setStatusCode(404);
    serveNotFound(httpResponse);
    break;
  }
  // Send the response to the client (user agent)
  return httpResponse.send();
}

void GoldbachWebApp::serveGoldbachSums(HttpResponse& httpResponse, std::vector<int64_t> numbers) {
  std::vector<std::vector<std::string>> sums = calculator.getGoldbachSums(numbers);
  std::string title = "Goldbach sums results";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n";
  for(size_t index = 0; index < numbers.size(); ++index) {
    httpResponse.body() << "  <h2";
    if(numbers[index] > -6 && numbers[index] < 6) {
      httpResponse.body() <<  " class=\"err\"";
    }
    httpResponse.body() << ">" << std::to_string(numbers[index]) << "</h2>\n"
      << "  <p>" << sums[index][0] <<"</p>\n";
    if(sums[index].size() > 1) {
      httpResponse.body() << "  <ol>\n";
      for(size_t sum_index; sum_index < sums[index].size(); ++sum_index) {
        httpResponse.body() << "    <li>" <<sums[index][sum_index] << "</li>\n";
      }
      httpResponse.body() << "  </ol>\n";
    }
  }
    httpResponse.body() << "  <hr><p><a href=\"/\">Back</a></p>\n"
    << "</html>\n";
}

void GoldbachWebApp::serveHomepage(HttpResponse& httpResponse) {
  // Build the body of the response
  std::string title = "Goldbach sums";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <form method=\"get\" action=\"/goldbach\">\n"
    << "    <label for=\"number\">Number</label>\n"
    << "    <input type=\"number\" name=\"number\" required/>\n"
    << "    <button type=\"submit\">Calculate</button>\n"
    << "  </form>\n"
    << "</html>\n";
}

void GoldbachWebApp::serveNotFound(HttpResponse& httpResponse) {
  std::string title = "Not found";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resouce was not found in this server.</p>\n"
    << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
    << "</html>\n";
}