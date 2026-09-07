/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hajmoham <hajmoham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:00 by raalifa           #+#    #+#             */
/*   Updated: 2026/09/02 15:07:12 by hajmoham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"

void handleInvite(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }

    std::string targetNick = params[0];
    std::string channelName = params[1];

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
    if (channel->isInviteOnly() && !channel->isOperator(&client))
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    Client *targetClient = server.getClientByNick(targetNick);
    if (!targetClient)
    {
        client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), targetNick));
        return;
    }
    if (channel->isMember(targetClient))
    {
        client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), targetNick, channelName));
        return;
    }

    channel->addInvite(targetNick);
    client.sendMessage(RPL_INVITING(client.getNickname(), targetNick, channelName));

    std::string inviteNotice = ":" + client.getPrefix() + " INVITE " + targetNick + " :" + channelName + "\r\n";
    targetClient->sendMessage(inviteNotice);
}
