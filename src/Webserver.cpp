/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jatan <jatan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:06:07 by jatan             #+#    #+#             */
/*   Updated: 2024/05/16 21:21:12 by jatan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Constructor for Webserver class.
 *
 * @param port The port number to listen on.
 * @param hostname The hostname to bind to.
 * @param server The name of the server.
 */
Webserver::Webserver(std::vector<ServerBlock*>& server_blocks) :  _serverBlocks(server_blocks), _logger(Logger("Webserver"))
{
    // createSocket(port, hostname);
    // _serverBlocks = server_blocks;
    for (std::vector<ServerBlock*>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); it++)
    {
        // this->_logger.log("ServerBlock: ");
        // it->display();
        setupServerSocket(**it);
    }
}

Webserver::~Webserver()
{
}

void Webserver::start() {
    while (1) {
        configureSelect();
        int activity = select(_maxFd + 1, &_readFds, &_writeFds, NULL, NULL);

        // std::cout << "Activity: " << activity << std::endl;
        // std::cout << _maxFd << std::endl;
        if (activity < 0)
        {
            perror("select");
            exit(1);
        }
        handleConnections();
    }

}

void Webserver::setupServerSocket(ServerBlock& serverBlock)
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
    address.sin_port = htons(serverBlock.listen);
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
    std::cout << serverBlock.listen << std::endl;
    _serverSockets[server_socket] = &serverBlock;

}

void Webserver::setNonBlocking(int sock_fd)
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

void Webserver::configureSelect(void)
{
    FD_ZERO(&_readFds);
    FD_ZERO(&_writeFds);
    _maxFd = 0;

    for (std::map<int, ServerBlock*>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
    {
        FD_SET(it->first, &_readFds);
        if (it->first > _maxFd)
        {
            _maxFd = it->first;
        }
    }

    for (std::vector<Connection*>::iterator it = _connections.begin(); it != _connections.end(); it++)
    {
        FD_SET((*it)->fd, &_readFds);
        FD_SET((*it)->fd, &_writeFds);
        if ((*it)->fd > _maxFd)
        {
            _maxFd = (*it)->fd;
        }
    }
}

void Webserver::handleConnections()
{
    for (std::map<int, ServerBlock*>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
    {
        if (FD_ISSET(it->first, &_readFds))
        {
            acceptNewConnection(it->first, it->second);
        }
    }

    for (std::vector<Connection*>::iterator it = _connections.begin(); it != _connections.end(); )
    {
        bool close_conn = false;
        int fd = (*it)->fd;
        if (FD_ISSET(fd, &_readFds))
        {
            // handle read
            // close_conn = _connections[*it].readData();
            if ((*it)->readData() == false)
            {
                close_conn = true;
            }
        }
        if (FD_ISSET((*it)->fd, &_writeFds))
        {
            // handle write
            (*it)->sendData();
        }

        if (close_conn)
        {
            close((*it)->fd);
            it = _connections.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Webserver::acceptNewConnection(int server_socket, ServerBlock* serverBlock)
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
        _connections.push_back(new Connection(client_socket, serverBlock));
    }
}

// void Webserver::handleRequest(int sock) {

// }


// bool Webserver::echoMessage(int sock)
// {
//     char buffer[BUFFER_SIZE];
//     memset(buffer, 0, BUFFER_SIZE);
//     ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE - 1);

//     if (bytes_read == -1)
//     {
//         perror("read");
//         return false;
//     }
//     else if (bytes_read == 0)
//     {
//         return false; // Connection closed by client
//     }

//     send(sock, buffer, bytes_read, 0); // Echo back to the client
//     return true;
// }
