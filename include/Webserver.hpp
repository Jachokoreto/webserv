/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:46:03 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 04:50:20 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// struct addrinfo, gai_strerror
#include <unistd.h> // execve, dup, dup2, pipe, fork
#include <fcntl.h> // fcntl
#include <sys/select.h> // select

#include "Request.hpp"
#include "Response.hpp"
#include "StaticFileHandler.hpp"
#include "APIHandler.hpp"
#include "Router.hpp"

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

// #define PORT "8080"
// #define BACKLOG 10
// #define BUFFER_SIZE 1024

class Webserver
{
public:
	// Webserver(std::string port, std::string hostname, std::string server);
	Webserver(std::vector<ServerBlock*>& serverBlocks);
	~Webserver();

	void start();
	// void stop(); // no need to stop

private:
	int createSocket(std::string port, std::string hostname);
	void waitForRequest(void);
	void handleRequest(int sock);
	// void acceptConnection(fd_set&, int& fdmax);

	void setupServerSocket(ServerBlock& serverBlock);
	void setNonBlocking(int sock_fd);
	void configureSelect(void);
	void handleConnections(void);
	void acceptNewConnection(int server_socket, ServerBlock *serverBlock);
	bool echoMessage(int client_socket);

	int _listener; // socket fd
	// std::vector<ServerBlock*>& _serverBlocks;
	Logger _logger;
	std::map<int, ServerBlock*> _serverSockets;
	// std::map<int, ServerBlock&> _client_sockets;
	std::vector<Connection*> _connections;
	fd_set _readFds, _writeFds;
	int _maxFd;
	// std::map<int, Connection> _connections;

	Webserver();
};

#endif