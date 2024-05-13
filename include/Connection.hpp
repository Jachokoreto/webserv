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


class Connection
{

	public:

		Connection();
		Connection(int fd, fd_set *master);
		Connection( Connection const & src );
		Connection & operator=( Connection const & rhs );
		~Connection();

		void readData(char *requestString);
		static int _connectionCount;


	private:
		int _fd;
		fd_set* _master;
		std::string _requestString;
		Logger _logger;
		int _index;



};

std::ostream &			operator<<( std::ostream & o, Connection const & i );

#endif /* ****************************************************** CONNECTION_H */