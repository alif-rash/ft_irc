/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajmoham <hajmoham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 18:06:31 by hajmoham          #+#    #+#             */
/*   Updated: 2026/09/07 18:06:31 by hajmoham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
class Client
{
    private:
        int _fd;
        std::string _receiveBuffer;
        std::string _sendBuffer;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _prefix;     // prefix for messages sent to the client
        bool _passOk;        // has the client sent the correct server password?
        bool _registered;    // has the client finished PASS + NICK + USER?
    public:
        Client(int fd);
        ~Client();
        int getFd() const;

        void appendToBuffer(const char *data, size_t size);
        void sendMessage(const std::string &message);
        void sendPendingData();
        bool hasPendingData() const;

        bool hasCompleteMessage() const;
        std::string getNextMessage();

        std::string getPrefix() const;

        // --- getters ---
        const std::string &getNickname() const;
        const std::string &getUsername() const;
        const std::string &getRealname() const;
        bool isPassOk() const;
        bool isRegistered() const;

        // --- setters ---
        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setRealname(const std::string &realname);
        void setPassOk(bool value);
        void setRegistered(bool value);

};
#endif
