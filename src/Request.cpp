/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:19:09 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/15 22:47:28 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

const std::vector<std::string> Request::methodVector = {"GET", "POST", "PUT", "PATCH", "DELETE"};

const std::string Request::getUri() const
{
	return this->_uri;
}

const std::string Request::getMethod() const
{
	return this->_method;
}

void Request::setUri(const std::string &uri)
{
	this->_uri = uri;
}

void Request::setMethod(const std::string &method)
{
	this->_method = method;
}

Request::Request(const std::string &requestString)
{
	std::vector<std::string> split = utility::splitStringByDelim(requestString, '\n');
	std::vector<std::string> requestLine = utility::splitStringByDelim(split[0], ' ');
	const std::string &method = requestLine[0];
	if (std::find(Request::methodVector.begin(), Request::methodVector.end(), method) == Request::methodVector.end())
		throw Request::NotAllowedException("Invalid method");
	this->setMethod(method);
	this->setUri(requestLine[1]);
	this->_version = "HTTP/1.1";

	std::vector<std::string>::iterator iterator = std::find(split.begin() + 1, split.end(), "");
	std::vector<std::string> header(split.begin() + 1, iterator);
	for (const std::string &entry : header)
	{
		std::size_t delimPos = entry.find(": ");
		if (delimPos != std::string::npos && delimPos != 0)
		{
			std::string key = entry.substr(0, delimPos);
			std::string value = entry.substr(delimPos + 2);
			this->addHeader(key, value);
		}
	}

	if (iterator != split.end())
		while (++iterator != split.end())
			this->_body = this->_body + *iterator + '\n';
}

Request::~Request() {}

/************************ Request::NotAllowedException ************************/

Request::NotAllowedException::NotAllowedException(const std::string &reason) : _reason(reason) {}

Request::NotAllowedException::~NotAllowedException() throw() {}

/**
 * Reason why return 'const char*' instead of 'std::string'
 * - Exception safety: constructing std::string could potentially throw another
 * eception (eg bad_alloc_exception if system out of memory). Throwing an
 * exception while another exception is already being handled will lead to
 * std::terminate() being called.
 * - Historical reasons: The design of exceptions in C++ predates the STL
 * library and std::string.
 * - Compatability: The use of const char* ensures compatibility with C function
 * and libraries that might interact with C++ code.
 */
const char *Request::NotAllowedException::what() const throw()
{
	return _reason.c_str();
}
