#include "RouteDetails.hpp"

RouteDetails::RouteDetails()
{
	autoindex = false;
	index = "";
	root = "";
	allowedMethods = 0;
}

RouteDetails::RouteDetails(bool autoindex, std::string index, std::string root, int allowedMethods) : autoindex(autoindex), index(index), root(root), allowedMethods(allowedMethods) {}

RouteDetails::~RouteDetails()
{
}