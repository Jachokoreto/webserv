/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 01:51:31 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 16:55:55 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <sys/stat.h>

#include "Request.hpp"
#include "Response.hpp"
#include "RouteDetails.hpp"

class RequestHandler
{
public:
	virtual ~RequestHandler(){};
	virtual bool handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath) = 0;
};

#endif
