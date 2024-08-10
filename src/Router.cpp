/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/08/10 19:24:08 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include "ServerBlock.hpp"

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
Router::Router(ServerBlock *serverBlock) : _logger(Logger("Router")), _serverBlock(serverBlock) {}

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
	// std::cout << "methods: " << routeDetail->allowedMethods << std::endl;
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
}

void Router::routeRequest(const Request &request, Response &response)
{
	// response.addHeader("Connection", "closed");

	if (request.getBody().length() > static_cast<std::string::size_type>(this->_serverBlock->getBodyLimit()))
	{
		response.errorResponse(413, "Request Entity Too Large");
		return;
	}

	RouteDetails *routeDetail = getRouteDetails(request.getUri(), this->_routeTable);
	if (routeDetail)
	{
		this->_logger.log("Handling route detail: " + routeDetail->route);
		if (routeDetail->redirection.empty() && checkAllowedMethods(request.getMethod(), routeDetail->allowedMethods) == false)
		{
			response.errorResponse(405, "Method not allowed");
			return;
		}

		std::string fullPath = this->_projectDir + routeDetail->root + request.getResource();
		for (requestHandlerVecIt it = this->_requestHandlers->begin(); it != this->_requestHandlers->end(); it++)
		{
			if ((*it)->checkIfHandle(request, *routeDetail, fullPath))
			{
				if ((*it)->handleRequest(request, response, *routeDetail, fullPath))
					return;
			}
		}
		response.errorResponse(404, "no handler found");
	}
	else
	{
		response.errorResponse(404, "route not found in route table");
	}
}

void Router::display(void) const
{
	// std::cout << "Router Table:" << std::endl;
	// for (std::map<std::string, RouteDetails *>::const_iterator it = this->_routeTable.begin(); it != this->_routeTable.end(); it++)
	// {
	// 	std::cout << std::left << std::setw(20) << it->first << std::endl;
	// }
}

//  /jaclyn/bombom.html
