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
#include "Channel.hpp"

class Server
{
    private:
        int _serverFd;
        int _port;
        int _nextChannelId;
        std::string _password;
        std::vector<struct pollfd> _pollFds;
        std::map<int, Client> _clients;
        std::map<int, Channel> _channels;
        void handleMessage(Client &client, const std::string &message);
        bool dispatchCommand(Client &client, const std::string &command,
                     const std::vector<std::string> &params);
        void acceptClient();
        bool receiveMessage(size_t index);
        void handleDisconnect(size_t index);
        bool processPollEvent(size_t &index);
        bool handlePollError(size_t &index);
        bool handleServerSocket(size_t index);
        void handleClientOutput(size_t index);
    public:
        Server(int port, const std::string &password);
        ~Server();
        void run();

        const std::string &getPassword() const;

        Channel *getChannel(const std::string &name);
        Channel &createChannel(const std::string &name);
        void removeChannel(const std::string &name);
        Client *getClientByNick(const std::string &nickname);

        void enableWrite(Client &client);
};

#endif
