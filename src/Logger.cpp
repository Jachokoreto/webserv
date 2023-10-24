#include "Logger.hpp"

Logger::Logger(std::string context)
{
    this->_context = context;
}

Logger::~Logger() {}

void Logger::output(std::string color, std::string str)
{
    time_t curr_time = time(NULL);
    char buffer[9];

    strftime(buffer, sizeof(buffer), "%X", localtime(&curr_time));

    std::cerr
        << color << buffer << "  " << this->_context << ": " << str << RESET << std::endl;
}

void Logger::log(std::string str)
{
    output("", str);
}

void Logger::info(std::string str)
{
    output(BLUE, str);
}

void Logger::warning(std::string str)
{
    output(YELLOW, str);
}

void Logger::error(std::string str)
{
    output(RED, str);
}