/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/04 02:50:20 by chenlee          ###   ########.fr       */
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

	// Append "/" to the end of _projectDir if it's not already there
	if (!_projectDir.empty())
	{
#ifdef _WIN32
		if (_projectDir.back() != '\\')
			_projectDir += "\\";
#else
		if (_projectDir.back() != '/')
			_projectDir += "/";
#endif
	}
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
	std::cout << "methods: " << routeDetail->allowedMethods << std::endl;
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

RouteDetails *Router::getRouteDetails(const std::string &path, const routeTableMap &routeTable)
{
	RouteDetails *ret;
	for (routeTableMap::const_iterator it = routeTable.begin(); it != routeTable.end(); ++it)
	{
		if (path.find(it->first) != std::string::npos)
		{
			ret = it->second;
			if (!ret->cgiPass.empty())
				return ret;
		}
	}
	return ret;
	// routeTableMap::const_iterator routeIt;
	// try
	// {
	// size_t toFind = 1;
	// while (toFind != std::string::npos)
	// {
	// 	toFind = path.find('/', toFind);
	// 	std::string route;
	// 	if (toFind != std::string::npos)
	// 	{
	// 		route = path.substr(0, toFind);
	// 	}
	// 	else
	// 	{
	// 		route = "/";
	// 	}
	// 	this->_logger.warning("toFind: " + route);
	// 	routeIt = routeTable.find(route);
	// 	if (routeIt != routeTable.end())
	// 	{
	// 		return routeIt->second;
	// 	} else if (route == "/") {
	// 		break;
	// 	}
	// }

	// }
	// catch (std::exception &e)
	// {
	// 	this->_logger.log("Error: " + std::string(e.what()));
	// }
	// return NULL;
}

void Router::routeRequest(const Request &request, Response &response)
{
	response.addHeader("Connection", "closed");

	RouteDetails *routeDetail = getRouteDetails(request.getUri(), this->_routeTable);
	if (routeDetail)
	{
		this->_logger.log("Handling route detail: " + routeDetail->route);
		if (checkAllowedMethods(request.getMethod(), routeDetail->allowedMethods) == false)
		{
			response.errorResponse(405, "Method not allowed");
			return;
		}

		std::string fullPath = this->_projectDir + routeDetail->root + request.getResource();
		for (requestHandlerVecIt it = this->_requestHandlers->begin(); it != this->_requestHandlers->end(); it++)
		{
			if ((*it)->checkIfHandle(request, *routeDetail, fullPath) == false)
				continue;
			if ((*it)->handleRequest(request, response, *routeDetail, fullPath))
				return;
		}
		if (request.getMethod() == "GET")
		{
			_logger.log("sending default plain success");
			response.setStatusCode(200);
			response.addHeader("Content-Type", "text/html");
			// response.addHeader("Connection", "closed");
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
