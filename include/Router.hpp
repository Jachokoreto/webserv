/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:50:55 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 00:16:40 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "StaticFileHandler.hpp"
// #include "CGIHandler.hpp"

#define GET (1 << 0)	// 00000001 in binary, represents "GET"
#define POST (1 << 1)	// 00000001 in binary, represents "POST"
#define DELETE (1 << 2) // 00000001 in binary, represents "DELETE"

struct routeDetails
{
	bool autoIndex;
	std::string index;
	std::string root;
	int allowedMethods;
};

class Router
{
private:
	std::map<std::string, routeDetails> _routeTable;

public:
	Router();
	~Router();
	void addRoute(const std::string &path, bool autoIndex, std::string index, std::string root, int allowedMethods);
	void routeRequest(const Request &request, Response &response);
};

#endif
