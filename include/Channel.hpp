/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:57:58 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/12 17:14:44 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <set>
#include <map>

class Server; // Forward declaration

class Channel {
public:
    Channel(const std::string& name);
    ~Channel();

    const std::string& getName() const;
    const std::string& getTopic() const;
    void setTopic(const std::string& topic);

    bool isMember(const Client& client) const;
    void addMember(Client& client);
    void removeMember(const Client& client);

    bool isInvited(const Client& client) const;
    void addInvite(Client& client);
    void removeInvite(const Client& client);

    void broadcast(const std::string& message, const Client* sender) const;

    std::string getMemberNames() const;
    
    bool isOperator(const Client& client) const;
    void addOperator(Client& client);
    void removeOperator(const Client& client);

private:
    std::string name;
    std::string topic;
    std::set<int> members;         // Set of client file descriptors
    std::set<int> invitedClients;  // Set of invited client file descriptors
    std::set<int> operators;
};

#endif // CHANNEL_HPP
