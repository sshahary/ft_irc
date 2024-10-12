/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:58:11 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/12 17:28:06 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp" 
#include "Logger.hpp"
#include <algorithm>
#include <sstream>

// Forward declare server instance if necessary
extern Server server; // Uncomment if you have a global server instance

Channel::Channel(const std::string& name) : name(name), topic("") {}

Channel::~Channel() {}

const std::string& Channel::getName() const {
    return name;
}

const std::string& Channel::getTopic() const {
    return topic;
}

void Channel::setTopic(const std::string& newTopic) {
    topic = newTopic;
}

bool Channel::isMember(const Client& client) const {
    return members.find(client.getFd()) != members.end();
}

void Channel::addMember(Client& client) {
    members.insert(client.getFd());
}

void Channel::removeMember(const Client& client) {
    members.erase(client.getFd());
}

bool Channel::isInvited(const Client& client) const {
    return invitedClients.find(client.getFd()) != invitedClients.end();
}

void Channel::addInvite(Client& client) {
    invitedClients.insert(client.getFd());
}

void Channel::removeInvite(const Client& client) {
    invitedClients.erase(client.getFd());
}

void Channel::broadcast(const std::string& message, const Client* sender) const {
    // Iterate through all members and send the message
    for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
        int clientFd = *it;
        // Optionally, skip sending to the sender
        if (sender && clientFd == sender->getFd()) {
            continue;
        }
        server.sendRawMessage(clientFd, message);
    }
}

std::string Channel::getMemberNames() const {
    std::string names;
    // Declare the server instance
    for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
        Client* client = server.getClient(*it);
        if (client) {
            names += client->getNickname() + " ";
        }
    }
    // Remove trailing space
    if (!names.empty()) {
        names.pop_back();
    }
    return names;
}


// In Channel.cpp
bool Channel::isOperator(const Client& client) const {
    return operators.find(client.getFd()) != operators.end();
}

void Channel::addOperator(Client& client) {
    if (isMember(client)) {
        operators.insert(client.getFd());
    }
}

void Channel::removeOperator(const Client& client) {
    operators.erase(client.getFd());
}

