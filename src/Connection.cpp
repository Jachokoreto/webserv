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
	char buf[BUFFER_SIZE + 1]; // buffer for client data
	memset(buf, 0, BUFFER_SIZE + 1);
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

// bool Connection::sendData()
// {
// 	std::cout << "SENDING DATA NOW\n"
// 			  << std::endl;

// 	this->_response->addHeader("Transfer-Encoding", "chunked");
// 	std::cout << (this->_response->getHeader("Transfer-Encoding")) << std::endl;
// 	usleep(5000);
// 	std::string data = this->_response->toString();
// 	size_t offset = 0;
// 	size_t chunkSize = 4096; // You can adjust the chunk size.

// 	std::cout << "piupiu\n"
// 			  << data << std::endl;

// 	while (offset < data.size())
// 	{
// 		size_t sizeToSend = std::min(chunkSize, data.size() - offset);
// 		// std::cout << sizeToSend << std::endl;
// 		std::stringstream chunkHeader;
// 		chunkHeader << std::hex << sizeToSend << "\r\n";

// 		std::string chunk = chunkHeader.str() + data.substr(offset, sizeToSend) + "\r\n";

// 		// std::cout << "send loop: " << data.size() - offset << std::endl;
// 		ssize_t bytesSent = send(fd, chunk.c_str(), chunk.length(), 0);

// 		// std::cout << "!!" << std::endl;
// 		if (bytesSent == -1)
// 		{
// 			perror("send");
// 			return false; // Stop sending on error.
// 		}

// 		offset += sizeToSend;
// 		// std::cout << "offset: " << offset << std::endl;
// 		usleep(5000);
// 	}

// 	std::cout << "out here" << std::endl;
// 	std::string endChunk = "0\r\n\r\n"; // Send the ending empty chunk to complete the transfer.
// 	send(fd, endChunk.c_str(), endChunk.length(), 0);

// 	std::cout << "send last" << std::endl;

// 	_logger.log("Sent chunked data");
// 	return true;
// }

bool Connection::hasResponse()
{
	return _response != NULL && _response->getReady();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
