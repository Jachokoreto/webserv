#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

Connection::Connection(int fd, ServerBlock *serverBlock):  fd(fd), _serverBlock(serverBlock), _logger(Logger("Connection"))
{
	// this->_index = this->_connectionCount++;
	// std::stringstream ss;
	// ss << "created " << this->_index;;
	// this->_logger.log(ss.str());
	// std::cout << this->_requestString << std::endl;
	_logger.log("Connection created at fd " + utl::toString(fd));
	_request = NULL;
	_response = NULL;
	_buffer = "";
}

// Connection::Connection(): _logger(Logger("Connection")), _serverBlock()
// {
// }

Connection::Connection(const Connection &src): _logger(Logger("Connection"))
{
	_serverBlock = src._serverBlock;
	_request = src._request;
	_response = src._response;
	_buffer = src._buffer;
	// this->_index = this->_connectionCount++;
	fd = src.fd;
}

Connection::~Connection()
{
	// std::stringstream ss;
	// ss << "deleted  " << this->_index;
	// this->_logger.error(ss.str());
	delete _request;
	delete _response;
	this->_logger.log("Connection closed at fd " + utl::toString(fd));
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Connection &Connection::operator=(const Connection &rhs)
{
	if (this != &rhs)
	{
		// Since _serverBlock is a reference, it cannot be reassigned
		// Logger can be reinitialized or reconfigured
		_logger = rhs._logger;
		_serverBlock = rhs._serverBlock;
		// Ensure other member variables like _request and _response are copied correctly
		_request = rhs._request;
		_response = rhs._response;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &o, Connection const &i)
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
	this->_logger.log("Read data");
	char buf[BUFFER_SIZE]; // buffer for client data
	memset(buf, 0, BUFFER_SIZE);
	ssize_t bytes_read = read(fd, buf, BUFFER_SIZE - 1);

	if (bytes_read == -1)
	{
		perror("read");
		return false;
	}
	else if (bytes_read == 0)
	{
		return false; // Connection closed by client
	}
	buf[bytes_read] = '\0';
	_logger.info(std::string(buf));
	_buffer += std::string(buf);
	if (_buffer.find("\r\n\r\n") == std::string::npos)
	{
		_logger.log("Request not complete yet");
		return true;
	}
	try
	{
		_request = new Request(_buffer);
		_response = new Response();
		_serverBlock->router.routeRequest(*_request, *_response);
	}
	catch (const std::exception &e)
	{
		_logger.error(e.what());
		return false;
	}
	return true;
}

bool Connection::sendData(void)
{
	const std::string resString = _response->toString();
	if (resString.empty())
	{
		_logger.log("Repond not ready yet");
		return false;
	}
	ssize_t bytes_sent = send(fd, resString.c_str(), resString.length(), 0);
	if (bytes_sent == -1)
	{
		perror("send");
		return false;
	}
	if ((unsigned long)bytes_sent < resString.length())
	{
		_response->truncateResponse(bytes_sent);
	}
	_logger.log("sent data");
	return true;
}

bool Connection::hasResponse()
{
	return _response != NULL;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
