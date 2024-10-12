
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
		case CMD_JOIN:
			handleJoin(client, params);
			break;
		case CMD_INVITE:
			handleInvite(client, params);
			break;
		case CMD_KICK:
			handleKick(client, params);
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
	if (command == "INVITE")
		return CMD_INVITE;
	if (command == "KICK")
		return CMD_KICK;
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
	if (client.isRegistered())
	{
		server.sendError(client.getFd(), ERR_ALREADYREGISTERED, ":You may not reregister");
		return;
	}
	if (!client.isAuthenticated())
	{
		server.sendError(client.getFd(), "ERR_NOTAUTHENTICATED", "You must authenticate using the PASS command and should has nickname.");
		std::cout << RED << "Client " << client.getFd() << " attempted to set username without authentication." << std::endl;
		return;
	}
	if (params.size() < 4)
	{
		server.sendError(client.getFd(), ERR_NEEDMOREPARAMS, ":Not enough parameters");
		return;
	}
	if (!client.hasNickSet())
	{
		server.sendError(client.getFd(), ERR_NONICKNAMEGIVEN, ":Nickname must be provided before USER");
		return;
	}
	client.setUsername(params[0]);
	client.setRealName(params[3]);
	if (client.isAuthenticated() && client.hasNickSet())
	{
		client.setRegistered(true);
		welcomeClient(client);
		std::cout << GRE << "Client " << client.getFd() << " registered with username '" << params[0] << "' and real name '" << params[3] << "'" << std::endl;
	}
	else
		std::cout << "Client provided USER, but is not yet authenticated or has no nickname (waiting for NICK/PASS)." << std::endl;

}

void IrcCommands::handleJoin(Client& client, const std::vector<std::string>& params){
    // Check if the client is fully registered
    if (!client.isRegistered()) {
        server.sendError(client.getFd(), ERR_NOTREGISTERED, ":You have not registered" + std::string(CRLF));
        return;
    }

    // Check if channel name is provided
    if (params.empty()) {
        server.sendError(client.getFd(), ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters" + std::string(CRLF));
        return;
    }

    std::string channelName = params[0];

    // Validate channel name (should start with '#' or '&')
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        server.sendError(client.getFd(), ERR_NOSUCHCHANNEL, channelName + " :No such channel" + std::string(CRLF));
        return;
    }

    // Get or create the channel
    Channel* channel = server.getChannel(channelName);
    if (!channel) {
        // Create a new channel
        channel = new Channel(channelName);
        server.addChannel(channel);
    }

    // Check if the client is already in the channel
    if (channel->isMember(client)) {
        std::string error = ":" + server.getServerName() + " " + "443 " + client.getNickname() + " " + channelName + " :You're already on that channel" + CRLF;
        server.sendRawMessage(client.getFd(), error);
        return;
    }

    // Check if the client was invited
    if (!channel->isInvited(client)) {
        // For simplicity, let's assume channels are invite-only if they have any invites
        // Alternatively, implement channel modes to determine invite-only status
        // Here, we'll allow joining without an invite
        // To enforce invite-only, uncomment the following lines:

        /*
        std::string error = ":" + server.getServerName() + " " + "473 " + client.getNickname() + " " + channelName + " :Cannot join channel (+i)" + CRLF;
        server.sendRawMessage(client.getFd(), error);
        return;
        */
    }

    // Add the client to the channel
    channel->addMember(client);
    channel->removeInvite(client); // Remove the invite as it's used

    // Send JOIN message to all channel members
    std::string joinMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " JOIN " + channelName + CRLF;
    channel->broadcast(joinMessage, &client);

    // Send topic if set
    if (!channel->getTopic().empty()) {
        std::string topicMessage = ":" + server.getServerName() + " " + RPL_TOPIC + " " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + CRLF;
        server.sendRawMessage(client.getFd(), topicMessage);
    }
    else {
        std::string noTopicMessage = ":" + server.getServerName() + " " + RPL_NOTOPIC + " " + client.getNickname() + " " + channelName + " :No topic is set" + CRLF;
        server.sendRawMessage(client.getFd(), noTopicMessage);
    }

    // Send NAMES list
    std::string namesMessage = ":" + server.getServerName() + " " + RPL_NAMREPLY + " " + client.getNickname() + " = " + channelName + " :" + channel->getMemberNames() + CRLF;
    server.sendRawMessage(client.getFd(), namesMessage);

    std::string endOfNamesMessage = ":" + server.getServerName() + " " + RPL_ENDOFNAMES + " " + client.getNickname() + " " + channelName + " :End of /NAMES list" + CRLF;
    server.sendRawMessage(client.getFd(), endOfNamesMessage);
}

void IrcCommands::handleInvite(Client& client, const std::vector<std::string>& params) {
    if (!client.isRegistered()) {
        server.sendError(client.getFd(), ERR_NOTREGISTERED, ":You have not registered" + std::string(CRLF));
        return;
    }

    if (params.size() < 2) {
        server.sendError(client.getFd(), ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters" + std::string(CRLF));
        return;
    }

    std::string nickname = params[0];
    std::string channelName = params[1];

    // Check if the nickname exists
    Client* targetClient = server.getClientByNickname(nickname);
    if (!targetClient) {
        std::string error = ":" + server.getServerName() + " " + "401 " + client.getNickname() + " " + nickname + " :No such nick/channel" + CRLF;
        server.sendRawMessage(client.getFd(), error);
        return;
    }

    // Check if the channel exists
    Channel* channel = server.getChannel(channelName);
    if (!channel) {
        std::string error = ":" + server.getServerName() + " " + "403 " + client.getNickname() + " " + channelName + " :No such channel" + CRLF;
        server.sendRawMessage(client.getFd(), error);
        return;
    }

    // Check if the client is a member of the channel
    if (!channel->isMember(client)) {
        std::string error = ":" + server.getServerName() + " " + "442 " + client.getNickname() + " " + channelName + " :You're not on that channel" + CRLF;
        server.sendRawMessage(client.getFd(), error);
        return;
    }

    // Invite the target client
    channel->addInvite(*targetClient);

    // Send invite message to the target client
    std::string inviteMessage = ":" + client.getNickname() + " INVITE " + nickname + " " + channelName + CRLF;
    server.sendRawMessage(targetClient->getFd(), inviteMessage);
}


void IrcCommands::handleKick(Client& client, const std::vector<std::string>& params) {
    // KICK <channel> <user> [<comment>]

    // 1. Registration Check
    if (!client.isRegistered()) {
        server.sendTargetError(client.getFd(), ERR_NOTREGISTERED, client.getNickname(), "You have not registered" + std::string(CRLF));
        return;
    }

    // 2. Parameter Validation
    if (params.size() < 2) {
        server.sendTargetError(client.getFd(), ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters" + std::string(CRLF));
        return;
    }

    std::string channelName = params[0];
    std::string targetNickname = params[1];
    std::string comment = (params.size() >= 3) ? params[2] : "No comment";

    // 3. Channel Validation
    Channel* channel = server.getChannel(channelName);
    if (!channel) {
        server.sendTargetError(client.getFd(), ERR_NOSUCHCHANNEL, channelName, "No such channel" + std::string(CRLF));
        return;
    }

    // 4. Client Membership Check
    if (!channel->isMember(client)) {
        server.sendTargetError(client.getFd(), "442", channelName, "You're not on that channel" + std::string(CRLF));
        return;
    }

    // 5. Target User Validation
    Client* targetClient = server.getClientByNickname(targetNickname);
    if (!targetClient) {
        server.sendTargetError(client.getFd(), "401", targetNickname, "No such nick/channel" + std::string(CRLF));
        return;
    }

    if (!channel->isMember(*targetClient)) {
        server.sendTargetError(client.getFd(), ERR_USERONCHANNEL, channelName, targetNickname + " :They are not on that channel" + std::string(CRLF));
        return;
    }

    // 6. Permission Check (Optional)
    // If implementing operator permissions, uncomment the following:
    /*
    if (!channel->isOperator(client)) {
        server.sendError(client.getFd(), "482", channelName, "You're not channel operator" + std::string(CRLF));
        return;
    }
    */

    // 7. Perform the Kick
    channel->removeMember(*targetClient);

    // 8. Broadcast the Kick
    std::stringstream kickMessageStream;
    kickMessageStream << ":" << client.getNickname() << "!user@" << "hostname"
                      << " KICK " << channelName << " " << targetNickname
                      << " :" << comment << CRLF;
    std::string kickMessage = kickMessageStream.str();
    channel->broadcast(kickMessage, &client);

    // 9. Notify the Kicked User
    server.sendRawMessage(targetClient->getFd(), kickMessage);
}
