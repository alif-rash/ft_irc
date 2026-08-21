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

void Server::acceptClient()
{
    int clientFd = accept(_serverFd, NULL, NULL);
    if (clientFd == -1)
    {
        perror("Accept failed");
        return;
    }
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    struct pollfd clientPoll;
    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    _pollFds.push_back(clientPoll);
    _clients.insert(std::make_pair(clientFd, Client(clientFd)));
}

void Server::receiveMessage(size_t index)
{
    std::map<int, Client>::iterator it = _clients.find(_pollFds[index].fd);
    if (it == _clients.end())
        return;
    Client &client = it->second;
    char buffer[512];
    int bytesreceived = recv(_pollFds[index].fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesreceived > 0)
    {
        buffer[bytesreceived] = '\0';
        client.appendToBuffer(buffer, bytesreceived);
        while (client.hasCompleteMessage())
        {
            std::string message = client.getNextMessage();
            handleMessage(client, message);
        }
    }
    else if (bytesreceived == 0)
        handleDisconnect(index);
    else if (errno != EAGAIN && errno != EWOULDBLOCK)
        std::cerr << "recv error " << errno << std::endl;
}

void Server::handleDisconnect(size_t index)
{
    close(_pollFds[index].fd);
    _clients.erase(_pollFds[index].fd);
    _pollFds.erase(_pollFds.begin() + index);
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
            if (_pollFds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            {
                if (i != 0)
                {
                    handleDisconnect(i);
                    i--;
                }
            continue;
            }
            if (!(_pollFds[i].revents & POLLIN))
                continue;
            if (i == 0)
                acceptClient();
            else
                receiveMessage(i);
        }
    }
}

void Server::handleMessage(Client &client, const std::string &message)
{
    std::cout << "Handling message from FD " << client.getFd() << ": " << message << std::endl;
    client.sendMessage("Message received\r\n");
}

Server::~Server()
{
    close(_serverFd);
}
