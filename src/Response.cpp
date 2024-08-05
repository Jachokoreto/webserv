/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:12:06 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/06 02:01:11 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::statusMap;

Response::Response() : _logger(Logger("Response")), _ready(0)
{
	_statusCode = 200;
	statusMap[200] = "OK";
	statusMap[403] = "Forbidden";
	statusMap[404] = "Not Found";
	statusMap[405] = "Method not allowed";
	statusMap[500] = "Internal Server Error";
	_body = "";
	_isDone = false;
	// std::cout << "Response constructor" << std::endl;
}

Response::~Response()
{
	// std::cout << "Response destructor" << std::endl;
}

std::string Response::toString()
{
	if (this->_ready != 1)
		return "";
	std::stringstream responseStream;
	size_t bodyLen = this->_body.length();

	if (!_responseString.empty())
		return _responseString;

	if (!_isHeaderSent)
	{
		// Status line
		responseStream << "HTTP/1.1 " << this->_statusCode << " " << Response::statusMap.at(this->_statusCode) << "\r\n";

		if (bodyLen > 0)
		{
			if (bodyLen <= BODY_SIZE)
			{
				this->addHeader("Content-Length", utl::toString(this->getBody().length()));
			}
			else
			{
				this->addHeader("Transfer-Encoding", "chunked");
			}
		}
		// header
		for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		{
			responseStream << it->first << ": " << it->second << "\r\n";
		}
		responseStream << "\r\n";
		_isHeaderSent = true;
	}

	bool isChunked = this->getHeader("Transfer-Encoding") == "chunked";
	// optional body
	if (bodyLen > 0)
	{
		if (isChunked)
		{
			int chunk_size = bodyLen <= BODY_SIZE ? bodyLen : BODY_SIZE;
			std::ostringstream ss;
			ss << std::hex << chunk_size;
			responseStream << ss.str() << "\r\n"
						   << this->_body.substr(0, chunk_size) << "\r\n";
			this->_body = this->_body.substr(chunk_size);
			std::cout << responseStream.str() << std::endl;
		}
		else
		{
			responseStream << this->_body;
			this->_isDone = true;
		}
	}
	else if (bodyLen == 0 && isChunked && !this->_isDone)
	{
		responseStream << "0\r\n\r\n";
		this->_isDone = true;
	}
	else if (bodyLen == 0 && !isChunked)
	{
		this->_isDone = true;
	}

	this->_responseString = responseStream.str();
	return this->_responseString;
}

void Response::errorResponse(int statusCode, std::string message)
{
	(void)message;
	setStatusCode(statusCode);
	// _logger.error(message);
}

void Response::truncateResponse(unsigned long length)
{
	if (this->_responseString.length() > length)
		this->_responseString = this->_responseString.substr(length);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void Response::setStatusCode(int status)
{
	this->_ready = 1;
	this->_statusCode = status;
}

int Response::getReady(void)
{
	return this->_ready;
}

void Response::setReady(int status)
{
	this->_ready = status;
}

int Response::getStatusCode() const
{
	return this->_statusCode;
}

bool Response::getIsDone() const
{
	return this->_isDone;
}
