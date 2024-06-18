/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:19:09 by chenlee           #+#    #+#             */
/*   Updated: 2024/06/18 18:55:44 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

const char *methods[] = {"GET", "POST", "PUT", "PATCH", "DELETE"};
const std::vector<std::string> Request::methodVector(methods, methods + sizeof(methods) / sizeof(char *));

std::string normalizePath(const std::string &path)
{
	std::cout << path << std::endl;
	std::vector<std::string> parts = utl::splitStringByDelim(path, '/');
	std::vector<std::string> stack;

	for (size_t i = 0; i < parts.size(); i++)
	{
		if (parts[i] == "." || parts[i].empty())
			continue; // Skip current directory references and empty tokens
		else if (parts[i] == "..")
		{
			if (!stack.empty())
				stack.pop_back(); // Pop the last directory aka move up in the directory tree
		}
		else
		{
			stack.push_back(parts[i]);
		}
	}
	std::stringstream result;
	for (size_t i = 0; i < stack.size(); i++)
		result << "/" << stack[i];
	return result.str().empty() ? "/" : result.str();
}

std::string urlDecode(const std::string &encoded)
{
	std::string result;
	result.reserve(encoded.length());
	for (size_t i = 0; i < encoded.length(); i++)
	{
		if (encoded[i] == '%' && i + 2 < encoded.length())
		{
			int value = 0;
			std::istringstream is(encoded.substr(i + 1, 2));
			if (is >> std::hex >> value)
			{
				result += static_cast<char>(value);
				i += 2;
			}
			else
				result += '%';
		}
		else if (encoded[i] == '+')
			result += ' ';
		else
			result += encoded[i];
	}
	return result;
}

std::string sanitizeUri(const std::string &uri)
{
	const std::string decodedPath = urlDecode(uri);
	const std::string normalizedPath = normalizePath(decodedPath);
	return (normalizedPath);
}

const std::string Request::getUri() const
{
	return this->_uri;
}

const std::string Request::getMethod() const
{
	return this->_method;
}

// Get the requested resource from the URI
// ie. HTTP/1.1 GET /path/to/resource -> /path/to/resource
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

Request::Request(const std::string &requestString): _logger(Logger("Request"))
{
	if (requestString.empty()) return ;
	std::vector<std::string> split = utl::splitStringByDelim(requestString, '\n');
	std::vector<std::string> requestLine = utl::splitStringByDelim(split[0], ' ');
	const std::string &method = requestLine[0];
	// if (std::find(Request::methodVector.begin(), Request::methodVector.end(), method) == Request::methodVector.end())
	// 	throw Request::NotAllowedException("Invalid method");
	this->setMethod(method);
	_logger.info(split[0]);
	this->setUri(sanitizeUri(requestLine[1]));
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

Request::Request(Request const &src): _logger(Logger("Request"))
{
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
