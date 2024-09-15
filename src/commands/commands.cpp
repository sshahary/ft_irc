/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:14:59 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/15 13:09:58 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"

void Server::handlePassCommand(int clientFd, const std::vector<std::string>& params)
{
	Client* client = findClientByFd(clientFd);
	if (client == NULL)
		return;  // If client not found, do nothing

	if (client->isAuthenticated())
	{
		sendError(clientFd, "ERR_ALREADYREGISTRED", "You may not reregister");
		return;
	}

	if (params.size() < 1)
	{
		std::cerr << RED << "No password provided" << WHI << std::endl;
		sendError(clientFd, "ERR_NEEDMOREPARAMS", "No password provided");
		return ;
	}
	std::string providedPassword = params[0];
	if (providedPassword == serverPassword)
	{
		std::cout << GRE << "Client authenticated successfully" << WHI << std::endl;
		client->setAuthenticated(true);
	}
	else
	{
		std::cerr << RED << "Invalid password" << WHI << std::endl;
		sendError(clientFd, "ERR_PASSWDMISMATCH", "Invalid password");
		removeClient(clientFd);
	}
}

// Handle the NICK command
void Server::handleNickCommand(int clientFd, const std::vector<std::string>& params)
{
	Client* client = findClientByFd(clientFd);
	if (client == NULL)
		return;

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

	// Update the client's nickname and the nickname tracking
	std::string oldNickname = client->getNickname();
	client->setNickname(newNickname);
	client->setNickSet(true);
	updateNicknameTracking(oldNickname, newNickname, clientFd);

	// Log the change
	std::cout << GRE << "Client " << clientFd << " set nickname to '" << newNickname << "'" << WHI << std::endl;
	 // Check if USER is also set to mark the client as registered
	if (client->isAuthenticated() && client->hasNickSet())
	{
		client->setRegistered(true);
		std::cout << GRE << "Client " << clientFd << " is now fully registered" << WHI << std::endl;
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
		removeNicknameTracking(oldNickname);
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


//USer command
void Server::handleUserCommand(int clientFd, const std::vector<std::string>& params)
{
	std::cout<<"clientfd: "<<clientFd<<"pars: "<<&params<<std::endl;
	Client* client = findClientByFd(clientFd);
	if (client == NULL)
		return;

	if (client->isRegistered())
	{
		sendError(clientFd, "ERR_ALREADYREGISTERED", ":You may not reregister");
		return;
	}

	if (params.size() < 4)
	{
		sendError(clientFd, "ERR_NEEDMOREPARAMS", ":Not enough parameters");
		return;
	}

	if (!client->hasNickSet())
	{
		sendError(clientFd, "ERR_NONICKNAMEGIVEN", ":Nickname must be provided before USER");
		return;
	}

	client->setRegistered(true);

	std::string username = params[0];
	std::string realname = params[3]; // Real name prefixed with ':'

	std::cout << GRE << "Client " << clientFd << " registered with username '" << username << "' and real name '" << realname << "'" << WHI << std::endl;
}
