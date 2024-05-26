/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:58:26 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/18 02:04:27 by chenlee          ###   ########.fr       */
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
	}
	return "application/octet-stream"; // Default MIME type
}

bool serveFile(const std::string &filePath, Response &response)
{
	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!file)
	{
		response.errorResponse(404, "File not found");
		return true;
	}
	std::string content(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	file.close();
	response.setStatusCode(200);
	response.setBody(content);
	response.addHeader("Content-Type", getMimeType(filePath));
	return true;
}

bool handleGetRequest(const std::string &path, const struct stat &path_stat, const Request &request, Response &response, const RouteDetails &routeDetail)
{
	if (S_ISDIR(path_stat.st_mode) && !routeDetail.index.empty())
		return serveFile(path + "/" + routeDetail.index, response);
	else if (S_ISREG(path_stat.st_mode))
		return serveFile(path, response);
	else
		return false;
}

bool handleDeleteRequest(const std::string &path, const struct stat &path_stat, Response &response)
{
	if (S_ISREG(path_stat.st_mode))
	{
		if (remove(path.c_str()) == 0)
			response.setStatusCode(202);
		else
			response.errorResponse(500, "Unable to delete file");
		return true;
	}
	return false;
}

StaticFileHandler::StaticFileHandler() {}

StaticFileHandler::~StaticFileHandler() {}

bool StaticFileHandler::handleRequest(const Request &request, Response &response, RouteDetails &routeDetail, const std::string &fullPath)
{
	struct stat path_stat;
	if (stat(fullPath.c_str(), &path_stat) != 0)
	{
		response.errorResponse(404, "Resource not found");
		return true;
	}

	if (request.getMethod() == "GET" && (routeDetail.allowedMethods & GET))
		return handleGetRequest(fullPath, path_stat, request, response, routeDetail);
	else if (request.getMethod() == "POST" && (routeDetail.allowedMethods & POST))
		return true;
	else if (request.getMethod() == "DELETE" && (routeDetail.allowedMethods & DELETE))
		return handleDeleteRequest(fullPath, path_stat, response);
	else
		return false;
}
