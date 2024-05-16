#include "Webserver.hpp"
#include "ConfigParser.hpp"
#include "AutoindexHandler.hpp"

void testAutoindex(void)
{
    Request req("GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
    Response res;
    AutoindexHandler autoindexHandler;
    autoindexHandler.handleRequest(req, res, "/Users/user/sidess/webserv/public");
    std::cout << res.toString() << std::endl;
}

void testWebserver(void)
{
    // WebServer webserver(PORT, "localhost", "Webserver");
    
    // webserver.start();
    ConfigParser configParser;
    std::vector<ServerBlock*> serverBlocks;

    configParser.parseConfig("default.conf", serverBlocks);
    for (std::vector<ServerBlock*>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
    {
        configParser.displayConfig(**it);
    }

    Webserver webserver(serverBlocks);
    webserver.start();
}
int main(void)
{
    testAutoindex();
    // testWebserver();
    
} 
