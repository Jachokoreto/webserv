#include "Webserver.hpp"
#include "ConfigParser.hpp"
#include "AutoindexHandler.hpp"

void testRouter(void)
{
	Router router("/home/sirhcofe/Core/webserv");
	router.addRoute("/jaclyn", new RouteDetails(false, "index.html", "/public/jaclyn", 1));

	Request req("GET /jaclyn/./haha/popo/.. HTTP/1.1\r\nHOST: localhost:8080");
	Response res;
	router.routeRequest(req, res);
}

void testAutoindex(void)
{
    Request req("GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
    Response res;
    AutoindexHandler autoindexHandler;
    RouteDetails routeDetails;
    routeDetails.root = "/Users/user/sidess/webserv/public";
    autoindexHandler.handleRequest(req, res, routeDetails);
    std::cout << res.toString() << std::endl;
}

void testWebserver(void)
{
    // WebServer webserver(PORT, "localhost", "Webserver");
    
    // webserver.start();
    ConfigParser configParser;
    std::vector<RequestHandler*> requestHandlers;
    requestHandlers.push_back(new AutoindexHandler());
    requestHandlers.push_back(new StaticFileHandler());
    std::vector<ServerBlock*> serverBlocks;

    

    configParser.createServerBlocksFromConf("conf/default.conf", requestHandlers,  serverBlocks);
    for (std::vector<ServerBlock*>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
    {
        configParser.displayConfig(**it);
    }


	Webserver webserver(serverBlocks);
	webserver.start();
}
int main(void)
{
	testRouter();
	// testAutoindex();
	// testWebserver();
}
