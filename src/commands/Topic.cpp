/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:22 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:22 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"

void handleTopic(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
        return;
    }

    std::string channelName = params[0];
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

    if (params.size() == 1)
    {
        if (channel->getTopic().empty())
            client.sendMessage(RPL_NOTOPIC(client.getNickname(), channelName));
        else
            client.sendMessage(RPL_TOPIC(client.getNickname(), channelName, channel->getTopic()));
        return;
    }

    if (channel->isTopicRestricted() && !channel->isOperator(&client))
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    std::string newTopic = params[1];
    channel->setTopic(newTopic);

    std::string topicMsg = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel->broadcast(topicMsg);
}