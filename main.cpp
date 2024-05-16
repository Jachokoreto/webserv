#include "Webserver.hpp"
#include <iostream>

int main(void)
{
	// WebServer webserver;

	// webserver.start();

	Router router("/home/sirhcofe/Core/webserv/public");
	StaticFileHandler *staticHandler = new StaticFileHandler;
	// APIHandler apiHandler;

	router.addRoute("/static-files", staticHandler);
	// router.addRoute("/static-files/*", cgiHandler);
	// router.addRoute("/static-files", cgiHandler);
	// router.addRoute("/cgi")

	std::string requestStr = "GET /static-files/floor.png HTTP/1.1";

	Request incomingRequest(requestStr);
	Response response;

	router.routeRequest(incomingRequest, response);
	// staticHandler->handleRequest(incomingRequest, response);
	std::string responseStr = response.toString();

	std::cout << responseStr << std::endl;
	delete staticHandler;
	return (0);
}
