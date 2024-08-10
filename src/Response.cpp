/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:12:06 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 17:42:56 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::statusMap;

Response::Response() : _logger(Logger("Response")), _ready(0)
{
	_statusCode = 200;
	statusMap[200] = "OK";
	statusMap[202] = "Accepted";
	statusMap[307] = "Temporary Redirect";
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
			// this->addHeader("Content-Length", utl::toString(bodyLen));

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
		// if (isChunked)
		// {
		// 	int chunk_size = bodyLen <= BODY_SIZE ? bodyLen : BODY_SIZE;
		// 	std::ostringstream ss;
		// 	ss << std::hex << chunk_size;
		// 	responseStream << ss.str() << "\r\n"
		// 				   << this->_body.substr(0, chunk_size) << "\r\n";
		// 	this->_body = this->_body.substr(chunk_size);
		// 	std::cout << responseStream.str() << std::endl;
		// }
		// else
		// {
		// 	responseStream << this->_body;
		// 	this->_isDone = true;
		// }
		if (isChunked)
		{
			size_t pos = 0; // Position in the body
			while (pos < bodyLen)
			{
				size_t remaining = bodyLen - pos;
				size_t chunkSize = remaining < BODY_SIZE ? remaining : BODY_SIZE; // Manually calculate the minimum
				responseStream << std::hex << chunkSize << "\r\n";
				responseStream << _body.substr(pos, chunkSize) << "\r\n";
				pos += chunkSize;
			}
			responseStream << "0\r\n\r\n"; // End of chunks
			_isDone = true;				   // Mark the response as complete
		}
		else
		{
			responseStream << _body;
			_isDone = true;
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

std::string readFile(const std::string &filePath)
{
	std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
	if (!fileStream)
	{
		std::cerr << "Cannot open file: " << filePath << std::endl;
		return "";
	}
	std::ostringstream content;
	content << fileStream.rdbuf();
	return content.str();
}

void Response::errorResponse(int statusCode, const std::string &errorMessage)
{
	this->_logger.error(errorMessage);
	std::string errorFilePath = "public/custom_" + std::to_string(statusCode) + ".html";
	std::string errorContent = readFile(errorFilePath);

	if (errorContent.empty())
	{
		// If the error file is not found or is empty, use a default message
		errorContent = "<html><body><h1>" + std::to_string(statusCode) + " " + errorMessage + "</h1></body></html>";
	}

	setStatusCode(statusCode);
	setBody(errorContent);
	addHeader("Content-Type", "text/html");
	_isDone = true; // Indicate that the response is ready to be sent
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
