/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:50:55 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/16 17:41:39 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "RequestHandler.hpp"

class Router
{
private:
	std::map<std::string, RequestHandler *> _routeTable;

public:
	Router();
	Router(std::string rootDir);
	~Router();
	void addRoute(const std::string &path, RequestHandler *handler);
	void routeRequest(const Request &request, Response &response);
};

#endif
