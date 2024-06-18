/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:50:55 by chenlee           #+#    #+#             */
/*   Updated: 2024/06/07 19:37:56 by jatan            ###   ########.fr       */
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

public:

	typedef std::vector<RequestHandler *> requestHandlerVec;
	typedef std::vector<RequestHandler *>::iterator requestHandlerVecIt;

	typedef std::map<std::string, RouteDetails *> routeTableMap;
	typedef std::map<std::string, RouteDetails *>::iterator routeTableMapIt;

	Router();
	Router(std::string projectDir);
	~Router();
	void addRoute(const std::string &path, RouteDetails *routeDetail);
	void routeRequest(const Request &request, Response &response);
	void display(void) const;
	void assignHandlers(requestHandlerVec &handlers);

private:
	routeTableMap _routeTable;
	requestHandlerVec *_requestHandlers;
	std::string _projectDir;
	Logger _logger;
};

#endif
