#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>

#include "colors.h"

class Logger
{
public:
    Logger(std::string context);
    Logger(const Logger &src);
    Logger &operator=(const Logger &rhs);
    ~Logger();

    void log(std::string str) const;
    void info(std::string str) const;
    void warning(std::string str) const;
    void error(std::string str) const;

private:
    void output(std::string color, std::string str) const;
    Logger();
    std::string _context;
    static int _loggerCount;
};

std::ostream &			operator<<( std::ostream & o, Logger const & i );

#endif