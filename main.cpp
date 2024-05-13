#include "Webserver.hpp"

int main(void)
{
    WebServer webserver(PORT, "localhost", "Webserver");
    
    webserver.start();
}