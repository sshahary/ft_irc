/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:34:51 by sshahary          #+#    #+#             */
/*   Updated: 2024/10/02 11:05:23 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"
#include "../../include/commands.hpp"
#include "../../include/constants.hpp"

// // void Server::handleJoinCommand(int clientFd, const std::vector<std::string>& params)
// // {
// // 	if (!findClientByFd(clientFd)->hasNickSet())
// // 	{
// // 		sendError(clientFd, "ERR_NONICKNAMEGIVEN", ":No nickname given");
// // 		return;
// // 	}
// // 	if (params.size() < 1)
// // 	{
// // 		sendError(clientFd, "ERR_NEEDMOREPARAMS", "JOIN :Not enough parameters");
// // 		return;
// // 	}

// // 	std::string channelName = params[0];
// // 	std::string password = params.size() > 1 ? params[1] : "";

// // 	// Check if client is already in the channel
// // 	Channel* channel = findChannelByName(channelName);
// // 	if (channel && channel->isClientInChannel(clientFd))
// // 	{
// // 		sendError(clientFd, "ERR_ALREADYINCHANNEL", channelName + " :You are already in this channel");
// // 		return;
// // 	}

// // 	// Channel doesn't exist, create a new one
// // 	if (!channel)
// // 	{
// // 		channel = new Channel(channelName);
// // 		channels.push_back(channel);
// // 		channel->addClient(clientFd);
// // 	}
// // 	else
// // 	{
// // 		// Existing channel: Check if invite-only or password protected
// // 		if (channel->isInviteOnly() || (!password.empty() && !channel->checkPassword(password)))
// // 		{
// // 			sendError(clientFd, "ERR_BADCHANNELKEY", channelName + " :Cannot join channel (invite-only or wrong key)");
// // 			return;
// // 		}

// // 		if (channel->isUserLimitExceeded())
// // 		{
// // 			sendError(clientFd, "ERR_CHANNELISFULL", channelName + " :Cannot join channel (channel is full)");
// // 			return;
// // 		}

// // 		// Add the client to the channel
// // 		channel->addClient(clientFd);
// // 	}

// // 	// Notify other clients in the channel that this client has joined
// // 	std::string joinMessage = ":" + getClientNickname(clientFd) + " JOIN " + channelName + "\r\n";
// // 	broadcastToChannel(channel, joinMessage, clientFd);  // Broadcast to everyone except the joining client

// // 	// Send confirmation to the client that they have joined the channel
// // 	sendToClient(clientFd, joinMessage);

// // 	// Send the channel topic to the client if it has one
// // 	if (!channel->getTopic().empty())
// // 	{
// // 		std::string topicMessage = ":" + serverName + " 332 " + getClientNickname(clientFd) + " " + channelName + " :" + channel->getTopic() + "\r\n";
// // 		sendToClient(clientFd, topicMessage);
// // 	}

// // 	// Send the list of users currently in the channel (NAMES command response)
// // 	std::string namesMessage = ":" + serverName + " 353 " + getClientNickname(clientFd) + " = " + channelName + " :" + getChannelUsers(channel) + "\r\n";
// // 	sendToClient(clientFd, namesMessage);

// // 	std::string endOfNamesMessage = ":" + serverName + " 366 " + getClientNickname(clientFd) + " " + channelName + " :End of /NAMES list\r\n";
// // 	sendToClient(clientFd, endOfNamesMessage); 

// // 	std::cout << "Client " << clientFd << " joined channel " << channelName << std::endl;
// // }

// std::string Channel::getUsersList(const Server& server) const
// {
// 	std::string userList;
// 	for (const int clientFd : clients) {
// 		userList += server.getClientNickname(clientFd) + " ";
// 	}
// 	return userList;
// }

// void Server::sendChannelUserList(int clientFd, Channel* channel)
// {
// 	std::string usersList = getChannelUsers(channel);
// 	std::string namesMessage = ":" + serverName + " 353 " + getClientNickname(clientFd) + " = " + channel->getName() + " :" + usersList + "\r\n";
// 	sendToClient(clientFd, namesMessage);

// 	std::string endOfNamesMessage = ":" + serverName + " 366 " + getClientNickname(clientFd) + " " + channel->getName() + " :End of /NAMES list\r\n";
// 	sendToClient(clientFd, endOfNamesMessage);
// }

// void Server::handleJoinCommand(int clientFd, const std::vector<std::string>& params)
// {
// 	if (params.size() < 1)
// 	{
// 		sendError(clientFd, ERR_NEEDMOREPARAMS , "JOIN :Not enough parameters");
// 		return;
// 	}

// 	// Check if the user has a nickname set
// 	Client* client = findClientByFd(clientFd);
// 	if (!client || !client->hasNickSet())
// 	{
// 		sendError(clientFd,  ERR_NONICKNAMEGIVEN, ":You need to set a nickname first");
// 		return;
// 	}

// 	std::string channelName = params[0];
// 	Channel* channel = findChannelByName(channelName);

// 	if (!channel)
// 	{
// 		// Channel doesn't exist, create a new one
// 		channel = new Channel(channelName);
// 		channels.push_back(channel);
// 	}

// 	// Add the client to the channel
// 	if (channel->isClientInChannel(clientFd))
// 	{
// 		sendError(clientFd, ERR_USERONCHANNEL , channelName + " :You are already in the channel");
// 	}
// 	else
// 	{
// 		channel->addClient(clientFd);
// 		sendToClient(clientFd, ":" + getClientNickname(clientFd) + " JOIN " + channelName + "\r\n");
// 		// Notify other users
// 		broadcastToChannel(channel, ":" + getClientNickname(clientFd) + " JOIN " + channelName + "\r\n", clientFd);

// 		// Send the list of users currently in the channel
// 		sendChannelUserList(clientFd, channel);
// 	}
// }


void Server::handleJoinCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.empty()) {
		sendError(clientFd, "461", "JOIN :Not enough parameters");
		return;
	}

	std::string channelName = params[0];
	std::string channelKey = (params.size() > 1) ? params[1] : "";

	Channel* channel = findChannelByName(channelName);
	
	// Create a new channel if it doesn't exist
	if (!channel) {
		channel = new Channel(channelName);
		channels.push_back(channel);
		std::cout << "Created new channel: " << channelName << std::endl;
	}

	// Check if the user is already in the channel
	if (channel->isClientInChannel(clientFd)) {
		sendError(clientFd, "443", "You're already in the channel");
		return;
	}

	// If invite-only, check if the client is invited
	if (channel->isInviteOnly() && !channel->isOperator(clientFd)) {
		sendError(clientFd, "473", "Cannot join channel: invite-only");
		return;
	}

	// Check for password if the channel is password-protected
	if (!channel->checkPassword(channelKey)) {
		sendError(clientFd, "475", "Cannot join channel: incorrect password");
		return;
	}

	// Check for user limit
	if (channel->isUserLimitExceeded()) {
		sendError(clientFd, "471", "Cannot join channel: channel is full");
		return;
	}

	// Add the client to the channel
	channel->addClient(clientFd);

	// Notify the client and other members in the channel
	std::string clientNick = getClientNickname(clientFd);
	std::string joinMessage = ":" + clientNick + " JOIN " + channelName;
	broadcastToChannel(channel, joinMessage, -1);  // Send to all users including the joining user
	
	// Send topic if exists
	if (!channel->getTopic().empty()) {
		sendToClient(clientFd, "332 " + clientNick + " " + channelName + " :" + channel->getTopic());
	} else {
		sendToClient(clientFd, "331 " + clientNick + " " + channelName + " :No topic is set");
	}

	// Send list of users in the channel
	sendChannelUserList(clientFd, channel);
	
	std::cout << clientNick << " joined channel: " << channelName << std::endl;
}
