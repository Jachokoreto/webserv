/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:54:29 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/16 12:08:34 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
#define STATICFILEHANDLER_HPP

#define PATH_MAX 32767

#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "RequestHandler.hpp"
#include "Utility.hpp"

class StaticFileHandler : public RequestHandler
{
public:
	StaticFileHandler();
	~StaticFileHandler();
	void handleRequest(const Request &request, Response &response) override;
};

#endif