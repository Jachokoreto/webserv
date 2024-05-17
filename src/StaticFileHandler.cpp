/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:58:26 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/17 17:14:14 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileHandler.hpp"

std::string getMimeType(const std::string &filePath)
{
	size_t dotPos = filePath.rfind('.');
	if (dotPos != std::string::npos)
	{
		std::string ext = filePath.substr(dotPos);
		if (ext == ".html")
			return "text/html";
		else if (ext == ".css")
			return "text/css";
		else if (ext == ".js")
			return "application/javascript";
		else if (ext == ".png")
			return "image/png";
		else if (ext == ".jpg" || ext == ".jpeg")
			return "image/jpeg";
		else if (ext == ".gif")
			return "image/gif";
		else
			return "application/octet-stream"; // Default MIME type
	}
	else
		return "application/octet-stream"; // Default MIME type
}

StaticFileHandler::StaticFileHandler() {}

StaticFileHandler::~StaticFileHandler() {}

bool StaticFileHandler::handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath)
{
	struct stat path_stat;
	stat(fullPath.c_str(), &path_stat);
	if (!S_ISREG(path_stat.st_mode)) // Check if it's not a regular file
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			// Handle directory case: serve an index file to list directory contents
			// fullPath += "/index.html";
			stat(fullPath.c_str(), &path_stat);
			if (!S_ISREG(path_stat.st_mode))
			{
				// Check again in case default file is not a regular file / not available
				response.setStatusCode(403);
				response.setBody("Reason: Directory access is not allowed!");
				return true;
			}
		}
		else
			return false;
	}

	std::cout << fullPath << std::endl;

	std::ifstream file(fullPath.c_str(), std::ios::in | std::ios::binary);
	if (!file)
	{
		response.errorResponse(404, "File not found");
		return true;
	}

	std::string content(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	file.close();

	response.setStatusCode(200);
	response.setBody(content);
	response.addHeader("Content-Type", getMimeType(fullPath));
	return true;
}
