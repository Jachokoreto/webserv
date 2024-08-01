/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/01 23:07:27 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

Router::Router() : _logger(Logger("Router"))
{
	char cwd[PATH_MAX];
#ifdef _WIN32
	if (GetCurrentDirectory(PATH_MAX, cwd) != 0)
	{
		_projectDir = cwd;
	}
	else
	{
		printf("Failed to get project directory: %lu\n", GetLastError());
	}
#else
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		_projectDir = cwd;
	}
	else
	{
		perror("Failed to get project directory");
	}
#endif
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

void Router::routeRequest(const Request &request, Response &response)
{
	(void)response;
	const std::string path = request.getUri();
	const std::string trim = path.substr(0, path.find('/', 1));

	response.addHeader("Connection", "closed");

	const std::map<std::string, RouteDetails *>::iterator routeIt = this->_routeTable.find(trim);
	if (routeIt != this->_routeTable.end())
	{
		if (checkAllowedMethods(request.getMethod(), routeIt->second->allowedMethods) == false)
		{
			response.errorResponse(405, "Method not allowed");
			return;
		}
		std::string fullPath = this->_projectDir + routeIt->second->root + request.getResource();

		for (requestHandlerVecIt it = this->_requestHandlers->begin(); it != this->_requestHandlers->end(); it++)
		{
			if ((*it)->handleRequest(request, response, *routeIt->second, fullPath))
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
		std::cout << "no handler found" << std::endl;
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
