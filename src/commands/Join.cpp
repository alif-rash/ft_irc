/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:02 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:02 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"

void handleJoin(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return;
    }

    std::string channelName = params[0];
    std::string key = (params.size() > 1) ? params[1] : "";

    if (channelName.empty() || channelName[0] != '#')
    {
        client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }

    Channel *channel = server.getChannel(channelName);
    bool isNew = false;

    if (!channel)
    {
        channel = &server.createChannel(channelName);
        isNew = true;
    }

    if (!isNew)
    {
        if (channel->hasKey() && channel->getKey() != key)
        {
            client.sendMessage(Reply::ERR_BADCHANNELKEY(client.getNickname(), channelName));
            return;
        }
        if (channel->hasLimit() && channel->getMemberCount() >= channel->getLimit())
        {
            client.sendMessage(Reply::ERR_CHANNELISFULL(client.getNickname(), channelName));
            return;
        }
        if (channel->isInviteOnly() && !channel->isInvited(client.getNickname()))
        {
            client.sendMessage(Reply::ERR_INVITEONLYCHAN(client.getNickname(), channelName));
            return;
        }
    }

    channel->addMember(&client);
    if (isNew)
        channel->addOperator(&client);

    channel->removeInvite(client.getNickname());

    std::string joinMsg = ":" + client.getPrefix() + " JOIN :" + channelName + "\r\n";
    channel->broadcast(joinMsg);

    if (!channel->getTopic().empty())
        client.sendMessage(Reply::RPL_TOPIC(client.getNickname(), channelName, channel->getTopic()));
    else
        client.sendMessage(Reply::RPL_NOTOPIC(client.getNickname(), channelName));

    client.sendMessage(Reply::RPL_NAMREPLY(client.getNickname(), channelName, channel->getNamesList()));
    client.sendMessage(Reply::RPL_ENDOFNAMES(client.getNickname(), channelName));
}