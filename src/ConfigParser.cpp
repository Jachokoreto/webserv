#include "ConfigParser.hpp"

void ConfigParser::parseConfig(const std::string& filename) {
    std::ifstream configFile(filename.c_str());
    std::string line;
    Location* currentLocation = NULL;

    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return;
    }

    while (getline(configFile, line)) {
        trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string key, value;
        ss >> key;

        if (key == "server") {
            continue;
        } else if (key == "location") {
            ss >> value;
            server.locations.push_back(Location());
            currentLocation = &server.locations.back();
            currentLocation->path = value;
        } else if (key == "}") {
            currentLocation = NULL;
        } else if (currentLocation) {
            parseLocationConfig(ss, key, *currentLocation);
        } else {
            parseServerConfig(ss, key, server);
        }
    }

    configFile.close();
}

void ConfigParser::parseServerConfig(std::stringstream& ss, std::string& key, Server& server) {
    std::string value;
    if (key == "listen") {
        ss >> server.listen;
    } else if (key == "root") {
        ss >> server.root;
    } else if (key == "index") {
        ss >> server.index;
    }
}

void ConfigParser::parseLocationConfig(std::stringstream& ss, std::string& key, Location& loc) {
    std::string value;
    if (key == "root") {
        ss >> loc.root;
    } else if (key == "index") {
        ss >> loc.index;
    } else if (key == "cgi_pass") {
        ss >> loc.cgi_pass;
    } else if (key == "autoindex") {
        ss >> value;
        loc.autoindex = (value == "on");
    } else if (key == "allowed_methods") {
        while (ss >> value) {
            loc.allowed_methods.push_back(value);
        }
    }
}

void ConfigParser::displayConfig() const {
    server.display();
}

void ConfigParser::trim(std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos) {
        str = "";
    } else {
        str = str.substr(first, (last - first + 1));
    }
}