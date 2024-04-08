/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 02:00:28 by chenlee           #+#    #+#             */
/*   Updated: 2024/04/09 00:42:37 by chenlee          ###   ########.fr       */
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

class Request
{
public:
	Request(const std::string &requestString);
	~Request();

	class NotAllowedException : public std::exception
	{
	private:
		std::string reason; // reason for the exception

	public:
		NotAllowedException(const std::string &reason); // constructor
		virtual ~NotAllowedException() throw();			// destructor
		virtual const char *what() const throw();		// override what()
	};

	static const std::vector<std::string> methodVector;
	std::string method;
	std::string uri;
	std::map<std::string, std::string> headers;
	std::string body;
};

#endif
