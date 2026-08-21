/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:33:43 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/18 22:24:17 by raalifa          ###   ########.fr       */
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
    public:
        Client(int fd);
        ~Client();
        int getFd() const;
        
        void appendToBuffer(const char *data, size_t size);
        void sendMessage(const std::string &message);
        
        bool hasCompleteMessage() const;
        std::string getNextMessage();
};

#endif
