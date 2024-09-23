/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:21:54 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/23 09:23:46 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands.hpp"

// Constructor
Channel::Channel(const std::string &name) 
	: channelName(name), inviteOnly(false), topicRestricted(false), userLimit(0) {}

// Client Management
bool Channel::isClientInChannel(int clientFd) const
{
	return clients.find(clientFd) != clients.end();
}

void Channel::addClient(int clientFd)
{
	clients.insert(clientFd);
}

void Channel::removeClient(int clientFd)
{
	clients.erase(clientFd);
}

// Operator Management
bool Channel::isOperator(int clientFd) const
{
	return operators.find(clientFd) != operators.end();
}

void Channel::addOperator(int clientFd)
{
	operators.insert(clientFd);
}

void Channel::removeOperator(int clientFd)
{
	operators.erase(clientFd);
}

// Topic Management
void Channel::setTopic(const std::string &newTopic)
{
	topic = newTopic;
}

std::string Channel::getTopic() const
{
	return topic;
}

// Channel Mode Management
void Channel::setInviteOnly(bool value)
{
	inviteOnly = value;
}

bool Channel::isInviteOnly() const
{
	return inviteOnly;
}

void Channel::setTopicRestricted(bool value)
{
	topicRestricted = value;
}

bool Channel::isTopicRestricted() const
{
	return topicRestricted;
}

// Password Management
void Channel::setPassword(const std::string &pass)
{
	password = pass;
}

bool Channel::checkPassword(const std::string &pass) const
{
	return password == pass;
}

// User Limit Management
void Channel::setUserLimit(int limit)
{
	userLimit = limit;
}

bool Channel::isUserLimitExceeded() const
{
	return userLimit > 0 && clients.size() >= userLimit;
}
