/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:34 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 04:51:32 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <iostream>
# include <string>
# include <sstream>

# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>	// struct addrinfo, gai_strerror
# include <unistd.h> // execve, dup, dup2, pipe, fork

# include "Logger.hpp"
# include "ServerBlock.hpp"

#define BUFFER_SIZE 30000

class Connection
{

	public:
		int fd;
		
		// Connection();
		Connection(int fd, ServerBlock* serverBlock);
		Connection( Connection const & src );
		Connection & operator=( Connection const & rhs );
		~Connection();

		bool readData(void);
		bool sendData(void);


	private:
		// fd_set* _master;
		// std::string _requestString;
		// std::string _request;
		// std::string _response;
		Request* _request;
		Response* _response;
		ServerBlock* _serverBlock;
		Logger _logger;
		static int _connectionCount;
		// int _index;



};

std::ostream &			operator<<( std::ostream & o, Connection const & i );

#endif /* ****************************************************** CONNECTION_H */