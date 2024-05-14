#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>



// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <netdb.h>	// struct addrinfo, gai_strerror
// # include <unistd.h> // execve, dup, dup2, pipe, fork

# include "Logger.hpp"
# include "Server.hpp"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::vector;

class ConfigParser {
public:
    void parseConfig(const std::string& filename);
    void displayConfig() const;

private:
    Server server;
    void parseServerConfig(std::stringstream& ss, std::string& key, Server& server);
    void parseLocationConfig(std::stringstream& ss, std::string& key, Location& loc);
    static void trim(std::string& str);
};

#endif /* ****************************************************** CONFIG_PARSER_HPP */