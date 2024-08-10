#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

Connection::Connection(int fd, ServerBlock *serverBlock) : fd(fd), _serverBlock(serverBlock), _request(NULL), _response(NULL), _logger(Logger("Connection")), _lastReadStatus(1)
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

bool Connection::shouldKeepAlive()
{
	if (!_request)
		return false;
	if (_request->getHeader("Connection") == "keep-alive")
		return true;
	else
		return false;
}

bool Connection::isConnectionClosed()
{
	if (_lastReadStatus == 0)
	{
		// recv returned 0, indicating the client closed the connection
		_logger.log("Client closed the connection");
		return true;
	}
	else if (_lastReadStatus == -1)
	{
		// recv returned -1, check errno to determine the reason
		switch (errno)
		{
		case ECONNRESET:
		case ETIMEDOUT:
		case EHOSTUNREACH:
		case ENETDOWN:
		case ENETUNREACH:
		case ENETRESET:
			_logger.log("Connection error due to network failure or client reset");
			return true;
		default:
			return false; // Keep the connection open in case of recoverable errors
		}
	}
	return false; // Default to not closed if no issues
}

bool Connection::readData()
{
	bool dataProcessed = false; // Flag to indicate if data was processed

	while (true)
	{
		char buf[BUFFER_SIZE + 1] = {0};
		ssize_t bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
		_lastReadStatus = bytes_read;

		if (bytes_read < 0)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
			{
				break; // No more data to read
			}
			_logger.log("Read error");
			return false; // Error in reading data
		}
		else if (bytes_read == 0)
		{
			_logger.log("Connection closed by client");
			return false; // Connection closed
		}

		_buffer.append(buf, bytes_read);

		// Process complete headers
		if (!_request)
		{
			if ((_buffer.find("\r\n\r\n") != std::string::npos))
			{
				size_t headerEnd = _buffer.find("\r\n\r\n") + 4;
				_request = new Request(_buffer.substr(0, headerEnd));
				_response = new Response();
				_buffer.erase(0, headerEnd);
			}
		}

		// Handle request with expected body or without
		if (_request)
		{
			int res = this->_request->processBody(_buffer);
			if (res > 0)
			{
				_buffer.erase(0, res);
				_serverBlock->router.routeRequest(*_request, *_response);
				dataProcessed = true;
				break;
			}
			else if (res == 0)
			{
				_serverBlock->router.routeRequest(*_request, *_response);
				dataProcessed = true; // Data has been processed for GET-like requests
				if (_buffer.empty())
					break; // No more data to process
			}
			else
			{
				_buffer.erase(0, res);
			}
			// if (_request->getHeader("Transfer-Encoding") == "chunked")
			// {
			// 	if (!handleChunkedTransfer()) {
			// 		return false;
			// 	}

			// }
			// else
			// {
			// 	size_t expectedBodySize = atoi(_request->getHeader("Content-Length").c_str());
			// 	if (expectedBodySize > 0 && _buffer.length() >= expectedBodySize)
			// 	{
			// 		_request->setBody(_buffer.substr(0, expectedBodySize));
			// 		_buffer.erase(0, expectedBodySize);
			// 		_serverBlock->router.routeRequest(*_request, *_response);
			// 		dataProcessed = true; // Data has been processed
			// 		break;				  // Exit after processing
			// 	}
			// 	else if (expectedBodySize == 0)
			// 	{
			// 		_serverBlock->router.routeRequest(*_request, *_response);
			// 		dataProcessed = true; // Data has been processed for GET-like requests
			// 		if (_buffer.empty())
			// 			break; // No more data to process
			// 	}
			// }
		}
		// needle = _buffer.find("\r\n\r\n");
		// if (needle != std::string::npos)
		// {
		// 	try
		// 	{
		// 		std::cout << _buffer.substr(0, needle + 4) << std::endl;
		// 		_request = new Request(_buffer.substr(0, needle + 4));
		// 		_response = new Response();
	}
	return dataProcessed; // Return whether any data was processed
}

// bool Connection::readData()
// {
// 	char buf[BUFFER_SIZE + 1]; // buffer for client data
// 	memset(buf, 0, BUFFER_SIZE + 1);
// 	ssize_t bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
// 	// size_t needle;

// 	if (bytes_read <= 0)
// 	{
// 		_logger.log(bytes_read == 0 ? "Connection closed by client" : "Read error");
// 		return false;
// 	}

// 	_buffer += std::string(buf);

// 	if (_request != NULL)
// 	{
// 		int res = _request->processBody(_buffer);
// 		if (res == 1)
// 		{
// 			this->_logger.info("handle with body");
// 			_serverBlock->router.routeRequest(*_request, *_response);
// 		}
// 		if (res != -1)
// 			_buffer.clear();
// 	}
// 	else
// 	{
// 		size_t headersEnd = _buffer.find("\r\n\r\n");
// 		if (headersEnd != std::string::npos)
// 		{
// 			try
// 			{
// 				_request = new Request(_buffer.substr(0, headersEnd + 4));
// 				_response = new Response();
// 				_buffer.erase(0, headersEnd + 4);
// 			}
// 			catch (const std::exception &e)
// 			{
// 				_logger.error(std::string(e.what()));
// 				return false;
// 			}
// 		}
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
// 		std::cout << "res: " << res << std::endl;
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
// 	}

// 	return true;
// }

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
