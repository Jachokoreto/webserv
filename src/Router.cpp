/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 17:01:32 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

#include <iostream>
#include <iomanip>

Router::Router() {}

Router::~Router() {
	for (std::map<std::string, RouteDetails*>::iterator it = this->_routeTable.begin(); it != this->_routeTable.end(); it++)
	{
		delete it->second;
	}
	this->_routeTable.clear();
}

void Router::addRoute(const std::string &path, RouteDetails *bruh)
{
	// RouteDetails bruh;
	// bruh.autoindex = autoindex;
	// bruh.index = index;
	// bruh.root = root;
	// bruh.allowedMethods = allowedMethods;
	this->_routeTable[path] = bruh;
}


void Router::assignHandlers(requestHandlerVec &handlers)
{
	this->_requestHandlers = &handlers;
}

void Router::routeRequest(const Request &request, Response &response)
{
	const std::string path = request.getUri();
	const std::string trim = path.substr(0, path.find('/', 1));

	const std::map<std::string, RouteDetails*>::iterator routeIt = this->_routeTable.find(trim);
	if (routeIt != this->_routeTable.end())
	{
		for (requestHandlerVecIt it = this->_requestHandlers->begin(); it != this->_requestHandlers->end(); it++)
		{
			if ((*it)->handleRequest(request, response, *routeIt->second)) {
				return ;
			}
		}
		// reach here if no handler found in the loop above
		response.errorResponse(404, "no handler found");
	}
	else
		response.errorResponse(404, "route not found in route table");
}


void Router::display(void) const
{
	std::cout << "Router Table:" << std::endl;
	for (std::map<std::string, RouteDetails*>::const_iterator it = this->_routeTable.begin(); it != this->_routeTable.end(); it++)
	{
		std::cout << std::left << std::setw(20) << it->first << std::endl;
	}
}