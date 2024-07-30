/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/06/21 10:05:40 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

#include <iostream>
#include <iomanip>

Router::Router() : _logger(Logger("Router"))
{
	_projectDir = "/Users/user/sidess/webserv/";
}

Router::Router(std::string projectDir) : _projectDir(projectDir), _logger(Logger("Router")) {}

Router::~Router()
{
	for (std::map<std::string, RouteDetails *>::iterator it = this->_routeTable.begin(); it != this->_routeTable.end(); it++)
	{
		delete it->second;
	}
	this->_routeTable.clear();
}

void Router::addRoute(const std::string &path, RouteDetails *routeDetail)
{
	this->_routeTable[path] = routeDetail;
}

void Router::assignHandlers(requestHandlerVec &handlers)
{
	this->_requestHandlers = &handlers;
}

bool checkAllowedMethods(std::string reqMethods, int allowedMethods)
{
	if (reqMethods == "GET")
	{
		return allowedMethods & GET;
	}
	else if (reqMethods == "POST")
	{
		return allowedMethods & POST;
	}
	else if (reqMethods == "DELETE")
	{
		return allowedMethods & DELETE;
	}
	return false;
}

RouteDetails *Router::getRouteDetails(const std::string &path, const std::map<std::string, RouteDetails *> &routeTable)
{
	std::map<std::string, RouteDetails *>::const_iterator routeIt;
	try
	{
		size_t toFind = 1;
		while (toFind != std::string::npos)
		{
			toFind = path.find('/', toFind);
			std::string route;
			if (toFind != std::string::npos)
			{
				route = path.substr(0, toFind);
			}
			else
			{
				route = "/";
			}
			this->_logger.warning("toFind: " + route);
			routeIt = routeTable.find(route);
			if (routeIt != routeTable.end())
			{
				return routeIt->second;
			} else if (route == "/") {
				break;
			}
		}
	}
	catch (std::exception &e)
	{
		this->_logger.log("Error: " + std::string(e.what()));
	}
	return NULL;
}

void Router::routeRequest(const Request &request, Response &response)
{
	response.addHeader("Connection", "closed");

	RouteDetails *routeDetail = getRouteDetails(request.getUri(), this->_routeTable);
	if (routeDetail)
	{
		if (checkAllowedMethods(request.getMethod(), routeDetail->allowedMethods) == false)
		{
			response.errorResponse(405, "Method not allowed");
			return;
		}

		std::string fullPath = this->_projectDir + routeDetail->root + request.getResource();
		for (requestHandlerVecIt it = this->_requestHandlers->begin(); it != this->_requestHandlers->end(); it++)
		{
			if ((*it)->handleRequest(request, response, *routeDetail, fullPath))
				return;
		}
		if (request.getMethod() == "GET")
		{
			_logger.log("sending default plain success");
			response.setStatusCode(200);
			response.addHeader("Content-Type", "text/html");
			response.addHeader("Connection", "closed");
			response.setBody("<html>Welcome to the webserv, example.re homepage!</html>");
			return;
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
	for (std::map<std::string, RouteDetails *>::const_iterator it = this->_routeTable.begin(); it != this->_routeTable.end(); it++)
	{
		std::cout << std::left << std::setw(20) << it->first << std::endl;
	}
}

//  /jaclyn/bombom.html
