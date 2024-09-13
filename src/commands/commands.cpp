/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:14:59 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/13 17:44:34 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"

void Server::handlePassCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.size() < 1)
	{
		std::cerr << RED << "No password provided" << WHI << std::endl;
		return ;
	}
	std::string providedPassword = params[0];
	if (providedPassword == serverPassword)
	{
		std::cout << GRE << "Client authenticated successfully" << WHI << std::endl;
		// Set client authenticated (not shown, but you would likely have a flag in Client)
	}
	else
	{
		std::cerr << RED << "Invalid password" << WHI << std::endl;
		removeClient(clientFd);
	}
}

// Handle the NICK command
void Server::handleNickCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		sendError(clientFd, "ERR_NONICKNAMEGIVEN", "No nickname given");
		return;
	}

	std::string newNickname = params[0];

	// Check if the nickname is already in use
	if (isNicknameTaken(newNickname))
	{
		sendError(clientFd, "ERR_NICKNAMEINUSE", newNickname + " :Nickname is already in use");
		return;
	}

	// Find the client and update their nickname
	for (auto &client : clients)
	{
		if (client.getFd() == clientFd)
		{
			std::string oldNickname = client.getNickname();
			client.setNickname(newNickname);
			updateNicknameTracking(oldNickname, newNickname, clientFd);
			std::cout << GRE << "Client " << clientFd << " set nickname to '" << newNickname << "'" << WHI << std::endl;
			break;
		}
	}
}

// Check if a nickname is already taken
bool Server::isNicknameTaken(const std::string& nickname) const
{
	return nicknames.find(nickname) != nicknames.end();
}

// Update the nickname tracking (for nickname changes)
void Server::updateNicknameTracking(const std::string& oldNickname, const std::string& newNickname, int clientFd)
{
	if (!oldNickname.empty())
		nicknames.erase(oldNickname);
	
	nicknames[newNickname] = clientFd;
}

// Remove a nickname from tracking (if a client disconnects)
void Server::removeNicknameTracking(const std::string& nickname)
{
	nicknames.erase(nickname);
}

// Send an error message to a client
void Server::sendError(int clientFd, const std::string& errorCode, const std::string& message)
{
	std::string errorMessage = ":" + errorCode + " " + message + "\r\n";
	send(clientFd, errorMessage.c_str(), errorMessage.length(), 0);
}
