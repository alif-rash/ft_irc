/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:18:25 by raalifa           #+#    #+#             */
/*   Updated: 2026/09/09 13:18:25 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Reply.hpp"
#include <cstdlib>

static bool isValidNick(const std::string &nickname)
{
    if (nickname.empty() || nickname.length() > 9)
        return false;

    std::string allowedSpecial = "[]\\`_^{}|";
    if (!std::isalpha(nickname[0]) && allowedSpecial.find(nickname[0]) == std::string::npos)
        return false;

    for (size_t i = 1; i < nickname.length(); ++i)
    {
        if (!std::isalnum(nickname[i]) && allowedSpecial.find(nickname[i]) == std::string::npos && nickname[i] != '-')
            return false;
    }
    return true;
}

//authentication & registration handlers

void handlePass(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (client.isRegistered())
    {
        client.sendMessage(Reply::ERR_ALREADYREGISTRED(client.getNickname()));
        return;
    }
    if (params.empty())
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
        return;
    }

    std::string providedPass = params[0];
    if (providedPass != server.getPassword())
    {
        client.sendMessage(Reply::ERR_PASSWDMISMATCH(client.getNickname()));
        return;
    }

    client.setPassOk(true);
    if (!client.isRegistered() && !client.getNickname().empty() && !client.getUsername().empty())
    {
        client.setRegistered(true);
        client.sendMessage(Reply::RPL_WELCOME(client.getNickname(), client.getPrefix()));
    }
}

void handleNick(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(Reply::ERR_NONICKNAMEGIVEN(client.getNickname()));
        return;
    }

    std::string newNick = params[0];

    if (!isValidNick(newNick))
    {
        client.sendMessage(Reply::ERR_ERRONEUSNICKNAME(client.getNickname(), newNick));
        return;
    }

    Client *existingClient = server.getClientByNick(newNick);
    if (existingClient && existingClient != &client)
    {
        client.sendMessage(Reply::ERR_NICKNAMEINUSE(client.getNickname(), newNick));
        return;
    }

    std::string oldNick = client.getNickname();
    client.setNickname(newNick);

    // If client is already fully registered, broadcast nickname change
    if (client.isRegistered())
    {
        std::string nickChangeMsg = ":" + oldNick + "!" + client.getUsername() + "@localhost NICK :" + newNick + "\r\n";
        client.sendMessage(nickChangeMsg);
    }
    // If unregistered, check if NICK completes the PASS + NICK + USER sequence
    else if (client.isPassOk() && !client.getUsername().empty())
    {
        client.setRegistered(true);
        client.sendMessage(Reply::RPL_WELCOME(client.getNickname(), client.getPrefix()));
    }
}

void handleUser(Server &, Client &client, const std::vector<std::string> &params)
{
    if (client.isRegistered())
    {
        client.sendMessage(Reply::ERR_ALREADYREGISTRED(client.getNickname()));
        return;
    }
    if (params.size() < 4)
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
        return;
    }

    client.setUsername(params[0]);
    client.setRealname(params[3]);

    // Check if USER completes the PASS + NICK + USER sequence
    if (client.isPassOk() && !client.getNickname().empty())
    {
        client.setRegistered(true);
        client.sendMessage(Reply::RPL_WELCOME(client.getNickname(), client.getPrefix()));
    }
}

void handleInvite(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.size() < 2)
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }

    std::string targetNick = params[0];
    std::string channelName = params[1];

    Channel *channel = server.getChannel(channelName);
    if (!channel)
    {
        client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isMember(&client))
    {
        client.sendMessage(Reply::ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (channel->isInviteOnly() && !channel->isOperator(&client))
    {
        client.sendMessage(Reply::ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    Client *targetClient = server.getClientByNick(targetNick);
    if (!targetClient)
    {
        client.sendMessage(Reply::ERR_NOSUCHNICK(client.getNickname(), targetNick));
        return;
    }
    if (channel->isMember(targetClient))
    {
        client.sendMessage(Reply::ERR_USERONCHANNEL(client.getNickname(), targetNick, channelName));
        return;
    }

    channel->addInvite(targetNick);
    client.sendMessage(Reply::RPL_INVITING(client.getNickname(), targetNick, channelName));

    std::string inviteNotice = ":" + client.getPrefix() + " INVITE " + targetNick + " :" + channelName + "\r\n";
    targetClient->sendMessage(inviteNotice);
}

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

    client.sendMessage(Reply::RPL_NAMREPLY(client.getNickname(), channelName, channel->getNamesString()));
    client.sendMessage(Reply::RPL_ENDOFNAMES(client.getNickname(), channelName));
}

void handleKick(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.size() < 2)
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
        return;
    }

    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : client.getNickname();

    Channel *channel = server.getChannel(channelName);
    if (!channel)
    {
        client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isMember(&client))
    {
        client.sendMessage(Reply::ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isOperator(&client))
    {
        client.sendMessage(Reply::ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    Client *targetClient = server.getClientByNick(targetNick);
    if (!targetClient || !channel->isMember(targetClient))
    {
        client.sendMessage(Reply::ERR_USERNOTINCHANNEL(client.getNickname(), targetNick, channelName));
        return;
    }

    std::string kickMsg = ":" + client.getPrefix() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    channel->broadcast(kickMsg);
    channel->removeMember(targetClient);

    if (channel->getMemberCount() == 0)
        server.removeChannel(channelName);
}

void handleMode(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
        return;
    }

    std::string channelName = params[0];
    Channel *channel = server.getChannel(channelName);

    if (!channel)
    {
        client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }

    if (params.size() == 1)
    {
        client.sendMessage(Reply::RPL_CHANNELMODEIS(client.getNickname(), channelName, channel->getModesString()));
        return;
    }

    if (!channel->isOperator(&client))
    {
        client.sendMessage(Reply::ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
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

    std::string modeBroadcast = ":" + client.getPrefix() + " MODE " + channelName + " " + modeString;
    for (size_t i = 2; i < params.size(); ++i)
        modeBroadcast += " " + params[i];
    modeBroadcast += "\r\n";
    channel->broadcast(modeBroadcast);
}

void handlePart(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
        return;
    }

    std::string channelName = params[0];
    std::string reason = (params.size() > 1) ? params[1] : "Leaving";

    Channel *channel = server.getChannel(channelName);
    if (!channel)
    {
        client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isMember(&client))
    {
        client.sendMessage(Reply::ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return;
    }

    std::string partMsg = ":" + client.getPrefix() + " PART " + channelName + " :" + reason + "\r\n";
    channel->broadcast(partMsg);
    channel->removeMember(&client);

    if (channel->getMemberCount() == 0)
        server.removeChannel(channelName);
}

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

void handleTopic(Server &server, Client &client, const std::vector<std::string> &params)
{
    if (params.empty())
    {
        client.sendMessage(Reply::ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
        return;
    }

    std::string channelName = params[0];
    Channel *channel = server.getChannel(channelName);

    if (!channel)
    {
        client.sendMessage(Reply::ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }
    if (!channel->isMember(&client))
    {
        client.sendMessage(Reply::ERR_NOTONCHANNEL(client.getNickname(), channelName));
        return;
    }

    if (params.size() == 1)
    {
        if (channel->getTopic().empty())
            client.sendMessage(Reply::RPL_NOTOPIC(client.getNickname(), channelName));
        else
            client.sendMessage(Reply::RPL_TOPIC(client.getNickname(), channelName, channel->getTopic()));
        return;
    }

    if (channel->isTopicRestricted() && !channel->isOperator(&client))
    {
        client.sendMessage(Reply::ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    std::string newTopic = params[1];
    channel->setTopic(newTopic);

    std::string topicMsg = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel->broadcast(topicMsg);
}
