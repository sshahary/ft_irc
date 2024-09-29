/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:05:43 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/29 14:08:22 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"


void Server::sendError(int clientFd, const std::string& errorCode, const std::string& message)
{
	std::string errorMessage = ":" + errorCode + " " + message + "\r\n";
	send(clientFd, errorMessage.c_str(), errorMessage.length(), 0);
}

// Send a message to a client
void Server::sendMessage(int clientFd, const std::string& command, const std::string& message, const std::string& prefix)
{
	std::string fullMessage;

	// If there's a prefix, prepend it
	if (!prefix.empty())
	{
		fullMessage += ":" + prefix + " ";
	}

	// Append the command and message
	fullMessage += command + " " + message + "\r\n";

	// Send the message to the client
	if (send(clientFd, fullMessage.c_str(), fullMessage.length(), 0) == -1)
	{
		std::cerr << "Error sending message to client: " << strerror(errno) << std::endl;
	}
}

// Send a welcome message upon successful registration
void Server::sendWelcomeMessage(int clientFd, const std::string& nickname)
{
	std::string serverName = "irc.myserver.com"; // Replace with your server name

	// Example welcome message sequence
	sendMessage(clientFd, "001", nickname + " :Welcome to the Internet Relay Network, " + nickname, serverName);
	sendMessage(clientFd, "002", nickname + " :Your host is " + serverName + ", running version 1.0", serverName);
	sendMessage(clientFd, "003", nickname + " :This server was created today", serverName);
	sendMessage(clientFd, "004", nickname + " " + serverName + " 1.0 o o", serverName);
}

// Example for sending a success message after a command execution
void Server::sendSuccessMessage(int clientFd, const std::string& message)
{
	std::string serverName = "irc.myserver.com"; // Replace with your server name

	// Send a success response (e.g., after a command like PRIVMSG or JOIN)
	sendMessage(clientFd, "NOTICE", ":Success - " + message, serverName);
}
