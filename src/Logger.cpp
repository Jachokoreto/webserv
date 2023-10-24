#include "Logger.hpp"

Logger::Logger(std::string context)
{
    this->_context = context;
}

void Logger::log(std::string str)
{
    std::cerr << this->_context << ": " << str << std::endl;
}

void Logger::info(std::string str)
{
    std::cerr << BLUE << this->_context << ": " << str << RESET << std::endl;
}

void Logger::warning(std::string str)
{
    std::cerr << YELLOW << this->_context << ": " << str << RESET << std::endl;
}

void Logger::error(std::string str)
{
    std::cerr << RED << this->_context << ": " << str << RESET << std::endl;
}