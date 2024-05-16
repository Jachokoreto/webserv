#include "AutoindexHandler.hpp"



/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

AutoindexHandler::AutoindexHandler()
{
}

AutoindexHandler::~AutoindexHandler()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void AutoindexHandler::handleRequest(std::string req, std::string res)
{
    std::cout << "AutoindexHandler::handleRequest" << std::endl;
    std::cout << req << std::endl;
    std::cout << res << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
