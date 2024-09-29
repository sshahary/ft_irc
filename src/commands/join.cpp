/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:34:51 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/29 15:20:22 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"
#include "../../include/commands.hpp"

void Server::handleJoinCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.size() < 1) {
		sendError(clientFd, "ERR_NEEDMOREPARAMS", "JOIN :Not enough parameters");
		return;
	}

	std::string channelName = params[0];
	std::string password = params.size() > 1 ? params[1] : "";

	// Check if client is already in the channel
	Channel* channel = findChannelByName(channelName);
	if (channel && channel->isClientInChannel(clientFd)) {
		sendError(clientFd, "ERR_ALREADYINCHANNEL", channelName + " :You are already in this channel");
		return;
	}

	// Channel doesn't exist, create a new one
	if (!channel) {
		channel = new Channel(channelName);
		channels.push_back(channel);
		channel->addClient(clientFd);
	} else {
		// Existing channel: Check if invite-only or password protected
		if (channel->isInviteOnly() || (!password.empty() && !channel->checkPassword(password))) {
			sendError(clientFd, "ERR_BADCHANNELKEY", channelName + " :Cannot join channel (invite-only or wrong key)");
			return;
		}

		if (channel->isUserLimitExceeded()) {
			sendError(clientFd, "ERR_CHANNELISFULL", channelName + " :Cannot join channel (channel is full)");
			return;
		}

		// Add the client to the channel
		channel->addClient(clientFd);
	}

	// Notify other clients in the channel that this client has joined
	std::string joinMessage = ":" + getClientNickname(clientFd) + " JOIN " + channelName + "\r\n";
	broadcastToChannel(channel, joinMessage, clientFd);  // Broadcast to everyone except the joining client

	// Send confirmation to the client that they have joined the channel
	sendToClient(clientFd, joinMessage);

	// Send the channel topic to the client if it has one
	if (!channel->getTopic().empty()) {
		std::string topicMessage = ":" + serverName + " 332 " + getClientNickname(clientFd) + " " + channelName + " :" + channel->getTopic() + "\r\n";
		sendToClient(clientFd, topicMessage);
	}

	// Send the list of users currently in the channel (NAMES command response)
	std::string namesMessage = ":" + serverName + " 353 " + getClientNickname(clientFd) + " = " + channelName + " :" + getChannelUsers(channel) + "\r\n";
	sendToClient(clientFd, namesMessage);

	std::string endOfNamesMessage = ":" + serverName + " 366 " + getClientNickname(clientFd) + " " + channelName + " :End of /NAMES list\r\n";
	sendToClient(clientFd, endOfNamesMessage); 

	std::cout << "Client " << clientFd << " joined channel " << channelName << std::endl;
}

std::string Channel::getUsersList(const Server& server) const
{
	std::string userList;
	for (const int clientFd : clients) {
		userList += server.getClientNickname(clientFd) + " ";
	}
	return userList;
}
