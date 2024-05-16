/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 01:51:31 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/16 19:57:02 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <sys/stat.h>

#include "Request.hpp"
#include "Response.hpp"

class RequestHandler
{
protected:
public:
	virtual ~RequestHandler(){};
	virtual void handleRequest(const Request &request, Response &response) = 0;
};

#endif
