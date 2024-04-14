/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:12:06 by chenlee           #+#    #+#             */
/*   Updated: 2024/04/14 18:24:50 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

const std::map<int, std::string> Response::statusMap = {
	{200, "OK"},
	{202, "Accepted"},
	{302, "Found"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{500, "Internal Server Error"}};

Response::Response() : _statusCode(200){};

Response::~Response(){};

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
	responseStream << "HTTP/1.1 " << this->_statusCode << " " << Response::statusMap.at(this->_statusCode) << "\r\n";

	// headers
	for (std::pair<std::string, std::string> header : this->_headers)
		responseStream << header.first << ": " << header.second << "/r/n";

	// optional body
	if (this->_body != "")
		responseStream << "/r/n" << this->_body << "/r/n";

	return responseStream.str();
}
