/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:19:09 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 04:51:10 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

const char *methods[] = {"GET", "POST", "PUT", "PATCH", "DELETE"};
const std::vector<std::string> Request::methodVector(methods, methods + sizeof(methods) / sizeof(char *));

const std::string Request::getUri() const
{
	return this->_uri;
}

const std::string Request::getMethod() const
{
	return this->_method;
}

const std::string Request::getRoute() const
{
	return this->_uri.substr(0, this->_uri.find('/', 1));
}

// Get the requested resource from the URI
// ie. /path/to/resource -> /to/resource
const std::string Request::getResource() const
{
	if (this->_uri.find('/', 1) == std::string::npos)
		return "";
	return this->_uri.substr(this->_uri.find('/', 1));
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

	// from split separate body and header, seperator is """
	std::vector<std::string>::iterator headerEndIterator = std::find(split.begin() + 1, split.end(), "");
	// std::vector<std::string> header(split.begin() + 1, iterator);
	for (std::vector<std::string>::iterator startIt = split.begin() + 1; startIt != headerEndIterator; startIt++)
	{
		std::size_t delimPos = startIt->find(": ");
		if (delimPos != std::string::npos && delimPos != 0)
		{
			std::string key = startIt->substr(0, delimPos);
			std::string value = startIt->substr(delimPos + 2);
			this->addHeader(key, value);
		}
	}

	if (headerEndIterator != split.end())
		while (++headerEndIterator != split.end())
			this->_body = this->_body + *headerEndIterator + '\n';
}

// Request::Request() {}

Request::Request(Request const &src){
	this->_method = src._method;
	this->_uri = src._uri;
	this->_version = src._version;
	this->_headers = src._headers;
	this->_body = src._body;
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
