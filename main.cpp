#include "Webserver.hpp"
#include "ConfigParser.hpp"
#include "AutoindexHandler.hpp"
#include "CGIHandler.hpp"

void testRouter(void)
{
	Router router("/home/sirhcofe/Core/webserv");
	router.addRoute("/jaclyn", new RouteDetails(false, "index.html", "/public/jaclyn", GET));
	router.addRoute("/post", new RouteDetails(false, "index.html", "/public/post", GET | POST));
	std::vector<RequestHandler *> requestHandlers;
	requestHandlers.push_back(new CGIHandler());
	requestHandlers.push_back(new AutoindexHandler());
	requestHandlers.push_back(new StaticFileHandler());
	router.assignHandlers(requestHandlers);

	Request req("GET /jaclyn/./haha/popo/.. HTTP/1.1\r\nHOST: localhost:8080");
	Response res;
	router.routeRequest(req, res);

	Request req2("POST /post/hello.txt HTTP/1.1\r\nHOST: localhost:8080");
	Response res2;
	router.routeRequest(req2, res2);
}

void testAutoindex(void)
{
	Request req("GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response res;
	AutoindexHandler autoindexHandler;
	RouteDetails routeDetails;
	routeDetails.root = "/Users/user/sidess/webserv/public";
	autoindexHandler.handleRequest(req, res, routeDetails, "/Users/user/sidess/webserv/public");
	std::cout << res.toString() << std::endl;
}

void testCGI(void)
{
	Request req("POST /directory/youpi.bla HTTP/1.1\r\n\r\n");
	Response res;
	CGIHandler cgiHandler;
	RouteDetails routeDetails;
	routeDetails.allowedMethods = POST;
	routeDetails.cgiPass = "CGI-tester";
	std::cout << "testCGI" << std::endl;
	cgiHandler.handleRequest(req, res, routeDetails, "/Users/user/sidess/webserv/YoupiBanane/youpi.bla");
	std::cout << res.toString() << std::endl;
}

void testWebserver(void)
{
	// WebServer webserver(PORT, "localhost", "Webserver");

	// webserver.start();
	ConfigParser configParser;
	std::vector<RequestHandler *> requestHandlers;
	requestHandlers.push_back(new CGIHandler());
	requestHandlers.push_back(new AutoindexHandler());
	requestHandlers.push_back(new StaticFileHandler());
	std::vector<ServerBlock *> serverBlocks;

	configParser.createServerBlocksFromConf("conf/tester.conf", requestHandlers, serverBlocks);
	for (std::vector<ServerBlock *>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
	{
		configParser.displayConfig(**it);
	}

	Webserver webserver(serverBlocks);
	webserver.start();
}

int main(void)
{
	// testRouter();
	// testAutoindex();
	// testCGI();
	testWebserver();
}
