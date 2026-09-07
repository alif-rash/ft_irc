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

// Numeric Replies (RPL)
    inline std::string RPL_WELCOME(const std::string &nickname, const std::string &prefix)
    {
        return "001 " + nickname + " :Welcome to the FT_IRC Network " + prefix + "\r\n";
    }

    inline std::string RPL_TOPIC(const std::string &nickname, const std::string &channel, const std::string &topic)
    {
        return "332 " + nickname + " " + channel + " :" + topic + "\r\n";
    }

    inline std::string RPL_NOTOPIC(const std::string &nickname, const std::string &channel)
    {
        return "331 " + nickname + " " + channel + " :No topic is set\r\n";
    }

    inline std::string RPL_NAMREPLY(const std::string &nickname, const std::string &channel, const std::string &names)
    {
        return "353 " + nickname + " = " + channel + " :" + names + "\r\n";
    }

    inline std::string RPL_ENDOFNAMES(const std::string &nickname, const std::string &channel)
    {
        return "366 " + nickname + " " + channel + " :End of /NAMES list\r\n";
    }

    inline std::string RPL_INVITING(const std::string &nickname, const std::string &target, const std::string &channel)
    {
        return "341 " + nickname + " " + target + " " + channel + "\r\n";
    }

    inline std::string RPL_CHANNELMODEIS(const std::string &nickname, const std::string &channel, const std::string &modes)
    {
        return "324 " + nickname + " " + channel + " " + modes + "\r\n";
    }

    // Error Replies (ERR)
    inline std::string ERR_NOSUCHNICK(const std::string &nickname, const std::string &target)
    {
        return "401 " + nickname + " " + target + " :No such nickname/channel\r\n";
    }

    inline std::string ERR_NOSUCHCHANNEL(const std::string &nickname, const std::string &channel)
    {
        return "403 " + nickname + " " + channel + " :No such channel\r\n";
    }

    inline std::string ERR_CANNOTSENDTOCHAN(const std::string &nickname, const std::string &channel)
    {
        return "404 " + nickname + " " + channel + " :Cannot send to channel\r\n";
    }

    inline std::string ERR_USERNOTINCHANNEL(const std::string &nickname, const std::string &target, const std::string &channel)
    {
        return "441 " + nickname + " " + target + " " + channel + " :They aren't on that channel\r\n";
    }

    inline std::string ERR_NOTONCHANNEL(const std::string &nickname, const std::string &channel)
    {
        return "442 " + nickname + " " + channel + " :You're not on that channel\r\n";
    }

    inline std::string ERR_USERONCHANNEL(const std::string &nickname, const std::string &target, const std::string &channel)
    {
        return "443 " + nickname + " " + target + " " + channel + " :is already on channel\r\n";
    }

    inline std::string ERR_NEEDMOREPARAMS(const std::string &nickname, const std::string &command)
    {
        return "461 " + nickname + " " + command + " :Not enough parameters\r\n";
    }

    inline std::string ERR_BADCHANNELKEY(const std::string &nickname, const std::string &channel)
    {
        return "475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n";
    }

    inline std::string ERR_CHANNELISFULL(const std::string &nickname, const std::string &channel)
    {
        return "471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n";
    }

    inline std::string ERR_INVITEONLYCHAN(const std::string &nickname, const std::string &channel)
    {
        return "473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n";
    }

    inline std::string ERR_CHANOPRIVSNEEDED(const std::string &nickname, const std::string &channel)
    {
        return "482 " + nickname + " " + channel + " :You're not channel operator\r\n";
    }
#endif
