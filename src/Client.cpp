/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:31 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:31 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd)
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