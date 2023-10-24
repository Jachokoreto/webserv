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

#include "Logger.hpp"

#define PORT "8080"
#define BACKLOG 10

/**
 * This class represents the web server itself. It would typically contain
 * the main event loop that listens for incoming connections and dispatches
 * requests to the appropriate handler.
 */
class WebServer
{
public:
	WebServer();
	~WebServer();

	void start();
	// void stop();

private:
	int createSocket();
	void waitForRequest(void);
	void handleRequest(int);

	int _socket;
};

#endif