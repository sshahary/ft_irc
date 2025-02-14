/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:58:11 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/20 19:05:28 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Channel.cpp
#include "Channel.hpp"
#include <set>
#include <sstream>
#include "IrcCommands.hpp" 
#include "Logger.hpp"  
#include "Client.hpp"  



Channel::Channel(const std::string& name)
	: _name(name),_topic(""), _inviteOnly(false), _key(""), _userLimit(-1), _topicRestricted(false), _topicTimestamp(0) {}

Channel::~Channel() {}

void Channel::addClient(Client* client)
{
	_clients[client->getFd()] = client;
	// Optionally, set the first client as operator
	if (_clients.size() == 1)
	{
		setOperator(client, true);
	}
}

void Channel::removeClient(Client* client)
{
	_clients.erase(client->getFd());
	_operators.erase(client->getFd());

	// Notify remaining clients
	std::string partMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "server_hostname" + " PART :" + _name + "\r\n";
	broadcastMessage(partMsg, client);
}

bool Channel::isEmpty() const
{
	return _clients.empty();
}

bool Channel::isMember(const Client* client) const
{
	return _clients.find(client->getFd()) != _clients.end();
}

void Channel::addInvite(Client* client)
{
	_invitedClients.insert(client);
}


void Channel::setTopicSetter(const std::string& setter)
{
	_topicSetter = setter;
}

std::string Channel::getTopicSetter() const
{
	return _topicSetter;
}

void Channel::setTopicTimestamp(time_t timestamp)
{
	_topicTimestamp = timestamp;
}

time_t Channel::getTopicTimestamp() const
{
	return _topicTimestamp;
}

void Channel::removeMember(Client* client)
{
	_clients.erase(client->getFd());
	_operators.erase(client->getFd());
	std::string partMsg = ":" + client->getNickname() + " PART " + _name + "\r\n";
	broadcastMessage(partMsg, client);
}

void Channel::broadcast(const std::string& message, Client* sender) {
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second != sender) {
			it->second->sendToClient(message);
		}
	}
}

const std::string& Channel::getName() const{ return _name;}

std::string Channel::getMemberNames() const
{
	std::ostringstream names;
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = it->second;
		// Check if the client is an operator
		if (isOperator(client)) {
			names << "@" << it->second->getNickname() << " ";  // Add @ for operators
		} else {
			names << it->second->getNickname() << " ";         // Regular clients without @
		}
	}
	return names.str();
}


void Channel::broadcastMessage(const std::string& message, Client* sender)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second != sender) {
			// Replace 'sendRawMessage' with the actual message sending function name
			it->second->sendToClient(message);  // Use the correct method here
		}
	}
}

bool Channel::checkKey(const std::string& key) const
{
	return key == _key; // Compare provided key with stored key
}


bool Channel::isOperator(Client* client) const
{
	return _operators.find(client->getFd()) != _operators.end();
}


void Channel::setOperator(Client* client, bool status)
{
	if (status)
		_operators.insert(client->getFd());
	else
		_operators.erase(client->getFd());
}

bool Channel::isClient(Client* client) const
{
	return _clients.find(client->getFd()) != _clients.end();
}

bool Channel::isInviteOnly() const
{
	return _inviteOnly;
}

std::string Channel::getKey() const
{
	return _key;
}

bool Channel::hasKey() const
{
	return !_key.empty();
}

bool Channel::isFull() const
{
	if (_userLimit > 0)
	{
		return _clients.size() >= static_cast<size_t>(_userLimit);
	}
	return false;
}

void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

std::string Channel::getTopic() const
{
	return _topic;
}

bool Channel::hasTopic() const
{
	return !_topic.empty();
}

bool Channel::isBanned(Client* client) const
{
	// For now, we can assume no clients are banned
	// You can implement ban logic as needed
	return _bannedClients.find(client->getNickname()) != _bannedClients.end();
}

bool Channel::isInvited(Client* client) const
{
	return _invitedClients.find(client) != _invitedClients.end();
}


// Invite-only mode
void Channel::setInviteOnly(bool status)
{
	_inviteOnly = status;
}

// Topic restriction mode
void Channel::setTopicRestricted(bool status)
{
	_topicRestricted = status;
}

bool Channel::isTopicRestricted() const
{
	return _topicRestricted;
}

// Channel key (password)
void Channel::setKey(const std::string& key)
{
	_key = key;
}

void Channel::removeKey()
{
	_key.clear();
}



// User limit
void Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

void Channel::removeUserLimit()
{
	_userLimit = -1;
}

bool Channel::hasUserLimit() const
{
	return _userLimit != -1;
}

int Channel::getUserLimit() const
{
	return _userLimit;
}

// Channel operators
void Channel::addOperator(Client* client)
{
	_operators.insert(client->getFd());
}

void Channel::removeOperator(Client* client)
{
	_operators.erase(client->getFd());
}
