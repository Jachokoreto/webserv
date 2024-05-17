/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 16:56:53 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

#include <iostream>
#include <iomanip>

Router::Router() {}

Router::Router(std::string projectDir) : _projectDir(projectDir) {}

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

void Router::routeRequest(const Request &request, Response &response)
{
	(void)response;
	const std::string path = request.getUri();
	const std::string trim = path.substr(0, path.find('/', 1));

	const std::map<std::string, RouteDetails *>::iterator routeIt = this->_routeTable.find(trim);
	if (routeIt != this->_routeTable.end())
	{
		std::string fullPath = this->_projectDir + routeIt->second->root + request.getResource();
		std::cout << "fullPath: " << fullPath << std::endl;
		StaticFileHandler sfHandler;
		bool outcome = sfHandler.handleRequest(request, response, *routeIt->second, fullPath);
		std::cout
			<< "bool: " << outcome << std::endl;
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