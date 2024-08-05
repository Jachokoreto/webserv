/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:03:07 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/06 00:17:00 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

void HttpMessage::addHeader(const std::string &field, const std::string &value)
{
	this->_headers[field] = value;
}

void HttpMessage::setBody(const std::string responseBody)
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

void HttpMessage::parseHeaders(std::vector<std::string> split)
{

	// from split separate body and header, seperator is """
	std::vector<std::string>::iterator headerEndIterator = std::find(split.begin() + 1, split.end(), "");
	// std::vector<std::string> header(split.begin() + 1, iterator);
	for (std::vector<std::string>::iterator startIt = split.begin() + 1; startIt != headerEndIterator; startIt++)
	{
		// std::cout << *startIt << std::endl;
		std::size_t delimPos = startIt->find(": ");
		if (delimPos != std::string::npos && delimPos != 0)
		{
			std::string key = startIt->substr(0, delimPos);
			std::string value = startIt->substr(delimPos + 2);
			std::string::size_type pos;

			if ((pos = value.find("\r")) != std::string::npos)
			{
				value.erase(pos, 2);
			}
			this->addHeader(key, value);
		}
	}
};

HttpMessage::HttpMessage() {}

HttpMessage::~HttpMessage() {}
