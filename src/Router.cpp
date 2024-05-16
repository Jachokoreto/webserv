/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:58:14 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 01:02:48 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

#include <iostream>
#include <iomanip>

Router::Router() {}

Router::~Router() {}

void Router::addRoute(const std::string &path, bool autoIndex, std::string index, std::string root, int allowedMethods)
{
	routeDetails bruh;
	bruh.autoIndex = autoIndex;
	bruh.index = index;
	bruh.root = root;
	bruh.allowedMethods = allowedMethods;
	this->_routeTable[path] = bruh;
}

void Router::routeRequest(const Request &request, Response &response)
{
	const std::string path = request.getUri();
	const std::string trim = path.substr(0, path.find('/', 1));

	const std::map<std::string, routeDetails>::iterator it = this->_routeTable.find(trim);
	std::cout << "trim" << trim << ", path" << path << std::endl;
	if (it != this->_routeTable.end())
	{
		// if its rootDir + '/'
		// if its rootDir/static-files/img.png -> call static file handler
		// /static-files/image/
		// this->_routeTable[trim]->handleRequest(request, response);
		struct stat path_stat;
		stat(path.c_str(), &path_stat);
		if (!S_ISREG(path_stat.st_mode)) // Check if it's not a regular file
		{
			// this check not correct, can use debugger to see
			if (S_ISDIR(path_stat.st_mode)) // If it is directory
			{
				if (!it->second.index.empty()) // index file configured
				{
					// try find the index file in path, ie:
					// index boomba.html
					// GET /jaclyn/abc/123 -> /public/usr/jaclyn-dir/abc/123/boomba.html
					// if cant find, run autoindex if autoindex is  true
					const std::string temp = it->second.root + path.substr(trim.length(), path.length());
					std::cout << temp << std::endl;
				}
				// stat(filePath.c_str(), &path_stat);
				if (!S_ISREG(path_stat.st_mode))
				{
					// Check again in case default file is not a regular file / not available
					response.errorResponse(404, "Directory access is not allowed!");
					return;
				}
			}
			else
			{
				response.errorResponse(404, "request is not dir");
				return;
			}
		}
	}
	else
		response.errorResponse(404, "route not found in route table");
}
