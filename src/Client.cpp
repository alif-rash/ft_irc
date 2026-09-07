/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajmoham <hajmoham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 18:06:12 by hajmoham          #+#    #+#             */
/*   Updated: 2026/09/07 18:06:12 by hajmoham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>
#include <iostream>

Client::Client(int fd) : _fd(fd), _passOk(false), _registered(false)
{
}

Client::~Client()
{
}

int Client::getFd() const
{
    return _fd;
}

void Client::appendToBuffer(const char *data, size_t size)
{
   _receiveBuffer.append(data, size);
}

void Client::sendMessage(const std::string &message)
{
    send(_fd, message.c_str(), message.size(), 0);
}

bool Client::hasCompleteMessage() const
{
    return _receiveBuffer.find("\r\n") != std::string::npos;
}

std::string Client::getNextMessage()
{
    size_t pos = _receiveBuffer.find("\r\n");
    if (pos == std::string::npos)
    {
        return "";
    }
    std::string message = _receiveBuffer.substr(0, pos);
    _receiveBuffer.erase(0, pos + 2);
    return message;
}

// --- getters ---
const std::string &Client::getNickname() const { return _nickname; }
const std::string &Client::getUsername() const { return _username; }
const std::string &Client::getRealname() const { return _realname; }
bool Client::getPassOk() const { return _passOk; }
bool Client::isRegistered() const { return _registered; }

// --- setters ---
void Client::setNickname(const std::string &nick) { _nickname = nick; }
void Client::setUsername(const std::string &user) { _username = user; }
void Client::setRealname(const std::string &real) { _realname = real; }
void Client::setPassOk(bool value) { _passOk = value; }
void Client::setRegistered(bool value) { _registered = value; }