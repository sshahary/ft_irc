
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
		case CMD_PASS:
			handlePass(client, params);
			break;
		case CMD_NICK:
			handleNick(client, params);
			break;
		case CMD_USER:
			handleUser(client, params);
			break;
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

void IrcCommands::broadcastToAll(const std::string& message)
{
	std::map<int, Client> clients = server.get_clients();
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
		sendToClient(it->second, message);
}

void IrcCommands::welcomeClient(Client& client)
{
	sendToClient(client, ":" + server.getServerName() + " " + RPL_WELCOME + " " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + CRLF);
	sendToClient(client, ":" + server.getServerName() + " " + RPL_YOURHOST + " " + client.getNickname() + " :Your host is " + server.getServerName() + ", running version 1.0" + CRLF);
	sendToClient(client, ":" + server.getServerName() + " " + RPL_CREATED + " " + client.getNickname() + " :This server was created " + CRLF);
	sendToClient(client, ":" + server.getServerName() + " " + RPL_MYINFO + " " + client.getNickname() + " " + server.getServerName() + " 1.0 " + CRLF);
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

void IrcCommands::handlePass(Client& client, const std::vector<std::string>& params)
{
	if (params.size() < 1 || params.empty())
	{
		server.sendError(client.getFd(),ERR_NEEDMOREPARAMS, "PASS :No password provided");
		return ;
	}
	if (client.isAuthenticated())
	{
		server.sendError(client.getFd(), ERR_ALREADYREGISTERED,  "PASS :You already validated PASS");
		return ;
	}
	if (params[0] == server.getPassword())
	{
		client.setAuthenticated(true);
		std::cout<< GRE << "Password is validated" << std::endl;
	}
	else
	{
		server.sendError(client.getFd(), ERR_PASSWDMISMATCH, "PASS :Incorrect password");
		std::cerr << RED << " invalid password" << std::endl;
	}
}

void IrcCommands::handleNick(Client& client, const std::vector<std::string>& params)
{
	// for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
	// 	std::cout << *it << std::endl;
	// if (params.empty()) {
	// 	throw IrcException(ERR_NONICKNAMEGIVEN + " :No nickname given");
	// }
	// std::string newNick = params[0];
	// if (newNick.length() > MAX_NICKNAME_LENGTH) {
	// 	throw IrcException(ERR_ERRONEUSNICKNAME + " " + newNick + " :Nickname is too long");
	// }
	// if (server.isNickInUse(newNick)) {
	// 	throw IrcException(ERR_NICKNAMEINUSE + " " + newNick + " :Nickname is already in use");
	// }
	// std::string oldNick = client.getNickname();
	// client.setNickname(newNick);
	// server.updateNickname(client, oldNick, newNick);
	// if (client.isRegistered()) {
	// 	broadcastToAll(":" + oldNick + " NICK " + newNick + CRLF);
	// }
	if (!client.isAuthenticated())
	{
		server.sendError(client.getFd(), "ERR_NOTAUTHENTICATED", "You must authenticate using the PASS command before setting a nickname.");
		std::cout << RED << "Client " << client.getFd() << " attempted to set nickname without authentication." << std::endl;
		return;
	}
	if (params.size() < 1 || params.empty())
	{
		std::cout<<"nick error";
		server.sendError(client.getFd(), ERR_NEEDMOREPARAMS, "NICK :Not enough parameters");
		return;
	}
	std::string newNickname = params[0];
	if (server.isNickInUse(newNickname))
	{
		server.sendError(client.getFd(), ERR_NICKNAMEINUSE, newNickname + " :Nickname is already in use");
		return;
	}
	std::string oldNickname = client.getNickname();
	client.setNickname(newNickname);
	client.setNickSet(true);
	server.updateNickname(client, oldNickname, newNickname);
	std::cout << GRE << "Client " << client.getFd() << " set nickname to '" << newNickname << "'" << std::endl;
	if (client.isRegistered())
	{
		broadcastToAll(":" + oldNickname + " NICK " + newNickname + CRLF);
	}
	if (client.isAuthenticated())
	{
		std::cout << GRE << "Client has provided NICK, waiting for USER to fully register." << std::endl;
	}

}

void IrcCommands::handleUser(Client& client, const std::vector<std::string>& params)
{
	// for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
	// 	std::cout << *it << std::endl;
	// if (params.size() < 4) {
	// 	throw IrcException(ERR_NEEDMOREPARAMS + " USER :Not enough parameters");
	// }
	// if (client.isRegistered()) {
	// 	throw IrcException(ERR_ALREADYREGISTERED + " :You may not reregister");
	// }
	// client.setUsername(params[0]);
	// client.setRealName(params[3]);
	// if (client.isAuthenticated() && !client.getNickname().empty())
	// {
	// 	client.setRegistered(true);
	// 	welcomeClient(client);
	// }
	
}
