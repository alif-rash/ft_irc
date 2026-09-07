/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:14 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:14 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"

void handlePart(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
        return;
    }

    std::string channelName = params[0];
    std::string reason = (params.size() > 1) ? params[1] : "Leaving";

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

    std::string partMsg = ":" + client.getPrefix() + " PART " + channelName + " :" + reason + "\r\n";
    channel->broadcast(partMsg);
    channel->removeMember(&client);

    if (channel->getMemberCount() == 0)
        server.removeChannel(channelName);
}