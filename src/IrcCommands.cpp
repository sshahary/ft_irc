
#include "IrcCommands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Constants.hpp"
#include <sstream>
#include "IrcException.hpp"


/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/


IrcCommands::IrcCommands(Server& server) : server(server) {}

IrcCommands::~IrcCommands() {}


/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/

void IrcCommands::ircCommandsDispatcher(Client& client, const std::string& message)
{
	// Split the incoming message into command and parameters
	std::vector<std::string> messageParts = splitMessage(message);
	// If the message is empty after splitting, return without processing
	if (messageParts.empty())
		return;
	// Extract the command from the message (first part)
	std::string command = messageParts[0];
	// Convert the command to uppercase to handle case-insensitivity
	for (std::string::iterator it = command.begin(); it != command.end(); ++it)
		*it = std::toupper(*it);
	// Extract the parameters (everything after the command)
	std::vector<std::string> params(messageParts.begin() + 1, messageParts.end());
	// Determine the command type and dispatch to the appropriate handler
	int commandType = getCommandType(command);
	switch (commandType)
	{
		case CMD_PING:
			handlePing(client, params);
			break;
		default:
			sendToClient(client,
				":" + server.getServerName() + " " + ERR_UNKNOWNCOMMAND + " " +
				client.getNickname() + " " + command + " :Unknown command" + CRLF);
			break;
	}
}

std::vector<std::string> IrcCommands::splitMessage(const std::string& message)
{
	std::vector<std::string> parts;
	std::istringstream iss(message);
	std::string part;
	while (iss >> part)
	{
		if (part[0] == ':')
		{
			std::string remainder;
			std::getline(iss, remainder);
			part = part.substr(1) + remainder;
			parts.push_back(part);
			break;
		}
		parts.push_back(part);
	}
	return parts;
}

int IrcCommands::getCommandType(const std::string& command)
{
	if (command == "PING")
		return CMD_PING;
	if (command == "PASS")
		return CMD_PASS;
	if (command == "NICK")
		return CMD_NICK;
	if (command == "USER")
		return CMD_USER;
	if (command == "JOIN")
		return CMD_JOIN;
	if (command == "PRIVMSG")
		return CMD_PRIVMSG;
	return CMD_UNKNOWN;
}


/*_____________________________________________________________________________
								AUXILIARY FUNCTIONS
_____________________________________________________________________________*/


void IrcCommands::sendToClient(const Client& client, const std::string& message)
{
	send(client.getFd(), message.c_str(), message.length(), 0);
}


/*_____________________________________________________________________________
								IRC COMMANDS
_____________________________________________________________________________*/



void IrcCommands::handlePing(Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		sendToClient(client, ":" + server.getServerName() + " " + \
							ERR_NOORIGIN + " :No origin specified" + CRLF);
		return;
	}
	std::string response = ":" + server.getServerName() + " PONG " + \
							server.getServerName() + " :" + params[0] + CRLF;
	sendToClient(client, response);
}
