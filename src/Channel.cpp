/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:58:11 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/13 16:14:56 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Channel.cpp
#include "Channel.hpp"
#include <set>
#include <sstream>
#include "IrcCommands.hpp" 

Channel::Channel(const std::string& name)
    : _name(name),_topic(""), _inviteOnly(false), _key(""), _userLimit(-1), _topicRestricted(false) {
}

Channel::~Channel() {}

void Channel::addClient(Client* client) {
    _clients[client->getFd()] = client;
    // Optionally, set the first client as operator
    if (_clients.size() == 1) {
        setOperator(client, true);
    }
}

// void Channel::addClient(Client* client, const std::string& serverHostname) {
//     _clients[client->getFd()] = client;
//     // Optionally set the first client as operator
//     std::cout << "Client added to channel " << _name << " on server " << serverHostname << std::endl;
//     if (_clients.size() == 1) {
//         setOperator(client, true);
//     }
//     // Remove client from invite list if present
//     _invitedClients.erase(client->getNickname());
// }

void Channel::removeClient(Client* client) {
    _clients.erase(client->getFd());
    _operators.erase(client->getFd());

    // Notify remaining clients
    std::string partMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "server_hostname" + " PART :" + _name + "\r\n";
    broadcastMessage(partMsg, client);
}

bool Channel::isEmpty() const {
    return _clients.empty();
}

bool Channel::isMember(const Client* client) const {
    return _clients.find(client->getFd()) != _clients.end();
}

void Channel::addInvite(Client* client) {
    _invitedClients.insert(client);
}

void Channel::removeMember(Client* client) {
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

const std::string& Channel::getName() const {
    return _name;
}

std::string Channel::getMemberNames() const {
    std::ostringstream names;
    for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        names << it->second->getNickname() << " ";
    }
    // ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " JOIN :" + channelName + "\r\n"

    return names.str();
}


void Channel::broadcastMessage(const std::string& message, Client* sender) {
    std::cout << "From broadcastMessage Function: " << std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second != sender) {
            // Replace 'sendRawMessage' with the actual message sending function name
            it->second->sendToClient(message);  // Use the correct method here
        }
    }
}



bool Channel::isOperator(Client* client) const {
    return _operators.find(client->getFd()) != _operators.end();
}


void Channel::setOperator(Client* client, bool status) {
    if (status) {
        _operators.insert(client->getFd());
    } else {
        _operators.erase(client->getFd());
    }
}

bool Channel::isClient(Client* client) const {
    return _clients.find(client->getFd()) != _clients.end();
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

std::string Channel::getKey() const {
    return _key;
}

bool Channel::hasKey() const {
    return !_key.empty();
}

bool Channel::isFull() const {
    if (_userLimit > 0) {
        return _clients.size() >= static_cast<size_t>(_userLimit);
    }
    return false;
}

void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

std::string Channel::getTopic() const {
    return _topic;
}

bool Channel::hasTopic() const {
    return !_topic.empty();
}

bool Channel::isBanned(Client* client) const {
    // For now, we can assume no clients are banned
    // You can implement ban logic as needed
    return _bannedClients.find(client->getNickname()) != _bannedClients.end();
}

// bool Channel::isInvited(Client* client) const {
//     return _invitedClients.find(client->getNickname()) != _invitedClients.end();
// }

bool Channel::isInvited(Client* client) const {
    return _invitedClients.find(client) != _invitedClients.end();
}


// Invite-only mode
void Channel::setInviteOnly(bool status) {
    _inviteOnly = status;
}

// Topic restriction mode
void Channel::setTopicRestricted(bool status) {
    _topicRestricted = status;
}

bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}

// Channel key (password)
void Channel::setKey(const std::string& key) {
    _key = key;
}

void Channel::removeKey() {
    _key.clear();
}



// User limit
void Channel::setUserLimit(int limit) {
    _userLimit = limit;
}

void Channel::removeUserLimit() {
    _userLimit = -1;
}

bool Channel::hasUserLimit() const {
    return _userLimit != -1;
}

int Channel::getUserLimit() const {
    return _userLimit;
}

// Channel operators
void Channel::addOperator(Client* client) {
    _operators.insert(client->getFd());
}

void Channel::removeOperator(Client* client) {
    _operators.erase(client->getFd());
}



// #include "Channel.hpp"
// #include "Server.hpp" 
// #include "Logger.hpp"
// #include <algorithm>
// #include <sstream>

// // Forward declare server instance if necessary
// extern Server* g_server; // Uncomment if you have a global server instance

// Channel::Channel(const std::string& name) : name(name), topicSet(false) {}

// Channel::~Channel() {}

// const std::string& Channel::getName() const {
//     return name;
// }

// bool Channel::isMember(const Client& client) const {
//     return members.find(client.getFd()) != members.end();
// }

// void Channel::addClient(Client* client) {
//     clients[client->getFd()] = client;
//     members.insert(client->getFd());
// }



// void Channel::addMember(Client& client) {
//     members.insert(client.getFd());
// }

// void Channel::removeMember(const Client& client) {
//     members.erase(client.getFd());
// }

// bool Channel::isInvited(const Client& client) const {
//     return invitedClients.find(client.getFd()) != invitedClients.end();
// }

// bool Channel::isClient(const Client* client) const {
//     return clients.find(client->getFd()) != clients.end();
// }



// void Channel::addInvite(Client& client) {
//     invitedClients.insert(client.getFd());
// }

// void Channel::removeInvite(const Client& client) {
//     invitedClients.erase(client.getFd());
// }

// void Channel::setTopic(const std::string& newTopic) {
//     topic = newTopic;
//     topicSet = true;
// }

// const std::string& Channel::getTopic() const {
//     return topic;
// }

// bool Channel::hasTopic() const {
//     return topicSet;
// }

// void Channel::broadcast(const std::string& message, const Client* sender) const {
//     for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
//         if (sender == nullptr || it->second != sender) {
//             it->second->sendRawMessage(message);
//         }
//     }
// }



// // void Channel::broadcast(const std::string& message, const Client* sender) const {
// //     for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
// //         int clientFd = *it;
// //         if (sender && clientFd == sender->getFd()) {
// //             continue;
// //         }
// //         if (g_server) {
// //             g_server->sendRawMessage(clientFd, message);  // Use g_server to access server instance
// //         }
// //     }
// // }

// std::string Channel::getMemberNames() const {
//     std::string names;
//     for (const auto& clientFd : members) {
//         const Client* client = g_server ? g_server->getClient(clientFd) : nullptr;
//         if (client) {
//             names += client->getNickname() + " ";
//         }
//     }
//     if (!names.empty()) names.pop_back();  // Remove trailing space
//     return names;
// }


// // void Channel::broadcast(const std::string& message, const Client* sender) const {
// //     // Iterate through all members and send the message
// //     for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
// //         int clientFd = *it;
// //         // Optionally, skip sending to the sender
// //         if (sender && clientFd == sender->getFd()) {
// //             continue;
// //         }
// //         server.sendRawMessage(clientFd, message);
// //     }
// // }

// // std::string Channel::getMemberNames() const {
// //     std::string names;
// //     // Declare the server instance
// //     for (std::set<int>::iterator it = members.begin(); it != members.end(); ++it) {
// //         Client* client = server.getClient(*it);
// //         if (client) {
// //             names += client->getNickname() + " ";
// //         }
// //     }
// //     // Remove trailing space
// //     if (!names.empty()) {
// //         names.pop_back();
// //     }
// //     return names;
// // }


// // In Channel.cpp
// bool Channel::isOperator(const Client& client) const {
//     return operators.find(client.getFd()) != operators.end();
// }

// void Channel::addOperator(Client& client) {
//     if (isMember(client)) {
//         operators.insert(client.getFd());
//     }
// }

// void Channel::removeOperator(const Client& client) {
//     operators.erase(client.getFd());
// }

