#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>
// #include <chrono>
#include <ctime>

#include "colors.h"

class Logger
{
public:
	Logger(std::string context);
	~Logger();

	void log(std::string str);
	void info(std::string str);
	void warning(std::string str);
	void error(std::string str);

private:
	void output(std::string color, std::string str);
	Logger();
	std::string _context;
};

#endif