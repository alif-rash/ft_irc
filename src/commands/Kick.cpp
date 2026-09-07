/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:05 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:05 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"

void handleKick(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
        return;
    }

    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : client.getNickname();

    Channel *channel = server.getChannel(channelName);
    if (!channel)
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isMember(&client))
    {
        client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isOperator(&client))
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    Client *targetClient = server.getClientByNick(targetNick);
    if (!targetClient || !channel->isMember(targetClient))
    {
        client.sendMessage(ERR_USERNOTINCHANNEL(client.getNickname(), targetNick, channelName));
        return;
    }

    std::string kickMsg = ":" + client.getPrefix() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    channel->broadcast(kickMsg);
    channel->removeMember(targetClient);

    if (channel->getMemberCount() == 0)
        server.removeChannel(channelName);
}