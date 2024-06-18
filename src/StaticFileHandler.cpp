/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:58:26 by chenlee           #+#    #+#             */
/*   Updated: 2024/06/18 17:08:46 by chenlee          ###   ########.fr       */
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

bool handleGetRequest(const Request &request, Response &response, const RouteDetails &routeDetail, const std::string &fullPath, const struct stat &path_stat)
{
    if (S_ISDIR(path_stat.st_mode) && !routeDetail.index.empty())
        return serveFile(fullPath + "/" + routeDetail.index, response);
    else if (S_ISREG(path_stat.st_mode))
        return serveFile(fullPath, response);
    else
        return false;
}

std::string getCurrentTime()
{
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo = gmtime(&rawtime);
    char buffer[40];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return buffer;
}

bool handlePostRequest(const Request &request, Response &response, const RouteDetails &RouteDetails, const std::string &fullPath)
{
    std::ofstream file(fullPath.c_str(), std::ios::out | std::ios::binary);
    if (!file)
        response.errorResponse(500, "Failed to open file.");
    else
    {
        file.write(request.getBody().data(), request.getBody().size());
        if (file.fail())
            response.errorResponse(500, "Failed to save file.");
        else
        {
            response.setStatusCode(200);
            response.setBody("File uploaded successfully.");
            response.addHeader("Date", getCurrentTime());
            response.addHeader("Content-Type", "text/html");
        }
        file.close();
    }
}

bool handleDeleteRequest(Response &response, const std::string &fullPath, const struct stat &path_stat)
{
    if (S_ISREG(path_stat.st_mode))
    {
        if (remove(fullPath.c_str()) == 0)
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
    // Early handling for POST requests (for file uploads)
    if (request.getMethod() == "POST" && (routeDetail.allowedMethods & POST))
        return handlePostRequest(request, response, routeDetail, fullPath);

    // Use stat to check file or directory status for GET/DELETE methods
    struct stat path_stat;
    if (stat(fullPath.c_str(), &path_stat) != 0)
    {
        response.errorResponse(404, "Resource not found");
        return true;
    }
    if (request.getMethod() == "GET" && (routeDetail.allowedMethods & GET))
        return handleGetRequest(request, response, routeDetail, fullPath, path_stat);
    else if (request.getMethod() == "DELETE" && (routeDetail.allowedMethods & DELETE))
        return handleDeleteRequest(response, fullPath, path_stat);
    else
        return false;
}
