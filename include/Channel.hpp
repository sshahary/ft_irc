/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:57:58 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/13 15:46:09 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Channel.hpp
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Client.hpp"
#include "IrcCommands.hpp"
#include <iostream>
#include <set>
#include <algorithm>


class Server;
class Client;
class IrcCommands;
class Channel {
public:
    Channel(const std::string& name);
    ~Channel();

    void addClient(Client* client);
    void removeClient(Client* client);
    // void broadcastMessage(const std::string& message, Client* sender);
    void setOperator(Client* client, bool status);
    bool isOperator(Client* client) const;
    bool isClient(Client* client) const;
    void sendNamesList(Client* client,Server& server);

    // Methods for channel modes and properties
    bool isEmpty() const;
    bool isMember(const Client* client) const;

    void broadcastMessage(const std::string& message, Client* sender);

    bool hasTopic() const;
    void setTopic(const std::string& topic);
    std::string getTopic() const;
    bool isInviteOnly() const;
    bool hasKey() const;
    std::string getKey() const;
    bool isFull() const;
    bool isBanned(Client* client) const;
    bool isInvited(Client* client) const;
    const std::string& getName() const;

    // Mode management
    void setInviteOnly(bool status);
    

    void setTopicRestricted(bool status);
    bool isTopicRestricted() const;

    void setKey(const std::string& key);
    void removeKey();
    
    void setUserLimit(int limit);
    void removeUserLimit();
    bool hasUserLimit() const;
    int getUserLimit() const;
    void addInvite(Client* client);
    std::string getMemberNames() const;
    void removeMember(Client* client);
    void broadcast(const std::string& message, Client* sender);


    void addOperator(Client* client);
    void removeOperator(Client* client);
    

private:
    std::string _name;
    std::map<int, Client*> _clients;
    std::string _topic;
    // Channel modes and properties
    bool _inviteOnly;
    std::string _key;
    int _userLimit;
    std::set<std::string> _bannedClients;
    std::set<Client*> _invitedClients;
    bool _topicRestricted;
    std::set<int> _operators; // Store operator client FDs
};

#endif


// #ifndef CHANNEL_HPP
// #define CHANNEL_HPP

// #include "Client.hpp"
// #include <string>
// #include <set>
// #include <map>

// class Server; // Forward declaration

// class Channel {
// public:
//     Channel(const std::string& name);
//     ~Channel();

//     const std::string& getName() const;
//     const std::string& getTopic() const;
//     void setTopic(const std::string& topic);

//      void addClient(Client* client); 
     
//     bool isMember(const Client& client) const;
//     bool isClient(const Client* client) const;

//     void addMember(Client& client);
//     void removeMember(const Client& client);

//     bool isInvited(const Client& client) const;
//     void addInvite(Client& client);
//     void removeInvite(const Client& client);

//     void broadcast(const std::string& message, const Client* sender) const;

//     std::string getMemberNames() const;
    
//     bool isOperator(const Client& client) const;
//     void addOperator(Client& client);
//     void removeOperator(const Client& client);
//     bool hasTopic() const;

// private:
//     std::string name;
//     std::string topic;
//     std::set<int> members;         // Set of client file descriptors
//     std::set<int> invitedClients;  // Set of invited client file descriptors
//     std::set<int> operators;
//     bool topicSet;
//     std::map<int, Client*> clients;
// };

// #endif // CHANNEL_HPP
