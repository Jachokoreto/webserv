#include "Webserver.hpp"
#include <iostream>

int main(void)
{
	// WebServer webserver;

	// webserver.start();

	Router router;
	// StaticFileHandler *staticHandler = new StaticFileHandler;
	// APIHandler apiHandler;

	router.addRoute("/jaclyn", false, "boomba.html", "/public/usr/jaclyn-dir", GET & POST);
	// router.addRoute("/static-files/*", cgiHandler);
	// router.addRoute("/static-files", cgiHandler);
	// router.addRoute("/cgi")

	std::string requestStr = "GET /jaclyn/floor.png HTTP/1.1";

	Request incomingRequest(requestStr);
	Response response;

	router.routeRequest(incomingRequest, response);
	// staticHandler->handleRequest(incomingRequest, response);
	std::string responseStr = response.toString();

	std::cout << responseStr << std::endl;
	// delete staticHandler;
	return (0);
}
