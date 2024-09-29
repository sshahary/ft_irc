/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:21:54 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/29 15:37:21 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands.hpp"
#include "../../include/server.hpp"

// Constructor
Channel::Channel(const std::string &name)
	: channelName(name), userLimit(0), inviteOnly(false), topicRestricted(false), clients() {}

// Client Management
bool Channel::isClientInChannel(int clientFd) const
{
	return std::find(clients.begin(), clients.end(), clientFd) != clients.end();
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
	return userLimit > 0 && static_cast<int>(clients.size()) >= userLimit;
}

std::string Channel::getName() const
{
	return channelName;
}

Channel* Server::findChannelByName(const std::string& channelName)
{
	for (Channel* channel : channels)
	{  // Assuming 'channels' is a member of Server that holds all channels
		if (channel->getName() == channelName)
			return channel;		// Assuming Channel has a getName() method

	}
	return NULL;  // If the channel was not found, return nullptr
}

const std::set<int>& Channel::getClients() const
{
	return clients;
}

void Server::broadcastToChannel(Channel* channel, const std::string& message, int excludeFd)
{
	for (int clientFd : channel->getClients()) {
		if (clientFd != excludeFd) {
			sendToClient(clientFd, message);
		}
	}
}

void Server::sendToClient(int clientFd, const std::string& message)
{
	send(clientFd, message.c_str(), message.length(), 0);
}

std::string Server::getClientNickname(int clientFd) const
{
	for (const auto& pair : nicknames)
	{
		if (pair.second == clientFd)
			return pair.first;  // Return the nickname (the key)

	}
	return "Unknown";
}

std::string Server::getChannelUsers(Channel* channel) const
{
	return channel->getUsersList(*this);  // Call getUsersList from Channel
}

bool Channel::hasClient(int clientFd) const
{
	return clients.find(clientFd) != clients.end();
}
