/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:12:06 by chenlee           #+#    #+#             */
/*   Updated: 2024/06/18 18:29:40 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::statusMap;

Response::Response() : _logger(Logger("Response"))
{
    _statusCode = 200;
    statusMap[200] = "OK";
    statusMap[403] = "Forbidden";
    statusMap[404] = "Not Found";
    statusMap[405] = "Method not allowed";
    statusMap[500] = "Internal Server Error";
    // std::cout << "Response constructor" << std::endl;
}

Response::~Response()
{
    // std::cout << "Response destructor" << std::endl;
}

void Response::setStatusCode(int status)
{
    this->_statusCode = status;
}

int Response::getStatusCode() const
{
    return this->_statusCode;
}

std::string Response::toString()
{
    if (_responseString.empty())
    {

        std::stringstream responseStream;

        // Status line
        responseStream << "HTTP/1.1 " << this->_statusCode << " " << Response::statusMap.at(this->_statusCode) << "\r\n";

        // header
        for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
        {
            responseStream << it->first << ": " << it->second << "\r\n";
        }

        // optional body
        if (this->_body != "")
            responseStream << "\r\n"
                           << this->_body;
        _responseString = responseStream.str();
    }
    return _responseString;
}

void Response::errorResponse(int statusCode, std::string message)
{
    setStatusCode(statusCode);
    setBody(statusMap[statusCode]);
    _logger.error(message);
}

void Response::truncateResponse(unsigned long length)
{
    if (this->_responseString.length() > length)
        this->_responseString = this->_responseString.substr(length, this->_responseString.length());
}