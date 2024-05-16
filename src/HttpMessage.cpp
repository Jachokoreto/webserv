/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:03:07 by chenlee           #+#    #+#             */
/*   Updated: 2024/04/14 15:22:11 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

void HttpMessage::addHeader(const std::string &field, const std::string &value)
{
	this->_headers[field] = value;
}

void HttpMessage::setBody(const std::string &responseBody)
{
	this->_body = responseBody;
}

const std::string HttpMessage::getBody() const
{
	return this->_body;
}

const std::string HttpMessage::getHeader(const std::string &field)
{
	std::map<std::string, std::string>::iterator it = this->_headers.find(field);
	if (it != this->_headers.end())
		return it->second;
	else
		return "";
}

const std::map<std::string, std::string> HttpMessage::getAllHeaders() const
{
	return this->_headers;
}

HttpMessage::HttpMessage() {}

HttpMessage::~HttpMessage() {}
