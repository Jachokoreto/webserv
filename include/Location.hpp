#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <sstream>

# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>	// struct addrinfo, gai_strerror
# include <unistd.h> // execve, dup, dup2, pipe, fork

# include "Logger.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

class Location {
public:
    string path;
    string root;
    string index;
    string cgi_pass;
    vector<string> allowed_methods;
    bool autoindex;

    Location() : autoindex(false) {} // Constructor to initialize default values

    void display() const;
};

#endif /* ****************************************************** LOCATION_HPP */