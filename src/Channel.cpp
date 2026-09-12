/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:27 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:34:27 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(const std::string &name) :
    _name(name),
    _limit(0),
    _inviteOnly(false),
    _topicRestricted(false),
    _hasKey(false),
    _hasLimit(false)
{
}

Channel::Channel(const std::string &name, const std::string &key) :
    _name(name),
    _key(key),
    _limit(0),
    _inviteOnly(false),
    _topicRestricted(false),
    _hasKey(true),
    _hasLimit(false)
{
}

Channel::~Channel()
{
}

const std::string &Channel::getName() const
{
    return _name;
}

const std::string &Channel::getTopic() const
{
    return _topic;
}

const std::string &Channel::getKey() const
{
    return _key;
}

size_t Channel::getLimit() const
{
    return _limit;
}

size_t Channel::getMemberCount() const
{
    return _members.size();
}

const std::vector<Client *> &Channel::getMembers() const
{
    return _members;
}

const std::vector<Client *> &Channel::getOperators() const
{
    return _operators;
}

bool Channel::isInviteOnly() const
{
    return _inviteOnly;
}

bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

bool Channel::hasKey() const
{
    return _hasKey;
}

bool Channel::hasLimit() const
{
    return _hasLimit;
}

std::string Channel::getModesString() const
{
    std::string modes = "+";
    if (_inviteOnly)
        modes += "i";
    if (_topicRestricted)
        modes += "t";
    if (_hasKey)
        modes += "k";
    if (_hasLimit)
        modes += "l";
    return modes;
}

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
    _hasKey = true;
}

void Channel::removeKey()
{
    _key = "";
    _hasKey = false;
}

void Channel::setLimit(size_t limit)
{
    _limit = limit;
    _hasLimit = true;
}

void Channel::removeLimit()
{
    _limit = 0;
    _hasLimit = false;
}

void Channel::setInviteOnly(bool value)
{
    _inviteOnly = value;
}

void Channel::setTopicRestricted(bool value)
{
    _topicRestricted = value;
}

void Channel::addMember(Client *client)
{
    if (client && !isMember(client))
    {
        _members.push_back(client);
    }
}

void Channel::removeMember(Client *client)
{
    for (std::vector<Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (*it == client)
        {
            _members.erase(it);
            break;
        }
    }
    removeOperator(client);
}

bool Channel::isMember(Client *client) const
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] == client)
            return true;
    }
    return false;
}

bool Channel::isMember(const std::string &nickname) const
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i]->getNickname() == nickname)
            return true;
    }
    return false;
}

void Channel::addOperator(Client *client)
{
    if (client && !isOperator(client))
    {
        _operators.push_back(client);
    }
}

void Channel::removeOperator(Client *client)
{
    for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        if (*it == client)
        {
            _operators.erase(it);
            break;
        }
    }
}

bool Channel::isOperator(Client *client) const
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == client)
            return true;
    }
    return false;
}

void Channel::addInvite(const std::string &nickname)
{
    _invitedNicknames.insert(nickname);
}

void Channel::removeInvite(const std::string &nickname)
{
    _invitedNicknames.erase(nickname);
}

bool Channel::isInvited(const std::string &nickname) const
{
    return _invitedNicknames.find(nickname) != _invitedNicknames.end();
}

void Channel::broadcast(const std::string &message, Client *sender)
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] != sender)
        {
            _members[i]->sendMessage(message);
        }
    }
}

std::string Channel::getNamesString() const
{
    std::string names = "";
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (i > 0)
            names += " ";
        if (isOperator(_members[i]))
            names += "@";
        names += _members[i]->getNickname();
    }
    return names;
}
