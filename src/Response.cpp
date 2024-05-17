/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:12:06 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 16:40:13 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// const std::map<int, std::string> Response::statusMap = {
// 	{200, "OK"},
// 	{202, "Accepted"},
// 	{302, "Found"},
// 	{403, "Forbidden"},
// 	{404, "Not Found"},
// 	{405, "Method Not Allowed"},
// 	{500, "Internal Server Error"}};

std::map<int, std::string> Response::statusMap;

Response::Response() : _logger(Logger("Response"))
{
	_statusCode = 200;
	statusMap[200] = "OK";
	statusMap[404] = "Not Found";
	statusMap[500] = "Internal Server Error";
	std::cout << "Response constructor" << std::endl;
}

Response::~Response() {
	std::cout << "Response destructor" << std::endl;
}

void Response::setStatusCode(int status)
{
	this->_statusCode = status;
}

int Response::getStatusCode() const
{
	return this->_statusCode;
}

std::string Response::toString() const
{
	std::stringstream responseStream;

	// Status line
	responseStream << "HTTP/1.1 " << this->_statusCode << " " << Response::statusMap.at(this->_statusCode) << std::endl;


	// header
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		responseStream << it->first << ": " << it->second << std::endl;
	}


	// optional body
	if (this->_body != "")
		responseStream << std::endl
					   << this->_body << std::endl;

	return responseStream.str();
}

void Response::errorResponse(int statusCode, std::string message)
{
	setStatusCode(statusCode);
	setBody(statusMap[statusCode]);
	_logger.error(message);
}