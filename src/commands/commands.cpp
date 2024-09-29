/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:14:59 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/29 15:18:28 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"


void Server::handlePassCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.size() < 1)
	{
		sendError(clientFd, "ERR_NEEDMOREPARAMS", "PASS :No password provided");
		return;
	}
	Client* client = findClientByFd(clientFd);
	if (client == NULL)
		return;

	// Check if the client is already authenticated
	if (client->isAuthenticated())
	{
		sendError(clientFd, "ERR_ALREADYREGISTRED", "PASS :You are already authenticated");
		return;
	}

	std::string providedPassword = params[0];

	// Compare provided password with the server's password
	if (providedPassword == serverPassword)
	{
		client->setAuthenticated(true);
		std::cout << "Client authenticated successfully" << std::endl;
	}
	else
	{
		sendError(clientFd, "ERR_PASSWDMISMATCH", "PASS :Incorrect password");
		std::cerr << "Invalid password" << std::endl;
	}
}

// void Server::handleNickCommand(int clientFd, const std::vector<std::string>& params)
// {
// 	if (params.size() < 1)
// 	{
// 		sendError(clientFd, "ERR_NEEDMOREPARAMS", "NICK :Not enough parameters");
// 		return;
// 	}
// 	Client* client = findClientByFd(clientFd);
// 	if (client == NULL)
// 		return;

// 	std::string newNickname = params[0];

// 	// Check if the nickname is already in use
// 	if (isNicknameTaken(newNickname))
// 	{
// 		sendError(clientFd, "ERR_NICKNAMEINUSE", newNickname + " :Nickname is already in use");
// 		return;
// 	}

// 	// Update the client's nickname and the nickname tracking
// 	std::string oldNickname = client->getNickname();
// 	client->setNickname(newNickname);
// 	client->setNickSet(true);
// 	updateNicknameTracking(oldNickname, newNickname, clientFd);

// 	std::cout << "Client " << clientFd << " set nickname to '" << newNickname << "'" << std::endl;

// 	// Don't mark the client as fully registered yet; wait for the USER command
// 	if (client->isAuthenticated())
// 	{
// 		std::cout << "Client has provided NICK, waiting for USER to fully register." << std::endl;
// 	}
// 	else
// 	{
// 		sendError(clientFd, "ERR_NOTAUTHENTICATED", "Please authenticate using the PASS command before proceeding.");
// 		std::cout << "Client provided NICK, but is not yet authenticated (waiting for PASS and USER)." << std::endl;
// 	}

void Server::handleNickCommand(int clientFd, const std::vector<std::string>& params)
{
	// Find the client by file descriptor
	Client* client = findClientByFd(clientFd);
	if (client == NULL)
		return;

	// Check if the client is authenticated
	if (!client->isAuthenticated())
	{
		sendError(clientFd, "ERR_NOTAUTHENTICATED", "You must authenticate using the PASS command before setting a nickname.");
		std::cout << "Client " << clientFd << " attempted to set nickname without authentication." << std::endl;
		return;
	}

	// Check if enough parameters are provided
	if (params.size() < 1)
	{
		sendError(clientFd, "ERR_NEEDMOREPARAMS", "NICK :Not enough parameters");
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

	std::cout << "Client " << clientFd << " set nickname to '" << newNickname << "'" << std::endl;

	// Check if the client is fully registered
	if (client->isAuthenticated())
	{
		std::cout << "Client has provided NICK, waiting for USER to fully register." << std::endl;
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

void Server::handleUserCommand(int clientFd, const std::vector<std::string>& params)
{
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

	std::string username = params[0];
	std::string realname = params[3];
	client->setUsername(username);
	client->setRealName(realname);

	// After receiving both NICK and USER, mark the client as registered
	if (client->isAuthenticated() && client->hasNickSet())
	{
		client->setRegistered(true);
		std::string serverName = "irc.myserver.com";  // Use your actual server name or identifier
		sendMessage(clientFd, "001", "Welcome to the Internet Relay Network " + client->getNickname() + "!", serverName);
		std::cout << "Client " << clientFd << " registered with username '" << username << "' and real name '" << realname << "'" << std::endl;
	}
	else
	{
		std::cout << "Client provided USER, but is not yet authenticated or has no nickname (waiting for NICK/PASS)." << std::endl;
	}
}
