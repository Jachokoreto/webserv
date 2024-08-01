#include "ConfigParser.hpp"

void ConfigParser::createServerBlocksFromConf(const std::string& filename, std::vector<RequestHandler*>& requestHandlers, std::vector<ServerBlock*>& serverBlocks) {
    std::ifstream configFile(filename.c_str());
    std::string line;
    ServerBlock *server = NULL;
    bool inServerBlock = false, inLocationBlock = false;
    RouteDetails *routeDetails;
    std::string currentRoute;

    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return;
    }

    while (getline(configFile, line)) {
        trim(line); // remove leading and trailing whitespaces
        if (line.empty() || line[0] == '#') continue; // skip empty lines and comments

        std::stringstream ss(line);
        std::string key, value, end;
        ss >> key;

        if (key == "server") {
            ss >> end;
            if (end == "{") {
                inServerBlock = true;
                // create new server block
                serverBlocks.push_back( new ServerBlock());
                server = serverBlocks.back();
                server->router.assignHandlers(requestHandlers);
                continue;
            } else {
                std::cerr << "Key server is reserved for server block\nie: server {...}" << std::endl;
                return;
            }
        } else if (key == "route") {
            ss >> value;
            ss >> end;
            // std::cout << value << end << inServerBlock << std::endl;
            if (value.empty() || end != "{" ) {
                std::cerr << "Invalid route block\nie: route /pathname {...}" << std::endl;
                return;
            } else if (inServerBlock == false) {
                std::cerr << "route should be inside a server {}" << std::endl;
                return;
            }
            currentRoute = value;
            routeDetails = new RouteDetails(value);
            inLocationBlock = true;
            continue;
        } else if (key == "}") {
            if (inLocationBlock) {
                server->router.addRoute(currentRoute, routeDetails);
                currentRoute = "";
                routeDetails = NULL;
                inLocationBlock = false;
            } else if (inServerBlock) {
                server = NULL;
                inServerBlock = false;
            }
        } else if (inLocationBlock) {
            parseLocationConfig(ss, key, *routeDetails);
        } else if (inServerBlock) {
            parseServerConfig(ss, key, *server);
        }
    }
    configFile.close();
}

void ConfigParser::parseServerConfig(std::stringstream& ss, std::string& key, ServerBlock& server) {
    std::string value;
    if (key == "listen") {
        ss >> server.listen;
    }
}

void ConfigParser::parseLocationConfig(std::stringstream& ss, std::string& key, RouteDetails& routeDetails) {
    std::string value;
    if (key == "root") {
        ss >> routeDetails.root;
    } else if (key == "index") {
        ss >> routeDetails.index;
    // } else if (key == "cgi_pass") {
    //     ss >> routeDetails.cgi_pass;
    } else if (key == "autoindex") {
        ss >> value;
        std::cout << "AUTOINDEX" << value << std::endl;
        routeDetails.autoindex = (value == "on");
    } else if (key == "allowed_methods") {
        while (ss >> value) {
            if (value == "GET")
                routeDetails.allowedMethods |= GET; // add GET to allowedMethods
            else if (value == "POST")
                routeDetails.allowedMethods |= POST; // add POST to allowedMethods
            else if (value == "DELETE")
                routeDetails.allowedMethods |= DELETE; // add DELETE to allowedMethods
            else {
                std::cerr << "Invalid method: " << value << std::endl;
                return;
            }
        }
    } else if (key == "cgi_pass") {
        ss >> routeDetails.cgiPass;
    }
}

void ConfigParser::displayConfig(ServerBlock& server) const {
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