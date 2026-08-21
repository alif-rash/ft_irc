/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:33:54 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:33:54 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include "Client.hpp"
class Server
{
    private:
        int _serverFd;
        int _port;
        std::vector<struct pollfd> _pollFds;
        std::map<int, Client> _clients;
        void handleMessage(Client &client, const std::string &message);
        void acceptClient();
        void receiveMessage(size_t index);
        void handleDisconnect(size_t index);
    public:
        Server();
        ~Server();
        void run();
};

#endif
