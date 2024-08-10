/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:34 by jatan             #+#    #+#             */
/*   Updated: 2024/08/10 11:32:49 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include <string>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>	// struct addrinfo, gai_strerror
#include <unistd.h> // execve, dup, dup2, pipe, fork

#include "Logger.hpp"
#include "ServerBlock.hpp"
#include "Utility.hpp"

#include "macro.h"

#define BUFFER_SIZE 8192

class Connection
{

public:
	int fd;

	// Connection();
	Connection(int fd, ServerBlock *serverBlock);
	Connection(Connection const &src);
	Connection &operator=(Connection const &rhs);
	~Connection();

	bool readData(void);
	bool sendData(void);
	bool hasResponse(void);
	bool shouldKeepAlive(void);
	bool isConnectionClosed(void);

private:
	// fd_set* _master;
	// std::string _requestString;
	// std::string _request;
	// std::string _response;
	ServerBlock *_serverBlock;
	Request *_request;
	Response *_response;
	Logger _logger;
	std::string _buffer;
	ssize_t _lastReadStatus;
	static int _connectionCount;
	// int _index;
};

std::ostream &operator<<(std::ostream &o, Connection const &i);

#endif /* ****************************************************** CONNECTION_H */