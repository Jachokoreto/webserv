#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

Connection::Connection(int fd, ServerBlock *serverBlock) : fd(fd), _request(NULL), _response(NULL), _serverBlock(serverBlock), _logger(Logger("Connection"))
{
	this->_logger.log("Connection created at fd " + utl::toString(fd));
	_buffer.clear();
}

Connection::Connection(const Connection &src) : _logger(Logger("Connection"))
{
	_serverBlock = src._serverBlock;
	_request = src._request;
	_response = src._response;
	_buffer = src._buffer;
	fd = src.fd;
}

Connection::~Connection()
{
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
		_logger = rhs._logger;
		_serverBlock = rhs._serverBlock;
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
	char buf[BUFFER_SIZE + 1]; // buffer for client data
	memset(buf, 0, BUFFER_SIZE + 1);
	ssize_t bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
	// size_t needle;

	if (bytes_read <= 0)
	{
		_logger.log(bytes_read == 0 ? "Connection closed by client" : "Read error");
		return false;
	}

	_buffer += std::string(buf, bytes_read);

	if (_request != NULL)
	{
		int res = _request->processBody(_buffer);
		if (res == 1)
		{
			this->_logger.info("handle with body");
			_serverBlock->router.routeRequest(*_request, *_response);
		}
		if (res != -1)
			_buffer.clear();
	}
	else
	{
		if ((_buffer.find("\r\n\r\n") != std::string::npos))
		{
			size_t headerEnd = _buffer.find("\r\n\r\n") + 4;
			_request = new Request(_buffer.substr(0, headerEnd));
			_response = new Response();
			_buffer.erase(0, headerEnd);
		}
		// needle = _buffer.find("\r\n\r\n");
		// if (needle != std::string::npos)
		// {
		// 	try
		// 	{
		// 		std::cout << _buffer.substr(0, needle + 4) << std::endl;
		// 		_request = new Request(_buffer.substr(0, needle + 4));
		// 		_response = new Response();

		// 		if ((_request->getHeader("Host") != _serverBlock->getHostname()) && (_request->getHeader("Host") != "localhost"))
		// 		{
		// 			_response->errorResponse(404, "Hostname not recognized");
		// 			return true;
		// 		}
		// 		int res = _request->checkIfHandleWithoutBody();
		// 		if (res == 1)
		// 		{
		// 			this->_logger.info("handle without body");
		// 			_serverBlock->router.routeRequest(*_request, *_response);
		// 		}
		// 		else if (res == -1)
		// 		{
		// 			_response->errorResponse(404, "Invalid body");
		// 		}
		// 		else if (res == 0)
		// 		{
		// 			_buffer = _buffer.substr(needle + 4);
		// 			if (_request->processBody(_buffer))
		// 			{
		// 				this->_logger.info("handle with body");
		// 				_serverBlock->router.routeRequest(*_request, *_response);
		// 			}
		// 		}
		// 	}
		// 	catch (const std::exception &e)
		// 	{
		// 		_logger.error(std::string(e.what()));
		// 		_buffer.clear();
		// 		return false;
		// 	}
		// 	_buffer.clear();
		// }
	}
	return true;
}

bool Connection::sendData(void)
{
	const std::string resString = _response->toString();
	if (resString.empty())
	{
		// _logger.log("Response not ready yet");
		return false;
	}
	// char * cstr = (char *)resString.c_str();
	// cstr[resString.length()] = '\0';
	ssize_t bytes_sent = send(fd, resString.c_str(), resString.length(), 0);
	if (bytes_sent == -1)
	{
		perror("send");
		return false;
	}
	if ((unsigned long)bytes_sent < resString.length())
	{
		this->_logger.log("didnt send finish");
		_response->truncateResponse(bytes_sent);
		return false;
	}
	_logger.log("sent data");
	return true;
}

bool Connection::hasResponse()
{
	return _response != NULL && _response->getReady();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
