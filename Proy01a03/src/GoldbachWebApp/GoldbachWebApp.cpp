// Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0

#include "GoldbachWebApp.hpp"

GoldbachWebApp::GoldbachWebApp() {}
GoldbachWebApp::~GoldbachWebApp() {}

bool GoldbachWebApp::serveGoldbachSums(HttpResponse& httpResponse
  , const std::vector<std::vector<std::string>>& sums) {
  setHeader(httpResponse);
  std::string title = "Goldbach sums results";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n";
  for (size_t index = 0; index < sums.size(); ++index) {
    httpResponse.body() << "  <h2";
    if (sums[index][0].rfind("NA") != std::string::npos) {
      httpResponse.body() <<  " class=\"err\"";
    }
    httpResponse.body() << ">" << sums[index][0].substr(0,
      sums[index][0].rfind(":")) << "</h2>\n" << "  <p>" << sums[index][0]
      <<"</p>\n";
    if (sums[index].size() > 1) {
      httpResponse.body() << "  <ol>\n";
      for (size_t sum_index = 1; sum_index < sums[index].size(); ++sum_index) {
        httpResponse.body() << "    <li>" <<sums[index][sum_index] << "</li>\n";
      }
      httpResponse.body() << "  </ol>\n";
    }
  }
    httpResponse.body() << "  <hr><p><a href=\"/\">Back</a></p>\n"
    << "</html>\n";

  return httpResponse.send();
}

bool GoldbachWebApp::serveHomepage(HttpResponse& httpResponse) {
  setHeader(httpResponse);
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
    << "    <input type=\"text\" name=\"numbers\" required/>\n"
    << "    <button type=\"submit\">Calculate</button>\n"
    << "  </form>\n"
    << "</html>\n";

  return httpResponse.send();
}

bool GoldbachWebApp::serveNotFound(HttpResponse& httpResponse) {
  setHeader(httpResponse);
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

  return httpResponse.send();
}

void GoldbachWebApp::setHeader(HttpResponse& httpResponse) {
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
}
