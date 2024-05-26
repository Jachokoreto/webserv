/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:50:55 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 22:38:05 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "HttpMethods.h"
#include "StaticFileHandler.hpp"
#include "RouteDetails.hpp"
// #include "CGIHandler.hpp"

class Router
{
private:
	std::map<std::string, RouteDetails *> _routeTable;
	std::string _projectDir;

public:
	Router();
	Router(std::string projectDir);
	~Router();
	void addRoute(const std::string &path, RouteDetails *routeDetail);
	void routeRequest(const Request &request, Response &response);
	void display(void) const;
};

#endif
