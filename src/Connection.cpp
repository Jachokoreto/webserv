#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Connection::Connection(int fd, fd_set *master): _fd(fd), _master(master), _requestString(""), _logger(Logger("Connection"))
{
	// this->_index = this->_connectionCount++;
	// std::stringstream ss;
	// ss << "created " << this->_index;;
	// this->_logger.log(ss.str());
	// std::cout << this->_requestString << std::endl;
}

Connection::Connection(): _logger(Logger("Connection"))
{
}


Connection::Connection( const Connection & src ):  _fd(src._fd), _master(src._master), _requestString(src._requestString), _logger(Logger("Connection"))
{
	// this->_index = this->_connectionCount++;
	// std::stringstream ss;
	// ss << "copy constructor " << this->_index;
	// this->_logger.log(ss.str());
	// std::cout << _fd << std::endl;
	// std::cout << _requestString << std::endl;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Connection::~Connection()
{
	// std::stringstream ss;
	// ss << "deleted  " << this->_index;
	// this->_logger.error(ss.str());
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Connection &	Connection::operator=( Connection const & rhs )
{
	(void)rhs;
	if ( this != &rhs )
	{
		this->_fd = rhs._fd;
		this->_master = rhs._master;
		this->_requestString = rhs._requestString;
		this->_logger = rhs._logger;
		this->_index = this->_connectionCount++;


		// std::stringstream ss;
		// ss << "copy assignment " << this->_connectionCount;
		// this->_logger.log(ss.str());
	}
	return *this;
}

std::ostream &	operator<<( std::ostream & o, Connection const & i )
{
	(void)i;
	// o << "i am still here?";

	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void Connection::readData(char *requestString)
{
	_requestString += std::string(requestString);

	std::string copy = _requestString;
	if (copy.find("\r\n\r\n") != std::string::npos) {
		std::cout << "Request complete" << std::endl;
		// if (FD_ISSET(_fd, _master)) {
			std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
			std::cout << _fd << _index << std::endl;
			send(_fd, response.c_str(), response.length(), 0);
			this->_logger.log("Response sent");
		// }			
		return ;
	}
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */