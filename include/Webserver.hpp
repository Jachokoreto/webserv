#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// struct addrinfo, gai_strerror
#include <unistd.h> // execve, dup, dup2, pipe, fork
#include <fcntl.h> // fcntl
#include <sys/select.h> // select

#include <iostream>
#include <cstring> // strerror
#include <cerrno>  // errno
#include <sstream> // stringstream
#include <string>
#include <stdexcept>
#include <map>


#include "Logger.hpp"
#include "Connection.hpp"
#include "ServerBlock.hpp"

#define PORT "8080"
#define BACKLOG 10
#define BUFFER_SIZE 1024

class WebServer
{
public:
	// WebServer(std::string port, std::string hostname, std::string server);
	WebServer(std::vector<ServerBlock> serverBlocks);
	~WebServer();

	void start();
	// void stop(); // no need to stop

private:
	int createSocket(std::string port, std::string hostname);
	void waitForRequest(void);
	void handleRequest(char *, int, fd_set&);
	// void acceptConnection(fd_set&, int& fdmax);

	void setupServerSocket(int port);
	void setNonBlocking(int sock_fd);
	void configureSelect(void);
	void handleConnections(void);
	void acceptNewConnection(int server_socket);
	bool echoMessage(int client_socket);

	int _listener; // socket fd
	std::vector<ServerBlock> _serverBlocks;
	Logger _logger;
	std::map<int, int> _server_sockets;
	std::vector<int> _client_sockets;
	fd_set _read_fds, _write_fds;
	int _max_fd;
	// std::map<int, Connection> _connections;

	WebServer();
};

#endif