#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

Connection::Connection(int fd, ServerBlock* serverBlock): fd(fd), _serverBlock(serverBlock), _logger(Logger("Connection"))
{
	// this->_index = this->_connectionCount++;
	// std::stringstream ss;
	// ss << "created " << this->_index;;
	// this->_logger.log(ss.str());
	// std::cout << this->_requestString << std::endl;
	this->_logger.log("Connection created at fd" + std::to_string(fd));
}

// Connection::Connection(): _logger(Logger("Connection")), _serverBlock()
// {
// }

Connection::Connection( const Connection & src ):  _logger(Logger("Connection"))
{
	_serverBlock = src._serverBlock;
	_request = src._request;
	_response = src._response;
	// this->_index = this->_connectionCount++;
	fd = src.fd;
}

Connection::~Connection()
{
	// std::stringstream ss;
	// ss << "deleted  " << this->_index;
	// this->_logger.error(ss.str());
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Connection& Connection::operator=(const Connection & rhs) {
    if (this != &rhs) {
        // Since _serverBlock is a reference, it cannot be reassigned
        // Logger can be reinitialized or reconfigured
        _logger = Logger("Connection");
		_serverBlock = rhs._serverBlock;
        // Ensure other member variables like _request and _response are copied correctly
        _request = rhs._request;
        _response = rhs._response;
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

bool Connection::readData()
{
	char buf[BUFFER_SIZE]; // buffer for client data
	memset(buf, 0, BUFFER_SIZE);
	ssize_t bytes_read = read(fd, buf, BUFFER_SIZE - 1);

	if (bytes_read == -1) {
		perror("read");
		return false;
	} else if (bytes_read == 0) {
		return false; // Connection closed by client
	}
	buf[bytes_read] = '\0';
	_request += buf;
	std::cout << RED << buf << std::endl;
	std::cout << BLUE << _request << RESET << std::endl;
	if (_request.find("\r\n\r\n") != std::string::npos) {
		_response = _serverBlock->handleRequest(_request);
		return true;
	}
	return true;
}

bool Connection::sendData()
{
	if (_response.empty()) {
		return false;
	}
	ssize_t bytes_sent = send(fd, _response.c_str(), _response.length(), 0);
	if (bytes_sent == -1) {
		perror("send");
		return false;
	}
	_response.clear();
	_request.clear();
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
