/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:42:28 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 00:32:17 by chenlee          ###   ########.fr       */
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
	std::string _contentType;
	int _contentLength;
	std::string _date;
	std::string _server;
	std::string _connection;
	Logger _logger;

public:
	Response();
	~Response();

	static std::map<int, std::string> statusMap;
	void setStatusCode(int status);
	int getStatusCode() const;
	std::string toString() const;
	void errorResponse(int status, std::string message);
};

#endif
