/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/16 12:08:20 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

#include <iostream>
#include <iomanip>

Router::Router() {}

Router::Router(std::string rootDir) : _rootDirectory(rootDir) {}

Router::~Router() {}

void Router::addRoute(const std::string &path, RequestHandler *handler)
{
	this->_routeTable[path] = handler;
}

void Router::routeRequest(const Request &request, Response &response)
{
	const std::string path = request.getUri();
	const std::string trim = request.getUri().substr(0, path.find('/', 1));

	if (this->_routeTable.find(trim) != this->_routeTable.end())
	{
		// if its rootDir + '/'
		// if its rootDir/static-files/img.png -> call static file handler
		// /static-files/image/
		this->_routeTable[trim]->handleRequest(request, response);
	}
	else
		response.setStatusCode(404);
}
