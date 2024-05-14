/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:06:07 by jatan             #+#    #+#             */
/*   Updated: 2024/05/15 00:47:39 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Constructor for WebServer class.
 *
 * @param port The port number to listen on.
 * @param hostname The hostname to bind to.
 * @param server The name of the server.
 */
WebServer::WebServer(std::vector<ServerBlock> server_blocks) : _logger(Logger("Webserver"))
{
    // createSocket(port, hostname);
    _serverBlocks = server_blocks;
    for (std::vector<ServerBlock>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); it++)
    {
        _server_sockets[it->getPort()] = createSocket(std::to_string(it->getPort()), it->getServerName());
    }
}

WebServer::~WebServer()
{
}

void WebServer::start() {
    while (1) {
        configureSelect();
        int activity = select(_max_fd + 1, &_read_fds, &_write_fds, NULL, NULL);

        if (activity < 0)
        {
            perror("select");
            exit(1);
        }
        handleConnections();
    }

}

void WebServer::setupServerSocket(int port)
{
    int server_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        exit(1);
    }

    // socketfd for accepting incoming connections, and backlog for queueing incoming connections
    if (listen(server_socket, 10) < 0)
    {
        perror("listen");
        exit(1);
    }

    setNonBlocking(server_socket);
}

void WebServer::setNonBlocking(int sock_fd)
{
    int opts = fcntl(sock_fd, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(F_GETFL)");
        exit(1);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock_fd, F_SETFL, opts) < 0)
    {
        perror("fcntl(F_SETFL)");
        exit(1);
    }
}

void WebServer::configureSelect(void)
{
    FD_ZERO(&_read_fds);
    FD_ZERO(&_write_fds);
    _max_fd = 0;

    for (std::map<int, int>::iterator it = _server_sockets.begin(); it != _server_sockets.end(); it++)
    {
        FD_SET(it->first, &_read_fds);
        if (it->first > _max_fd)
        {
            _max_fd = it->first;
        }
    }

    for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end(); it++)
    {
        FD_SET(*it, &_read_fds);
        FD_SET(*it, &_write_fds);
        if (*it > _max_fd)
        {
            _max_fd = *it;
        }
    }
}

void WebServer::handleConnections()
{
    for (std::map<int, int>::iterator it = _server_sockets.begin(); it != _server_sockets.end(); it++)
    {
        if (FD_ISSET(it->first, &_read_fds))
        {
            acceptNewConnection(it->first);
        }
        it++;
    }
    for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end(); it++)
    {
        bool close_conn = false;
        if (FD_ISSET(*it, &_read_fds))
        {
            // handle read
            // close_conn = _connections[*it].readData();
        }
        if (FD_ISSET(*it, &_write_fds))
        {
            // handle write
        }

        if (close_conn)
        {
            close(*it);
            it = _client_sockets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void WebServer::acceptNewConnection(int server_socket)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0)
    {
        perror("accept");
    }
    else
    {
        setNonBlocking(client_socket);
        _client_sockets.push_back(client_socket);
    }
}

bool WebServer::echoMessage(int sock)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE - 1);

    if (bytes_read == -1)
    {
        perror("read");
        return false;
    }
    else if (bytes_read == 0)
    {
        return false; // Connection closed by client
    }

    send(sock, buffer, bytes_read, 0); // Echo back to the client
    return true;
}

/**
 * @brief Starts the web server and begins listening for incoming connections.
 *
 * This function initializes the server socket, binds it to the specified port,
 * and begins listening for incoming connections. Once a connection is established,
 * the server will create a new thread to handle the incoming request.
 *
 * @return void
 */
// void WebServer::start()
// {
//     // int connectSocket;
//     // struct sockaddr_in address;
//     std::stringstream ss;
//     fd_set master;   // master file descriptor list
//     fd_set read_fds; // temp file descriptor list for select()
//     int fdmax;       // maximum file descriptor number

//     // int listener;                       // listening socket descriptor

//     char buf[1024]; // buffer for client data
//     int nbytes;

//     int i;

//     FD_ZERO(&master); // clear the master and temp sets
//     FD_ZERO(&read_fds);

//     // add the listener to the master set
//     FD_SET(_listener, &master);

//     // keep track of the biggest file descriptor
//     fdmax = _listener; // so far, it's this one
//     // int offset = -_listener - 2;

//     // main loop
//     for (;;)
//     {
//         read_fds = master; // copy it
//         if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
//         {
//             perror("select");
//             exit(4);
//         }

//         // run through the existing connections looking for data to read
//         for (i = 0; i <= fdmax; i++)
//         {
//             if (FD_ISSET(i, &read_fds))
//             { // we got one!!
//                 if (i == _listener)
//                 {
//                     // handle new connections
//                     acceptConnection(master, fdmax);
//                 }
//                 else
//                 {
//                     // handle data from a client
//                     if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
//                     {
//                         // got error or connection closed by client
//                         if (nbytes == 0)
//                         {
//                             // connection closed
//                             _logger.error("selectserver: socket " + std::to_string(i) + " hung up");
//                         }
//                         else
//                         {
//                             perror("recv");
//                         }
//                         close(i); // bye!
//                         _logger.error("closed on socket " + std::to_string(i));
//                         _connections.erase(i);
//                         FD_CLR(i, &master); // remove from master set
//                     }
//                     else
//                     {
//                         _connections[i].readData(buf);
//                         // we got some data from a client
//                         // pass the data to connection to handle the request
//                         // and send
//                     }
//                 } // END handle data from client
//             } // END got new incoming connection
//         } // END looping through file descriptors
//     } // END for(;;)--and you thought it would never end!

//     return;
// }

/**
 * @brief Creates a new socket for the web server to listen on.
 *
 * @return The file descriptor of the newly created socket, or -1 if an error occurred.
 */
int WebServer::createSocket(std::string port, std::string hostname)
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
    const char *hostname_c = hostname.c_str();
    const char *port_c = port.c_str();
    if ((rv = getaddrinfo(hostname_c, port_c, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // delete hostname_c;
    // delete port_c;

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((_listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            _logger.warning(strerror(errno));
            continue;
        }

        if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            _logger.error(strerror(errno));
            // exit(1);
            throw std::runtime_error(strerror(errno));
        }

        if (bind(_listener, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_listener);
            _logger.warning(strerror(errno));
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        _logger.error("failed to bind");
        exit(1);
    }

    if (listen(_listener, BACKLOG) == -1)
    {
        perror("listen");
        _logger.error(strerror(errno));
        exit(1);
    }

    _logger.log("Server started on port " + port);
    return (0);
}