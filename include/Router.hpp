/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:50:55 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 04:11:02 by jatan            ###   ########.fr       */
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

// struct routeDetails
// {
// 	bool autoindex;
// 	std::string index;
// 	std::string root;
// 	int allowedMethods;
// };

class Router
{
private:
	std::map<std::string, RouteDetails*> _routeTable;

public:
	Router();
	~Router();
	void addRoute(const std::string &path, RouteDetails *bruh);
	void routeRequest(const Request &request, Response &response);
	void display(void) const;
};

#endif
