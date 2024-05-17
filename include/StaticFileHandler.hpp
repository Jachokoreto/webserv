/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:54:29 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 16:17:27 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
#define STATICFILEHANDLER_HPP

// #define CUSTOM_PATH_MAX 32767

#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "RequestHandler.hpp"
#include "Utility.hpp"

class StaticFileHandler : public RequestHandler
{
public:
	StaticFileHandler();
	~StaticFileHandler();
	bool handleRequest(const Request &request, Response &response, RouteDetails &routeDetails);
};

#endif