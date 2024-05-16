/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 01:51:31 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 01:38:43 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <sys/stat.h>

#include "Request.hpp"
#include "Response.hpp"

class RequestHandler
{
public:
	virtual ~RequestHandler(){};
	virtual void handleRequest(const Request &request, Response &response, std::string root) = 0;
};

#endif
