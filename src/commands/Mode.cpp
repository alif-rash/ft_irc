/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:09 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:09 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"
#include <cstdlib>

void handleMode(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
        return;
    }

    std::string channelName = params[0];
    Channel *channel = server.getChannel(channelName);

    if (!channel)
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }

    if (params.size() == 1)
    {
        client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channelName, channel->getModesString()));
        return;
    }

    if (!channel->isOperator(&client))
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    std::string modeString = params[1];
    bool adding = true;
    size_t paramIdx = 2;

    for (size_t i = 0; i < modeString.length(); ++i)
    {
        char c = modeString[i];
        if (c == '+')
            adding = true;
        else if (c == '-')
            adding = false;
        else if (c == 'i')
            channel->setInviteOnly(adding);
        else if (c == 't')
            channel->setTopicRestricted(adding);
        else if (c == 'k')
        {
            if (adding && paramIdx < params.size())
                channel->setKey(params[paramIdx++]);
            else if (!adding)
                channel->removeKey();
        }
        else if (c == 'l')
        {
            if (adding && paramIdx < params.size())
                channel->setLimit(std::atoi(params[paramIdx++].c_str()));
            else if (!adding)
                channel->removeLimit();
        }
        else if (c == 'o')
        {
            if (paramIdx < params.size())
            {
                Client *target = server.getClientByNick(params[paramIdx++]);
                if (target && channel->isMember(target))
                {
                    if (adding)
                        channel->addOperator(target);
                    else
                        channel->removeOperator(target);
                }
            }
        }
    }

    std::string modeBroadcast = ":" + client.getPrefix() + " MODE " + channelName + " " + modeString + "\r\n";
    channel->broadcast(modeBroadcast);
}