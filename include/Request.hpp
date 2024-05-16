/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 02:00:28 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 04:49:44 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <exception>

#include "HttpMessage.hpp"
#include "Utility.hpp"

class Request : public HttpMessage
{
private:
	std::string _method;
	std::string _uri;

public:
	Request();
	Request(const std::string &requestString);
	Request(Request const &src);
	~Request();

	class NotAllowedException : public std::exception
	{
	private:
		std::string _reason; // reason for the exception

	public:
		NotAllowedException(const std::string &reason); // constructor
		virtual ~NotAllowedException() throw();			// destructor
		virtual const char *what() const throw();		// override what()
	};

	static const std::vector<std::string> methodVector;
	void setMethod(const std::string &method);
	void setUri(const std::string &uri);
	const std::string getMethod() const;
	const std::string getUri() const;
	const std::string getRoute() const;
	const std::string getResource() const;
};

#endif
