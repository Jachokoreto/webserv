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

Response::Response() : _logger(Logger("Response")), _ready(0)
{
    _statusCode = 200;
    statusMap[200] = "OK";
    statusMap[403] = "Forbidden";
    statusMap[404] = "Not Found";
    statusMap[405] = "Method not allowed";
    statusMap[500] = "Internal Server Error";
    _body = "";
    // std::cout << "Response constructor" << std::endl;
}

Response::~Response()
{
    // std::cout << "Response destructor" << std::endl;
}

void Response::setStatusCode(int status)
{
    this->_ready = 1;
    this->_statusCode = status;
}

void Response::setReady(int status)
{
    this->_ready = status;
}

int Response::getStatusCode() const
{
    return this->_statusCode;
}

int strlen(std::string toCount) {
    char * str = (char *)toCount.c_str();
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

std::string Response::toString()
{
    if (this->_ready != 1)
        return "";
    if (_responseString.empty())
    {

        std::stringstream responseStream;

        // Status line
        responseStream << "HTTP/1.1 " << this->_statusCode << " " << Response::statusMap.at(this->_statusCode) << "\r\n";

        if (!this->_body.empty())
        {
            // std::cout << "length: " << strlen(this->_body) << std::endl;
            this->addHeader("Content-Length", utl::toString(this->getBody().length()));
        }
        // header
        for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
        {
            responseStream << it->first << ": " << it->second << "\r\n";
        }


        responseStream << "\r\n";
        // optional body
        if (this->_body != "")
            responseStream << this->_body << "\n";
        _responseString = responseStream.str();
    }
    return _responseString;
}

void Response::errorResponse(int statusCode, std::string message)
{
    setStatusCode(statusCode);
    // setBody(statusMap[statusCode]);
    _logger.error(message);
}

void Response::truncateResponse(unsigned long length)
{
    if (this->_responseString.length() > length)
        this->_responseString = this->_responseString.substr(length);
}

// void Response::setResponseString(std::string string)
// {
//     std::stringstream responseStream;
//     responseStream << "HTTP/1.1 " << this->_statusCode << " " << Response::statusMap.at(this->_statusCode) << "\r\n";


//     this->_responseString += responseStream.str();
//     this->_responseString += string + "\r\n";

//     this->_ready = true;
// }