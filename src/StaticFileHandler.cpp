/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:58:26 by chenlee           #+#    #+#             */
/*   Updated: 2024/05/16 23:57:01 by chenlee          ###   ########.fr       */
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

std::string normalizePath(const std::string &path)
{
	std::vector<std::string> parts = utility::splitStringByDelim(path, '/');
	std::vector<std::string> stack;

	for (size_t i = 0; i < parts.size(); ++i)
	{
		if (parts[i] == "." || parts[i].empty())
			continue; // Skip current directory references and empty tokens
		else if (parts[i] == "..")
		{
			if (!stack.empty())
				stack.pop_back(); // Pop the last directory aka move up in the directory tree
		}
		else
		{
			stack.push_back(parts[i]);
		}
	}
	std::stringstream result;
	for (size_t i = 0; i < stack.size(); ++i)
	{
		result << "/" << stack[i];
	}
	return result.str().empty() ? "/" : result.str();
}

std::string urlDecode(const std::string &encoded)
{
	std::string result;
	result.reserve(encoded.length());
	for (size_t i = 0; i < encoded.length(); i++)
	{
		if (encoded[i] == '%' && i + 2 < encoded.length())
		{
			int value = 0;
			std::istringstream is(encoded.substr(i + 1, 2));
			if (is >> std::hex >> value)
			{
				result += static_cast<char>(value);
				i += 2;
			}
			else
				result += '%';
		}
		else if (encoded[i] == '+')
			result += ' ';
		else
			result += encoded[i];
	}
	return result;
}

StaticFileHandler::StaticFileHandler() {}

StaticFileHandler::~StaticFileHandler() {}

void StaticFileHandler::handleRequest(const Request &request, Response &response)
{
	(void)request;
	(void)response;
	// std::string decodedPath = urlDecode(request.getUri());
	// std::string normalizedPath = normalizePath(decodedPath);
	// std::string filePath = this->_rootDirectory + request.getUri();

	// struct stat path_stat;
	// stat(filePath.c_str(), &path_stat);
	// if (!S_ISREG(path_stat.st_mode)) // Check if it's not a regular file
	// {
	// 	if (S_ISDIR(path_stat.st_mode))
	// 	{
	// 		// Handle directory case: serve an index file to list directory contents
	// 		filePath += "/index.html";
	// 		stat(filePath.c_str(), &path_stat);
	// 		if (!S_ISREG(path_stat.st_mode))
	// 		{
	// 			// Check again in case default file is not a regular file / not available
	// 			response.setStatusCode(403);
	// 			response.setBody("Reason: Directory access is not allowed!");
	// 			return;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		response.setStatusCode(404);
	// 		return;
	// 	}
	// }

	// std::cout << filePath << std::endl;

	// std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
	// if (!file)
	// {
	// 	response.setStatusCode(404);
	// 	return;
	// }

	// Read the whole file into a string
	// std::string content(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
	// file.close();

	// response.setStatusCode(200);
	// response.setBody(content);
	// response.addHeader("Content-Type", getMimeType(filePath));
}
