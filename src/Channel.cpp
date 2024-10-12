/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:58:11 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/12 18:48:53 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp" 
#include "Logger.hpp"
#include <algorithm>
#include <sstream>

// Forward declare server instance if necessary
extern Server* g_server; // Uncomment if you have a global server instance

Channel::Channel(const std::string& name) : name(name), topicSet(false) {}

Channel::~Channel() {}

const std::string& Channel::getName() const {
    return name;
}

bool Channel::isMember(const Client& client) const {
    return members.find(client.getFd()) != members.end();
}

void Channel::addClient(Client* client) {
    clients[client->getFd()] = client;
    members.insert(client->getFd());
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

bool Channel::isClient(const Client* client) const {
    return clients.find(client->getFd()) != clients.end();
}



void Channel::addInvite(Client& client) {
    invitedClients.insert(client.getFd());
}

void Channel::removeInvite(const Client& client) {
    invitedClients.erase(client.getFd());
}

void Channel::setTopic(const std::string& newTopic) {
    topic = newTopic;
    topicSet = true;
}

const std::string& Channel::getTopic() const {
    return topic;
}

bool Channel::hasTopic() const {
    return topicSet;
}

void Channel::broadcast(const std::string& message, const Client* sender) const {
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (sender == nullptr || it->second != sender) {
            it->second->sendRawMessage(message);
        }
    }
}



// void Channel::broadcast(const std::string& message, const Client* sender) const {
//     for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
//         int clientFd = *it;
//         if (sender && clientFd == sender->getFd()) {
//             continue;
//         }
//         if (g_server) {
//             g_server->sendRawMessage(clientFd, message);  // Use g_server to access server instance
//         }
//     }
// }

std::string Channel::getMemberNames() const {
    std::string names;
    for (const auto& clientFd : members) {
        const Client* client = g_server ? g_server->getClient(clientFd) : nullptr;
        if (client) {
            names += client->getNickname() + " ";
        }
    }
    if (!names.empty()) names.pop_back();  // Remove trailing space
    return names;
}


// void Channel::broadcast(const std::string& message, const Client* sender) const {
//     // Iterate through all members and send the message
//     for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
//         int clientFd = *it;
//         // Optionally, skip sending to the sender
//         if (sender && clientFd == sender->getFd()) {
//             continue;
//         }
//         server.sendRawMessage(clientFd, message);
//     }
// }

// std::string Channel::getMemberNames() const {
//     std::string names;
//     // Declare the server instance
//     for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
//         Client* client = server.getClient(*it);
//         if (client) {
//             names += client->getNickname() + " ";
//         }
//     }
//     // Remove trailing space
//     if (!names.empty()) {
//         names.pop_back();
//     }
//     return names;
// }


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

