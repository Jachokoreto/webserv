#ifndef SERVER_BLOCK_HPP
# define SERVER_BLOCK_HPP

# include <iostream>
# include <string>
# include <sstream>

# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>	// struct addrinfo, gai_strerror
# include <unistd.h> // execve, dup, dup2, pipe, fork

# include "Logger.hpp"
#include "Location.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

class ServerBlock {
public:
    int listen;
    string root;
    string index;
    vector<Location> locations;

    void display() const;
    int getPort() const;
    string getServerName() const;
};

#endif /* ****************************************************** SERVER_BLOCK_HPP */