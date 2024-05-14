#include "ServerBlock.hpp"

void ServerBlock::display() const {
    std::cout << "Server Listen on: " << listen << std::endl
              << " Root: " << root << std::endl
              << " Index: " << index << std::endl;
    for (size_t i = 0; i < locations.size(); ++i) {
        locations[i].display();
    }
}

