/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:44 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:44 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <cerrno>

Server::Server()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd == -1)
    {
        perror("Socket creation failed");
    }
    std::cout << "Server socket FD: " << _serverFd << std::endl;

    fcntl(_serverFd, F_SETFL, O_NONBLOCK);
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6667);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Bind failed");
        close(_serverFd);
        return;
    }
    if (listen(_serverFd, 10) == -1)
    {
        perror("Listen failed");
        close(_serverFd);
        return;
    }
    struct pollfd serverPoll;
    serverPoll.fd = _serverFd;
    serverPoll.events = POLLIN;
    _pollFds.push_back(serverPoll);
}

void Server::run()
{
    while (true)
    {
        int result = poll(_pollFds.data(), _pollFds.size(), -1);
        if (result == -1)
        {
            perror("Poll failed");
            break;
        }
        for (size_t i = 0; i < _pollFds.size(); i++)
        {
            if (!(_pollFds[i].revents & POLLIN))
                continue;
            
            if (i == 0)
            {
                int clientFd = accept(_serverFd, NULL, NULL);
                if (clientFd == -1)
                {
                    perror("Accept failed");
                    continue;
                }
                fcntl(clientFd, F_SETFL, O_NONBLOCK);
                std::cout << "Client connected! FD: " << clientFd << std::endl;
                struct pollfd clientPoll;
                clientPoll.fd = clientFd;
                clientPoll.events = POLLIN;
                clientPoll.revents = 0;
                _pollFds.push_back(clientPoll);
                _clients.insert(std::make_pair(clientFd, Client(clientFd)));
            }
            else
            {
                std::map<int, Client>::iterator it = _clients.find(_pollFds[i].fd);
                if (it == _clients.end())
                    continue;
                Client &client = it->second;
                char buffer[512];
                int bytesreceived = recv(_pollFds[i].fd, buffer, sizeof(buffer) -1 , 0);
                if (bytesreceived > 0)
                {
                    buffer[bytesreceived] = '\0';
                    std::cout << "Received from FD " << _pollFds[i].fd << ": " << buffer << std::endl;
                    client.appendToBuffer(buffer, bytesreceived);
                    if (client.hasCompleteMessage())
                       { std::string message = client.getNextMessage();
                        
                    else
                        std::cout << "Incomplete message recieved!!";
                }
                else if (bytesreceived == 0)
                {
                    std::cout << "client disconnected! FD: " << _pollFds[i].fd << std::endl;
                    close(_pollFds[i].fd);
                    _clients.erase(_pollFds[i].fd);
                    _pollFds.erase(_pollFds.begin() + i);
                    i--;
                }
                else
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                        std::cerr << "recv error " << errno << std::endl;
                }
            }
        }
    }
}


Server::~Server()
{
    close(_serverFd);
}
