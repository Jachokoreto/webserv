/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteDetails.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 03:10:02 by jatan             #+#    #+#             */
/*   Updated: 2024/05/17 13:10:22 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTEDETAILS_HPP
#define ROUTEDETAILS_HPP

#include <string>

class RouteDetails
{
public:
	RouteDetails();
	RouteDetails(bool autoindex, std::string index, std::string root, int allowedMethods);
	~RouteDetails();

	bool autoindex;
	std::string index;
	std::string root;
	int allowedMethods;
};

#endif