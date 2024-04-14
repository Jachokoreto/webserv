#include "Webserver.hpp"
#include <iostream>

int main(void)
{
	// WebServer webserver;

	// webserver.start();
	const std::string requestStr = "POST /cgi-bin/process.cgi HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tutorialspoint.com\nContent-Type: text/xml; charset=utf-8\nContent-Length: length\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n\n<?xml version='1.0' encoding='utf-8'?>\n<string xmlns='http://clearforest.com/'>string</string>\n";
	Request request(requestStr);
	const std::string method = request.getMethod();
	const std::string uri = request.getUri();
	std::map<std::string, std::string> header = request.getAllHeaders();
	std::string body = request.getBody();
	std::cout << "METHOD: " << method + "\n"
			  << std::endl;
	std::cout << "URI: " << uri + "\n"
			  << std::endl;
	std::cout << "HEADER:" << std::endl;
	for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
	std::cout << "\nBODY:\n"
			  << body << std::endl;
}