#include "ServerBlock.hpp"

ServerBlock::ServerBlock() {
    listen = 0;

    std::cout << "ServerBlock created" << std::endl;
}

void ServerBlock::display() const {
    std::cout << "Server Listen on: " << listen << std::endl;
    router.display();
}


string ServerBlock::handleRequest(string request) {
    std::cout << "Handling request on port " << listen << std::endl;
    std::string copy(request);
    if (copy.find("\r\n\r\n") != std::string::npos) {
		std::cout << "Request complete" << std::endl;
        std::cout << GREEN << request << RESET << std::endl;
		// if (FD_ISSET(_fd, _master)) {
			// std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
			// std::cout << _fd << _index << std::endl;
			// send(_fd, response.c_str(), response.length(), 0);
			// this->_logger.log("Response sent");
		// }			
		return "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	}
    return "";
}
