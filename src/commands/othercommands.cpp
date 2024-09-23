/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   othercommands.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:34:51 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/23 10:09:43 by sshahary         ###   ########.fr       */
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

	Channel* channel = findChannelByName(channelName);

	if (!channel)
	{
		channel = new Channel(channelName);
		channels.push_back(channel);
		channel->addClient(clientFd);
	}
	else
	{
		// Check if channel has a password or invite-only
		if (channel->isInviteOnly() || !channel->checkPassword(password))
		{
			sendError(clientFd, "ERR_BADCHANNELKEY", channelName + " :Cannot join channel (invite only or wrong key)");
			return;
		}

		if (channel->isUserLimitExceeded())
		{
			sendError(clientFd, "ERR_CHANNELISFULL", channelName + " :Cannot join channel (channel is full)");
			return;
		}

		channel->addClient(clientFd);
	}

	std::cout <<GRE<<"Client " << clientFd << " joined channel " << channelName << std::endl;
}

