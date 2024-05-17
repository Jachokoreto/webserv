/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:50:55 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 15:39:18 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "StaticFileHandler.hpp"
#include "RouteDetails.hpp"
// #include "CGIHandler.hpp"

#define GET (1 << 0)	// 00000001 in binary, represents "GET"
#define POST (1 << 1)	// 00000001 in binary, represents "POST"
#define DELETE (1 << 2) // 00000001 in binary, represents "DELETE"

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
