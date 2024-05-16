/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:42:28 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/15 23:25:17 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>

#include "HttpMessage.hpp"

class Response : public HttpMessage
{
private:
	int _statusCode;
	std::string _contentType;
	int _contentLength;
	std::string _date;
	std::string _server;
	std::string _connection;

public:
	Response();
	~Response();

	static const std::map<int, std::string> statusMap;
	void setStatusCode(int status);
	int getStatusCode() const;
	std::string toString() const;
};

#endif
