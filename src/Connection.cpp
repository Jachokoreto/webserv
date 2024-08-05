#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

Connection::Connection(int fd, ServerBlock *serverBlock) : fd(fd), _serverBlock(serverBlock), _logger(Logger("Connection"))
{
	// this->_index = this->_connectionCount++;
	// std::stringstream ss;
	// ss << "created " << this->_index;;
	// this->_logger.log(ss.str());
	// std::cout << this->_requestString << std::endl;
	std::cout << "\n\n======NEW REQUEST========" << std::endl;
	this->_logger.log("Connection created at fd " + utl::toString(fd));
	_request = NULL;
	_response = NULL;
	_buffer.clear();
}

// Connection::Connection(): _logger(Logger("Connection")), _serverBlock()
// {
// }

Connection::Connection(const Connection &src) : _logger(Logger("Connection"))
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
	char buf[BUFFER_SIZE+1]; // buffer for client data
	memset(buf, 0, BUFFER_SIZE +1);
	ssize_t bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
	size_t needle;

	if (bytes_read == -1)
	{
		perror("read");
		return false;
	}
	else if (bytes_read == 0)
	{
		this->_logger.log("CLOSE CONNECTION?");
		return false; // Connection closed by client
	}
	_buffer += std::string(buf);

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
		needle = _buffer.find("\r\n\r\n");
		if (needle != std::string::npos)
		{
			try
			{
				_request = new Request(_buffer.substr(0, needle + 4));
				_response = new Response();
				
				int res = _request->checkIfHandleWithoutBody(); 
				if (res == 1)
				{
					this->_logger.info("handle without body");
					_serverBlock->router.routeRequest(*_request, *_response);
				}
				else if (res == -1)
				{
					_response->errorResponse(404, "Invalid body");
				}
				else if (res == 0)
				{
					_buffer = _buffer.substr(needle + 4);
					if (_request->processBody(_buffer))
					{
						this->_logger.info("handle with body");
						_serverBlock->router.routeRequest(*_request, *_response);
					}
				}
			}
			catch (const std::exception &e)
			{
				_logger.error(std::string(e.what()));
				_buffer.clear();
				return false;
			}
				_buffer.clear();
		}
	}


	return true;
}

bool Connection::sendData(void)
{
	const std::string resString = _response->toString();
	if (resString.empty())
	{
		return false;
	}
	// std::cout << "content length: " << _response->getHeader("Content-Length") << std::endl;
	ssize_t bytes_sent = send(fd, resString.c_str(), resString.length(), 0);
	if (bytes_sent == -1)
	{
		perror("send");
		return false;
	}
	if ((unsigned long)bytes_sent < resString.length())
	{
		this->_logger.log("didnt send finish, sent: " + utl::toString(bytes_sent));
		_response->truncateResponse(bytes_sent);
		return false;
	}
	if (!this->_response->getIsDone())
	{
		return false;
	}
	_logger.log("Finished sending response");
	return true;
}

bool Connection::hasResponse()
{
	return _response != NULL && !_response->toString().empty();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
