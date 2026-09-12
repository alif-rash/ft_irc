/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:33:51 by raalifa           #+#    #+#             */
/*   Updated: 2026/09/12 16:01:09 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>

class Reply
{
    private:
        Reply();

    public:
        // Numeric Replies (RPL)
        static std::string RPL_WELCOME(const std::string &nickname, const std::string &prefix)
        {
            return ":ft_irc 001 " + nickname + " :Welcome to FT_IRC Network " + prefix + "\r\n";
        }

        static std::string RPL_CHANNELMODEIS(const std::string &nickname, const std::string &channel, const std::string &modes)
        {
            return ":ft_irc 324 " + nickname + " " + channel + " " + modes + "\r\n";
        }
        
        static std::string RPL_NOTOPIC(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 331 " + nickname + " " + channel + " :No topic is set\r\n";
        }

        static std::string RPL_TOPIC(const std::string &nickname, const std::string &channel, const std::string &topic)
        {
            return ":ft_irc 332 " + nickname + " " + channel + " :" + topic + "\r\n";
        }
        
        static std::string RPL_INVITING(const std::string &nickname, const std::string &target, const std::string &channel)
        {
            return ":ft_irc 341 " + nickname + " " + target + " " + channel + "\r\n";
        }

        static std::string RPL_NAMREPLY(const std::string &nickname, const std::string &channel, const std::string &names)
        {
            return ":ft_irc 353 " + nickname + " = " + channel + " :" + names + "\r\n";
        }

        static std::string RPL_ENDOFNAMES(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 366 " + nickname + " " + channel + " :End of /NAMES list\r\n";
        }
        
        // Error Replies (ERR)
        static std::string ERR_NOSUCHNICK(const std::string &nickname, const std::string &target)
        {
            return ":ft_irc 401 " + nickname + " " + target + " :No such nick/channel\r\n";
        }

        static std::string ERR_NOSUCHCHANNEL(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 403 " + nickname + " " + channel + " :No such channel\r\n";
        }

        static std::string ERR_CANNOTSENDTOCHAN(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 404 " + nickname + " " + channel + " :Cannot send to channel\r\n";
        }

        static std::string ERR_UNKNOWNCOMMAND(const std::string &nickname, const std::string &command)
        {
            return ":ft_irc 421 " + nickname + " " + command + " :Unknown command\r\n";
        }

        static std::string ERR_NONICKNAMEGIVEN(const std::string &nickname)
        {
            return ":ft_irc 431 " + (nickname.empty() ? "*" : nickname) + " :No nickname given\r\n";
        }

        static std::string ERR_ERRONEUSNICKNAME(const std::string &nickname, const std::string &invalidNick)
        {
            return ":ft_irc 432 " + (nickname.empty() ? "*" : nickname) + " " + invalidNick + " :Erroneous nickname\r\n";
        }

        static std::string ERR_NICKNAMEINUSE(const std::string &nickname, const std::string &usedNick)
        {
            return ":ft_irc 433 " + (nickname.empty() ? "*" : nickname) + " " + usedNick + " :Nickname is already in use\r\n";
        }

        static std::string ERR_USERNOTINCHANNEL(const std::string &nickname, const std::string &target, const std::string &channel)
        {
            return ":ft_irc 441 " + nickname + " " + target + " " + channel + " :They aren't on that channel\r\n";
        }

        static std::string ERR_NOTONCHANNEL(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 442 " + nickname + " " + channel + " :You're not on that channel\r\n";
        }

        static std::string ERR_USERONCHANNEL(const std::string &nickname, const std::string &target, const std::string &channel)
        {
            return ":ft_irc 443 " + nickname + " " + target + " " + channel + " :is already on channel\r\n";
        }
        
        static std::string ERR_NOTREGISTERED(const std::string &nickname)
        {
            return ":ft_irc 451 " + (nickname.empty() ? "*" : nickname)+ " :You have not registered\r\n";
        }

        static std::string ERR_NEEDMOREPARAMS(const std::string &nickname, const std::string &command)
        {
            return ":ft_irc 461 " + nickname + " " + command + " :Not enough parameters\r\n";
        }

        static std::string ERR_ALREADYREGISTRED(const std::string &nickname)
        {
            return ":ft_irc 462 " + (nickname.empty() ? "*" : nickname) + " :Unauthorized command (already registered)\r\n";
        }

        static std::string ERR_PASSWDMISMATCH(const std::string &nickname)
        {
            return ":ft_irc 464 " + (nickname.empty() ? "*" : nickname) + " :Password incorrect\r\n";
        }

        static std::string ERR_CHANNELISFULL(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n";
        }

        static std::string ERR_INVITEONLYCHAN(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n";
        }

        static std::string ERR_BADCHANNELKEY(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n";
        }

        static std::string ERR_CHANOPRIVSNEEDED(const std::string &nickname, const std::string &channel)
        {
            return ":ft_irc 482 " + nickname + " " + channel + " :You're not channel operator\r\n";
        }
        
};

#endif

