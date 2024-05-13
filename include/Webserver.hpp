#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// struct addrinfo, gai_strerror
#include <unistd.h> // execve, dup, dup2, pipe, fork

#include <iostream>
#include <cstring> // strerror
#include <cerrno>  // errno
#include <sstream> // stringstream
#include <string>
#include <stdexcept>
#include <map>


#include "Logger.hpp"
#include "Connection.hpp"

#define PORT "8080"
#define BACKLOG 10

class WebServer
{
public:
	WebServer(std::string port, std::string hostname, std::string server);
	~WebServer();

	void start();
	// void stop(); // no need to stop

private:
	int createSocket(std::string port, std::string hostname);
	void waitForRequest(void);
	void handleRequest(char *, int, fd_set&);
	void acceptConnection(fd_set&, int& fdmax);

	int _listener; // socket fd
	Logger _logger;
	std::map<int, Connection> _connections;

	WebServer();
};

#endif