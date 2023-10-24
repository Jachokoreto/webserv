/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:06:07 by jatan             #+#    #+#             */
/*   Updated: 2023/10/24 15:52:26 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"
#include <stdexcept>

WebServer::WebServer()
{
}

WebServer::~WebServer()
{
}

/**
 * To start the server socket
 */
void WebServer::start()
{
    int connectSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    std::ostringstream ss;

    createSocket();

    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((connectSocket = accept(this->_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        ss << "connected on " << address.sin_port << std::endl;
        logger::log("socket", ss.str());
        handleRequest(connectSocket);
        ss.flush();

        close(connectSocket);
    }
}

// To create the listening socket
int WebServer::createSocket()
{
    struct addrinfo hints, *servinfo, *p;
    int rv, yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPV4
    hints.ai_socktype = SOCK_STREAM; // socket stream
    hints.ai_flags = AI_PASSIVE;     // use my IP

    // get a linked list for addrinfo and store in rv
    // thttps://beej.us/guide/bgnet/examples/server.c
    // This way is easily adapted to different network configurations and
    // protocols, and can handle errors and exceptions more gracefully.
    // * arg1: the host name, arg2: the port
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((this->_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            logger::warning("server: socket", strerror(errno));
            continue;
        }

        if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            logger::error("setsockopt", strerror(errno));
            // exit(1);
            throw std::runtime_error(strerror(errno));
        }

        if (bind(this->_socket, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(this->_socket);
            logger::warning("server: bind", strerror(errno));
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        logger::error("server: failed to bind", "");
        exit(1);
    }

    if (listen(this->_socket, BACKLOG) == -1)
    {
        perror("listen");
        logger::error("server: listen", strerror(errno));
        exit(1);
    }
    return (0);
}

void WebServer::handleRequest(int socket_fd)
{
    char buffer[3000];

    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    recv(socket_fd, buffer, 30000, 0);
    printf("%s\n", buffer);
    send(socket_fd, response.c_str(), response.length(), 0);
    printf("------------------Response sent-------------------\n");
};
