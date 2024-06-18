/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:42:28 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/18 23:15:01 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>

#include "HttpMessage.hpp"
#include "Logger.hpp"

class Response : public HttpMessage
{
private:
	int _statusCode;
	std::string _responseString;
	Logger _logger;
	// std::string _contentType;
	// int _contentLength;
	// std::string _date;
	// std::string _server;
	// std::string _connection;

public:
	Response();
	~Response();

	static std::map<int, std::string> statusMap;
	void setStatusCode(int status);
	int getStatusCode() const;
	std::string toString();
	void errorResponse(int status, std::string message);
	void truncateResponse(unsigned long length);
};

#endif
