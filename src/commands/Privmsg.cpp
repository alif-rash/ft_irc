/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:20 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:20 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"

void handlePrivmsg(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.size() < 2)
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
        return;
    }

    std::string target = params[0];
    std::string text = params[1];
    std::string formattedMsg = ":" + client.getPrefix() + " PRIVMSG " + target + " :" + text + "\r\n";

    if (target[0] == '#')
    {
        Channel *channel = server.getChannel(target);
        if (!channel)
        {
            client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), target));
            return;
        }
        if (!channel->isMember(&client))
        {
            client.sendMessage(Reply::ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
            return;
        }
        channel->broadcast(formattedMsg, &client);
    }
    else
    {
        Client *targetClient = server.getClientByNick(target);
        if (!targetClient)
        {
            client.sendMessage(Reply::ERR_NOSUCHNICK(client.getNickname(), target));
            return;
        }
        targetClient->sendMessage(formattedMsg);
    }
}