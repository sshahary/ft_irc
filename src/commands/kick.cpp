/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 09:00:08 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/26 12:58:47 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../include/commands.hpp"

void Server::handleKickCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.size() < 2) {
		sendError(clientFd, "ERR_NEEDMOREPARAMS", "KICK command requires a channel and a nickname");
		return;
	}

	std::string channelName = params[0];
	std::string targetNick = params[1];
	
	Client* client = findClientByFd(clientFd);
	if (!client || !client->isRegistered()) {
		sendError(clientFd, "ERR_NOTREGISTERED", "You must register first");
		return;
	}

	// Find the channel by its name
	Channel* channel = findChannelByName(channelName);
	if (!channel) {
		sendError(clientFd, "ERR_NOSUCHCHANNEL", "No such channel");
		return;
	}

	// Check if the client is an operator in the channel
	if (!channel->isOperator(clientFd)) {
		sendError(clientFd, "ERR_CHANOPRIVSNEEDED", "You're not a channel operator");
		return;
	}

	// Find the target client by their nickname
	Client* targetClient = findClientByNickname(targetNick);
	if (!targetClient || !channel->hasClient(targetClient->getFd()))
	{
		sendError(clientFd, "ERR_USERNOTINCHANNEL", targetNick + " is not in the channel");
		return;
	}

	// Remove the client from the channel
	channel->removeClient(targetClient->getFd());  // Use Channel's removeClient method
	
	// Notify other clients in the channel
	std::string kickMessage = ":" + client->getNickname() + " KICK " + channelName + " " + targetNick + "\r\n";
	sendToChannel(channelName, kickMessage, clientFd);  // Notify others
	send(targetClient->getFd(), kickMessage.c_str(), kickMessage.length(), 0);  // Notify the kicked client
}

void Server::sendToChannel(const std::string& channelName, const std::string& message, int excludeFd)
{
	Channel* channel = findChannelByName(channelName);
	if (!channel) return;

	// Iterate through all clients in the channel
	for (int clientFd : channel->getClients()) {
		if (clientFd != excludeFd) {  // Skip the client that triggered the event
			sendToClient(clientFd, message);
		}
	}
}

Client* Server::findClientByNickname(const std::string& nickname)
{
	for (Client& client : clients)
	{
		// Iterate over Client* pointers
		if (client.getNickname() == nickname)
			return &client; // Return pointer to the found client
	}
	return NULL;
}