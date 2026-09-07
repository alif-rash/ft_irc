/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raalifa <raalifa@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:33:36 by raalifa           #+#    #+#             */
/*   Updated: 2026/08/17 16:33:36 by raalifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <set>

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        size_t _limit;

        bool _inviteOnly;
        bool _topicRestricted;
        bool _hasKey;
        bool _hasLimit;

        std::vector<Client *> _members;
        std::vector<Client *> _operators;
        std::set<std::string> _invitedNicknames;

    public:
        Channel(const std::string &name);
        Channel(const std::string &name, const std::string &key);
        ~Channel();

        const std::string &getName() const;
        const std::string &getTopic() const;
        const std::string &getKey() const;
        size_t getLimit() const;
        size_t getMemberCount() const;
        const std::vector<Client *> &getMembers() const;
        const std::vector<Client *> &getOperators() const;

        bool isInviteOnly() const;
        bool isTopicRestricted() const;
        bool hasKey() const;
        bool hasLimit() const;
        std::string getModesString() const;

        void setTopic(const std::string &topic);
        void setKey(const std::string &key);
        void removeKey();
        void setLimit(size_t limit);
        void removeLimit();
        void setInviteOnly(bool value);
        void setTopicRestricted(bool value);

        void addMember(Client *client);
        void removeMember(Client *client);
        bool isMember(Client *client) const;
        bool isMember(const std::string &nickname) const;

        void addOperator(Client *client);
        void removeOperator(Client *client);
        bool isOperator(Client *client) const;

        void addInvite(const std::string &nickname);
        void removeInvite(const std::string &nickname);
        bool isInvited(const std::string &nickname) const;

        void broadcast(const std::string &message, Client *sender = NULL);
        std::string getNamesString() const;
};

#endif
