/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:56:50 by chenlee           #+#    #+#             */
/*   Updated: 2024/04/22 03:00:14 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APIHANDLER_HPP
#define APIHANDLER_HPP

#include "RequestHandler.hpp"

class APIHandler : public RequestHandler
{
public:
	void handleRequest(const Request &request, Response &response);
};

#endif