
#include "IrcCommands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Constants.hpp"
#include "Channel.hpp"
#include <sstream>
#include "IrcException.hpp"
#include "Logger.hpp"


/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/


IrcCommands::IrcCommands(Server& server) : server(server) {}

IrcCommands::~IrcCommands() {}


/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/


std::string normalizeLineEndings(const std::string& message) {
	std::string normalizedMessage = message;
	size_t pos = 0;
	while ((pos = normalizedMessage.find("\n", pos)) != std::string::npos) {
		if (pos == 0 || normalizedMessage[pos - 1] != '\r') {
			normalizedMessage.replace(pos, 1, "\r\n");
		}
		pos += 2; // Move past the new \r\n
	}
	return normalizedMessage;
}



void IrcCommands::ircCommandsDispatcher(Client& client, const std::string& message)
{
	std::string normalizedMessage = normalizeLineEndings(message);

	// Split the incoming message into command and parameters
	std::vector<std::string> messageParts = splitMessage(normalizedMessage);
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
		case CMD_TOPIC:
			handleTopic(client, params);
			break;
		case CMD_MODE:
			handleMode(client, params);
			break;
		case CMD_PRIVMSG:
			handlePrivmsg(client, params);
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
	if (command == "TOPIC")
		return CMD_TOPIC;
	if (command == "MODE")
		return CMD_MODE;
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
		Logger::error("No password provided");
		return ;
	}
	if (client.isAuthenticated())
	{
		server.sendError(client.getFd(), ERR_ALREADYREGISTERED,  "PASS :You already validated PASS");
		Logger::info("You already validated PASS");
		return ;
	}
	if (params[0] == server.getPassword())
	{
		client.setAuthenticated(true);
		Logger::connection("Password is validated");
	}
	else
	{
		server.sendError(client.getFd(), ERR_PASSWDMISMATCH, "PASS :Incorrect password");
		Logger::error("Invalid password");
	}
}

void IrcCommands::handleNick(Client& client, const std::vector<std::string>& params)
{
	if (!client.isAuthenticated())
	{
		server.sendError(client.getFd(), "ERR_NOTAUTHENTICATED", "You must authenticate using the PASS command before setting a nickname.");
		Logger::error("Attempted to set nickname without authentication.");
		return;
	}
	if (params.size() < 1 || params.empty())
	{
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
		Logger::connection("Client has provided NICK, waiting for USER to fully register.");

}

void IrcCommands::handleUser(Client& client, const std::vector<std::string>& params)
{
	if (client.isRegistered())
	{
		server.sendError(client.getFd(), ERR_ALREADYREGISTERED, ":You may not reregister");
		Logger::error("You may not reregister");
		return;
	}
	if (!client.isAuthenticated())
	{
		server.sendError(client.getFd(), "ERR_NOTAUTHENTICATED", "You must authenticate using the PASS command and should has nickname.");
		Logger::error("Attempted to set username without authentication.");
		return;
	}
	if (params.size() < 4)
	{
		server.sendError(client.getFd(), ERR_NEEDMOREPARAMS, ":Not enough parameters");
		Logger::error("Not enough parameters");
		return;
	}
	if (!client.hasNickSet())
	{
		server.sendError(client.getFd(), ERR_NONICKNAMEGIVEN, ":Nickname must be provided before USER");
		Logger::error("Nickname must be provided before USER");
		return;
	}
	client.setUsername(params[0]);
	client.setRealName(params[3]);
	if (client.isAuthenticated() && client.hasNickSet())
	{
		client.setRegistered(true);
		welcomeClient(client);
		Logger::connection("registered with user");
	}
	else
		Logger::error("Client provided USER, but is not yet authenticated or has no nickname (waiting for NICK/PASS).");

}

void IrcCommands::handlePrivmsg(Client& client, const std::vector<std::string>& params)
{
	if (params.empty() || params.size() < 2)
	{
		server.sendError(client.getFd(), ERR_NOTEXTTOSEND, ":No text to send");
		Logger::error("PRIVMSG: No text to send");
		return;
	}

	Logger::info("Received PRIVMSG command with params: " + params[0] + " " + params[1]);

	std::string recipient = params[0];
	std::string message;

	// Combine all remaining parts of the message into a single string
	for (size_t i = 1; i < params.size(); ++i)
	{
		if (i > 1) message += " ";  // Add space between message parts
		message += params[i];
	}

	// Check if the recipient is a valid channel or nickname
	if (recipient[0] == '#' || server.isChannel(recipient))  // Handle both cases for channel names with/without #
	{
		Channel* channel = server.getChannel(recipient);

		if (!channel)
		{
			server.sendError(client.getFd(), ERR_NOSUCHNICK, recipient + " :No such channel");
			Logger::error("PRIVMSG: No such channel '" + recipient + "'");
			return;
		}

		if (!channel->isMember(&client))
		{
			server.sendError(client.getFd(), ERR_CANNOTSENDTOCHAN, recipient + " :Cannot send to channel");
			Logger::error("PRIVMSG: Client is not a member of channel '" + recipient + "'");
			return;
		}

		// Broadcast the message to all members of the channel, except the sender
		std::string fullMessage = ":" + client.getNickname() + " PRIVMSG " + recipient + " :" + message + CRLF;
		channel->broadcastMessage(fullMessage, &client);
	}
	else
	{
		Client* targetClient = server.getClientByNickname(recipient);
		if (!targetClient)
		{
			server.sendError(client.getFd(), ERR_NOSUCHNICK, recipient + " :No such nickname");
			Logger::error("PRIVMSG: No such nickname '" + recipient + "'");
			return;
		}

		// Send the message to the target client
		std::string fullMessage = ":" + client.getNickname() + " PRIVMSG " + recipient + " :" + message + CRLF;
		sendToClient(*targetClient, fullMessage);
	}
}




void IrcCommands::handleJoin(Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		sendToClient(client, ":ircserv 451 " + client.getNickname() + " :You have not registered" + CRLF); // ERR_NOTREGISTERED
		return;
	}

	if (params.empty()) {
		sendToClient(client, ":ircserv 461 " + client.getNickname() + " JOIN :Not enough parameters" + CRLF); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string channelName = params[0];
	 std::string channelKey = (params.size() > 1) ? params[1] : ""; // Get the password if provided
	Channel* channel = server.getChannel(channelName);

	if (!channel) {
		// Create the channel if it doesn't exist
		channel = new Channel(channelName);
		server.addChannel(channel);
	} else {
		// Check if the client is already in the channel
		if (channel->isClient(&client)) {
			sendToClient(client, ":ircserv 443 " + client.getNickname() + " " + channelName + " :You're already in that channel" + CRLF); // ERR_USERONCHANNEL
			return;
		}

				// Password check
		if (channel->hasKey() && !channel->checkKey(channelKey)) {
			sendToClient(client, ":ircserv 475 " + client.getNickname() + " " + channelName + " :Cannot join channel (incorrect password)\r\n");
			return;
		}

		// Invite-only check
		if (channel->isInviteOnly() && !channel->isInvited(&client)) {
			sendToClient(client, ":ircserv 473 " + client.getNickname() + " " + channelName + " :Cannot join channel (+i) - you must be invited" + CRLF); // ERR_INVITEONLYCHAN
			return;
		}
		// Channel full check
		if (channel->isFull()) {
			sendToClient(client, ":ircserv 471 " + client.getNickname() + " " + channelName + " :Cannot join channel (+l) - channel is full" + CRLF); // ERR_CHANNELISFULL
			return;
		}
	}

	// Add the client to the channel
	channel->addClient(&client);

	// Broadcast the JOIN message to all members of the channel
	std::string joinMessage = ":" + client.getNickname() + "!" + client.getUsername() + client.getHostname() + " JOIN :" + channelName + CRLF;
	sendToClient(client, joinMessage);
	channel->broadcastMessage(joinMessage, NULL);

	// Topic information
	if (channel->hasTopic()) {
		std::string topicMessage = ":" + server.getServerName() + " 332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + CRLF;
		sendToClient(client, topicMessage); // RPL_TOPIC
	} else {
		std::string noTopicMessage = ":" + server.getServerName() + " 331 " + client.getNickname() + " " + channelName + " :No topic is set" + CRLF;
		sendToClient(client, noTopicMessage); // RPL_NOTOPIC
	}

	// NAMES list
	std::string namesMessage = ":" + server.getServerName() + " 353 " + client.getNickname() + " = " + channelName + " :" + channel->getMemberNames() + CRLF;
	sendToClient(client, namesMessage); // RPL_NAMREPLY

	// End of NAMES list
	std::string endOfNamesMessage = ":" + server.getServerName() + " 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list" + CRLF;
	sendToClient(client, endOfNamesMessage); // RPL_ENDOFNAMES
}


void IrcCommands::handleInvite(Client& client, const std::vector<std::string>& params) {
	if (!client.isRegistered()) {
		server.sendError(client.getFd(), "451", ":You have not registered" + CRLF); // ERR_NOTREGISTERED
		return;
	}

	if (params.size() < 2) {
		server.sendError(client.getFd(), "461", "INVITE :Not enough parameters" + CRLF); // ERR_NEEDMOREPARAMS
		return;
	}

	std::string nickname = params[0];
	std::string channelName = params[1];

	// Check if the channel exists
	Channel* channel = server.getChannel(channelName);
	if (!channel) {
		server.sendError(client.getFd(), "403", channelName + " :No such channel" + CRLF); // ERR_NOSUCHCHANNEL
		return;
	}

	// Check if the client is an operator in the channel
	if (!channel->isOperator(&client)) {
		server.sendError(client.getFd(), "482", channelName + " :You're not a channel operator" + CRLF); // ERR_CHANOPRIVSNEEDED
		return;
	}

	// Check if the target user exists
	Client* targetClient = server.getClientByNickname(nickname);
	if (!targetClient) {
		server.sendError(client.getFd(), "401", nickname + " :No such nick/channel" + CRLF); // ERR_NOSUCHNICK
		return;
	}

	// Check if the inviter is already a member of the channel
	if (!channel->isMember(&client)) {
		server.sendError(client.getFd(), "442", channelName + " :You're not on that channel" + CRLF); // ERR_NOTONCHANNEL
		return;
	}

	// Invite the target client
	channel->addInvite(targetClient);

	// Notify the invited user
	std::string inviteMessage = ":" + client.getNickname() + " INVITE " + targetClient->getNickname() + " :" + channelName + CRLF;
	sendToClient(*targetClient, inviteMessage);

	// Notify the inviter of the successful invite
	std::string successMessage = ":" + server.getServerName() + " 341 " + client.getNickname() + " " + targetClient->getNickname() + " " + channelName + CRLF;
	sendToClient(client, successMessage); // RPL_INVITING
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
	if (!channel->isMember(&client)) {
		server.sendTargetError(client.getFd(), "442", channelName, "You're not on that channel" + std::string(CRLF));
		return;
	}

	// 5. Target User Validation
	Client* targetClient = server.getClientByNickname(targetNickname);
	if (!targetClient) {
		server.sendTargetError(client.getFd(), "401", targetNickname, "No such nick/channel" + std::string(CRLF));
		return;
	}

	if (!channel->isMember(targetClient)) {
		server.sendTargetError(client.getFd(), ERR_USERONCHANNEL, channelName, targetNickname + " :They are not on that channel" + std::string(CRLF));
		return;
	}

	// 6. Permission Check (Optional)
	// If implementing operator permissions, uncomment the following:
	if (!channel->isOperator(&client)) {
		server.sendTargetError(client.getFd(), "482", channelName, "You're not channel operator" + std::string(CRLF));
		return;
	}


	// 7. Perform the Kick
	channel->removeMember(targetClient);

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

void IrcCommands::handleTopic(Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered()) {
		sendToClient(client, ":ircserv 451 " + client.getNickname() + " :You have not registered" + CRLF);
		return;
	}

	if (params.size() < 1) {
		sendToClient(client, ":ircserv 461 " + client.getNickname() + " TOPIC :Not enough parameters" + CRLF);
		return;
	}

	std::string channelName = params[0];
	Channel* channel = server.getChannel(channelName);

	if (!channel) {
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel" + CRLF);
		return;
	}

	if (params.size() == 1) { // View topic
		if (channel->hasTopic()) {
			sendToClient(client, ":ircserv 332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + CRLF);
			sendToClient(client, ":ircserv 333 " + client.getNickname() + " " + channelName + " " + channel->getTopicSetter() + " " + std::to_string(channel->getTopicTimestamp()) + CRLF);
		} else {
			sendToClient(client, ":ircserv 331 " + client.getNickname() + " " + channelName + " :No topic is set" + CRLF);
		}
	} else { // Change topic
		if (channel->isTopicRestricted() && !channel->isOperator(&client)) {
			sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + channelName + " :You're not a channel operator" + CRLF);
		} else {
			std::string newTopic = params[1];
			for (size_t i = 2; i < params.size(); ++i) {
				newTopic += " " + params[i];
			}

			channel->setTopic(newTopic);
			channel->setTopicSetter(client.getNickname());
			channel->setTopicTimestamp(time(NULL));

			// Send topic change message to all users in the channel
			std::string topicMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " TOPIC " + channelName + " :" + newTopic + CRLF;
			channel->broadcastMessage(topicMessage, NULL);
		}
	}
}

void IrcCommands::handleMode(Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		sendToClient(client, ":ircserv 451 " + client.getNickname() + " :You have not registered" + CRLF);
		return;
	}

	if (params.size() < 2)
	{
		sendToClient(client, ":ircserv 461 " + client.getNickname() + " MODE :Not enough parameters" + CRLF);
		return;
	}

	std::string channelName = params[0];
	Channel* channel = server.getChannel(channelName);

	if (!channel) {
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel" + CRLF);
		return;
	}

	if (!channel->isOperator(&client)) {
		sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + channelName + " :You're not a channel operator" + CRLF);
		return;
	}

	std::string modeFlags = params[1];
	bool adding = true; // true for adding modes, false for removing
	size_t paramIndex = 2; // Start index for additional parameters

	for (char mode : modeFlags) {
		switch (mode) {
			case '+': adding = true; break;
			case '-': adding = false; break;
			case 'i':
				channel->setInviteOnly(adding);
				break;
			case 't':
				channel->setTopicRestricted(adding);
				break;
			case 'k':
				if (adding) {
					if (paramIndex >= params.size()) {
						sendToClient(client, ":ircserv 461 " + client.getNickname() + " MODE :Not enough parameters for +k" + CRLF);
						return;
					}
					std::cout << "Setting channel key for " << channelName << " to " << params[paramIndex] << std::endl;
					channel->setKey(params[paramIndex++]);
				} else {
					channel->removeKey();
				}
				break;
			case 'o': {
				if (paramIndex >= params.size()) {
					sendToClient(client, ":ircserv 461 " + client.getNickname() + " MODE :Not enough parameters for +o/-o" + CRLF);
					return;
				}
				Client* target = server.getClientByNickname(params[paramIndex++]);
				if (target) {
					if (adding) {
						channel->addOperator(target);
					} else {
						channel->removeOperator(target);
					}
				}
				break;
			}
			case 'l':
				if (adding) {
					if (paramIndex >= params.size()) {
						sendToClient(client, ":ircserv 461 " + client.getNickname() + " MODE :Not enough parameters for +l" + CRLF);
						return;
					}
					channel->setUserLimit(std::stoi(params[paramIndex++]));
				} else {
					channel->removeUserLimit();
				}
				break;
			default:
				sendToClient(client, ":ircserv 472 " + client.getNickname() + " " + mode + " :is unknown mode char to me" + CRLF);
				return;
		}
	}
	channel->broadcastMessage(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " MODE " + channelName + " :" + modeFlags + CRLF, &client);
}
