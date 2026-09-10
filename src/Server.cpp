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
#include <csignal>
#include <cctype>
#include <stdexcept>
#include "Parser.hpp"
#include "Commands.hpp"
#include "Reply.hpp"
volatile sig_atomic_t g_running = 1;

static bool isSupportedCommand(const std::string &command)
{
    static const char *commands[] = {
        "CAP", "PING", "PONG", "QUIT", "PASS", "NICK", "USER",
        "INVITE", "JOIN", "KICK", "MODE", "PART", "PRIVMSG", "TOPIC"
    };
    for (size_t i = 0; i < 14; ++i)
    {
        if (command == commands[i])
            return true;
    }
    return false;
}

static bool isRegistrationCommand(const std::string &command)
{
    return command == "PASS" || command == "NICK" || command == "USER"
        || command == "CAP" || command == "PING" || command == "PONG"
        || command == "QUIT";
}

Server::Server(int port, const std::string &password) : _serverFd(-1), _port(port), _password(password)
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd == -1)
        throw std::runtime_error("Could not create server socket");
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1)
    {
        close(_serverFd);
        throw std::runtime_error("Could not configure server socket");
    }
    fcntl(_serverFd, F_SETFL, O_NONBLOCK);
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        close(_serverFd);
        throw std::runtime_error("Could not bind server socket");
    }
    if (listen(_serverFd, 10) == -1)
    {
        close(_serverFd);
        throw std::runtime_error("Could not listen on server socket");
    }
    std::cout << "Server listening on port " << _port << std::endl;
    struct pollfd serverPoll;
    serverPoll.fd = _serverFd;
    serverPoll.events = POLLIN;
    _pollFds.push_back(serverPoll);
}

const std::string &Server::getPassword() const
{
    return _password;
}

void Server::acceptClient()
{
    int clientFd = accept(_serverFd, NULL, NULL);
    if (clientFd == -1)
        return;
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    struct pollfd clientPoll;
    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    _pollFds.push_back(clientPoll);
    _clients.insert(std::make_pair(clientFd, Client(clientFd)));
    std::cout << "Client connected: FD " << clientFd << std::endl;
}

bool Server::receiveMessage(size_t index)
{
    std::map<int, Client>::iterator it = _clients.find(_pollFds[index].fd);
    if (it == _clients.end())
        return false;
    Client &client = it->second;
    char buffer[512];
    int bytesReceived = recv(_pollFds[index].fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';
        client.appendToBuffer(buffer, bytesReceived);
        while (client.hasCompleteMessage())
        {
            std::string message = client.getNextMessage();
            int clientFd = client.getFd();
            handleMessage(client, message);
            if (_clients.find(clientFd) == _clients.end())
                return true;
        }
    }
    else if (bytesReceived == 0)
    {
        handleDisconnect(index);
        return true;
    }
    else
        return false;
    return false;
}

void Server::handleDisconnect(size_t index)
{
    int clientFd = _pollFds[index].fd;
    std::map<int, Client>::iterator clientIt = _clients.find(clientFd);
    if (clientIt != _clients.end())
    {
        Client *client = &clientIt->second;
        for (std::map<int, Channel>::iterator it = _channels.begin(); it != _channels.end();)
        {
            it->second.removeMember(client);
            if (it->second.getMemberCount() == 0)
            {
                std::map<int, Channel>::iterator channelIt = it++;
                _channels.erase(channelIt);
            }
            else
                ++it;
        }
        _clients.erase(clientIt);
    }
    close(clientFd);
    _pollFds.erase(_pollFds.begin() + index);
    std::cout << "Client disconnected: FD " << clientFd << std::endl;
}

void Server::run()
{
    while (g_running)
    {
        int result = poll(_pollFds.data(), _pollFds.size(), -1);
        if (result == -1)
        {
            if (!g_running)
                break;
            throw std::runtime_error("Poll failed");
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
            if (i == 0)
            {
                if (_pollFds[i].revents & POLLIN)
                    acceptClient();
                continue;
            }
            else
            {
                if (_pollFds[i].revents & POLLIN)
                {
                    if (receiveMessage(i))
                    {
                        i--;
                        continue;
                    }
                }
                if (_pollFds[i].revents & POLLOUT)
                {
                    std::map<int, Client>::iterator it = _clients.find(_pollFds[i].fd);
                    if (it != _clients.end())
                    {
                        it->second.sendPendingData();
                        if (!it->second.hasPendingData())
                            _pollFds[i].events &= ~POLLOUT;
                    }
                }
            }

        }
    }
}

void Server::handleMessage(Client &client, const std::string &message)
{
    std::vector<std::string> tokens = Parser::parseMessage(message);
    if (tokens.empty())
        return;
    std::string command = tokens[0];
    
    for (size_t i = 0; i < command.length(); ++i)
        command[i] = std::toupper(static_cast<unsigned char>(command[i]));
    tokens[0] = command;
    std::vector<std::string> params(tokens.begin() + 1, tokens.end());
    if (!isSupportedCommand(command))
    {
        client.sendMessage(Reply::ERR_UNKNOWNCOMMAND(client.getNickname(), command));
        enableWrite(client);
        return;
    }
    if (!client.isRegistered() && !isRegistrationCommand(command))
    {
        client.sendMessage(Reply::ERR_NOTREGISTERED(client.getNickname()));
        enableWrite(client);
        return;
    }
    if (command == "CAP")
    {
        if (!params.empty() && params[0] == "LS")
            client.sendMessage("CAP * LS :\r\n");
        else if (!params.empty() && params[0] == "REQ" && params.size() > 1)
            client.sendMessage("CAP * NAK :" + params[1] + "\r\n");
    }
    else if (command == "PING")
    {
        std::string token = params.empty() ? "" : " " + params[0];
        client.sendMessage("PONG" + token + "\r\n");
    }
    else if (command == "PONG")
    {
    }
    else if (command == "QUIT")
    {
        for (size_t i = 1; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].fd == client.getFd())
            {
                handleDisconnect(i);
                break;
            }
        }
        return;
    }
    else if (command == "PASS")
        handlePass(*this, client, params);
    else if (command == "NICK")
        handleNick(*this, client, params);
    else if (command == "USER")
        handleUser(*this, client, params);
    else if (command == "INVITE")
        handleInvite(*this, client, params);
    else if (command == "JOIN")
        handleJoin(*this, client, params);
    else if (command == "KICK")
        handleKick(*this, client, params);
    else if (command == "MODE")
        handleMode(*this, client, params);
    else if (command == "PART")
        handlePart(*this, client, params);
    else if (command == "PRIVMSG")
        handlePrivmsg(*this, client, params);
    else if (command == "TOPIC")
        handleTopic(*this, client, params);
    else
        client.sendMessage(Reply::ERR_UNKNOWNCOMMAND(client.getNickname(), command));
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.hasPendingData())
            enableWrite(it->second);
    }
}

void Server::enableWrite(Client &client)
{
    for (size_t  i = 1; i < _pollFds.size();++i)
    {
        if (_pollFds[i].fd == client.getFd())
        {
            _pollFds[i].events |= POLLOUT;
            return;
        }
    }
}

Channel *Server::getChannel(const std::string &name)
{
    for (std::map<int, Channel>::iterator it = _channels.begin();
         it != _channels.end(); ++it)
    {
        if (it->second.getName() == name)
            return &(it->second);
    }
    return NULL;
}

Channel &Server::createChannel(const std::string &name)
{
    int id = _channels.size();
    _channels.insert(std::make_pair(id, Channel(name)));
    return _channels.find(id)->second;
}

void Server::removeChannel(const std::string &name)
{
    for (std::map<int, Channel>::iterator it = _channels.begin();
         it != _channels.end(); ++it)
    {
        if (it->second.getName() == name)
        {
            _channels.erase(it);
            return;
        }
    }
}

Client *Server::getClientByNick(const std::string &nickname)
{
    for (std::map<int, Client>::iterator it = _clients.begin();
         it != _clients.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
            return &(it->second);
    }
    return NULL;
}

Server::~Server()
{
    for (std::map<int, Client>::iterator it = _clients.begin();
         it != _clients.end(); ++it)
    {
        close(it->first);
    }

    close(_serverFd);
}

void handleSignal(int signal)
{
    if (signal == SIGINT)
    {
        const char message[] = "\nShutting down ...\n";
        write(STDOUT_FILENO, message, sizeof(message) - 1);
        g_running = 0;
    }
}