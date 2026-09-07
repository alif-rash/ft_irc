/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:33:51 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:33:51 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>

namespace Reply
{
    // Numeric Replies (RPL)
    inline std::string RPL_WELCOME(const std::string &nick, const std::string &prefix)
    {
        return "001 " + nick + " :Welcome to the FT_IRC Network " + prefix + "\r\n";
    }

    inline std::string RPL_TOPIC(const std::string &nick, const std::string &channel, const std::string &topic)
    {
        return "332 " + nick + " " + channel + " :" + topic + "\r\n";
    }

    inline std::string RPL_NOTOPIC(const std::string &nick, const std::string &channel)
    {
        return "331 " + nick + " " + channel + " :No topic is set\r\n";
    }

    inline std::string RPL_NAMREPLY(const std::string &nick, const std::string &channel, const std::string &names)
    {
        return "353 " + nick + " = " + channel + " :" + names + "\r\n";
    }

    inline std::string RPL_ENDOFNAMES(const std::string &nick, const std::string &channel)
    {
        return "366 " + nick + " " + channel + " :End of /NAMES list\r\n";
    }

    inline std::string RPL_INVITING(const std::string &nick, const std::string &target, const std::string &channel)
    {
        return "341 " + nick + " " + target + " " + channel + "\r\n";
    }

    inline std::string RPL_CHANNELMODEIS(const std::string &nick, const std::string &channel, const std::string &modes)
    {
        return "324 " + nick + " " + channel + " " + modes + "\r\n";
    }

    // Error Replies (ERR)
    inline std::string ERR_NOSUCHNICK(const std::string &nick, const std::string &target)
    {
        return "401 " + nick + " " + target + " :No such nick/channel\r\n";
    }

    inline std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel)
    {
        return "403 " + nick + " " + channel + " :No such channel\r\n";
    }

    inline std::string ERR_CANNOTSENDTOCHAN(const std::string &nick, const std::string &channel)
    {
        return "404 " + nick + " " + channel + " :Cannot send to channel\r\n";
    }

    inline std::string ERR_USERNOTINCHANNEL(const std::string &nick, const std::string &target, const std::string &channel)
    {
        return "441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n";
    }

    inline std::string ERR_NOTONCHANNEL(const std::string &nick, const std::string &channel)
    {
        return "442 " + nick + " " + channel + " :You're not on that channel\r\n";
    }

    inline std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &target, const std::string &channel)
    {
        return "443 " + nick + " " + target + " " + channel + " :is already on channel\r\n";
    }

    inline std::string ERR_NEEDMOREPARAMS(const std::string &nick, const std::string &command)
    {
        return "461 " + nick + " " + command + " :Not enough parameters\r\n";
    }

    inline std::string ERR_BADCHANNELKEY(const std::string &nick, const std::string &channel)
    {
        return "475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
    }

    inline std::string ERR_CHANNELISFULL(const std::string &nick, const std::string &channel)
    {
        return "471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
    }

    inline std::string ERR_INVITEONLYCHAN(const std::string &nick, const std::string &channel)
    {
        return "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
    }

    inline std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &channel)
    {
        return "482 " + nick + " " + channel + " :You're not channel operator\r\n";
    }
}

#endif
