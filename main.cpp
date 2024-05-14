#include "Webserver.hpp"
#include "ConfigParser.hpp"

int main(void)
{
    // WebServer webserver(PORT, "localhost", "Webserver");
    
    // webserver.start();
    ConfigParser configParser;

    configParser.parseConfig("default.conf");
    configParser.displayConfig();
    
}