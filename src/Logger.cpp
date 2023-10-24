#include "Logger.hpp"

void logger::log(std::string ctx, std::string str)
{
    std::cerr << ctx << ": " << str << std::endl;
}

void logger::info(std::string ctx, std::string str)
{
    std::cerr << BLUE << ctx << ": " << str << RESET << std::endl;
}

void logger::warning(std::string ctx, std::string str)
{
    std::cerr << YELLOW << ctx << ": " << str << RESET << std::endl;
}

void logger::error(std::string ctx, std::string str)
{
    std::cerr << RED << ctx << ": " << str << RESET << std::endl;
}