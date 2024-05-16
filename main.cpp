#include "Webserver.hpp"
#include "ConfigParser.hpp"

int main(void)
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
