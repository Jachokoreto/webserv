#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>

#include "colors.h"

class Logger
{
public:
    Logger(std::string context);


    void log(std::string str);
    void info(std::string str);
    void warning(std::string str);
    void error(std::string str);

private:
    Logger();
    ~Logger();

    std::string _context;
};

#endif