#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>

#include "colors.h"

namespace logger
{

    void log(std::string ctx, std::string str);
    void info(std::string ctx, std::string str);
    void warning(std::string ctx, std::string str);
    void error(std::string ctx, std::string str);

} // namespace webs

#endif //__LOGGER_H__