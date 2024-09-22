#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include <set>
#include "server.hpp"
class Commands
{
	public:
	// handleCommandPass();
};
#endif

class Channel {
private:
	std::string channelName;
	std::string topic;
	std::string password;
	int userLimit;
	std::set<int> clients; // Store the file descriptors of clients in the channel
	std::set<int> operators; // Store operator file descriptors
	bool inviteOnly;
	bool topicRestricted;
	
public:
	Channel(const std::string &name) : channelName(name), inviteOnly(false), topicRestricted(false), userLimit(0) {}

	bool isClientInChannel(int clientFd) const { return clients.find(clientFd) != clients.end(); }
	bool isOperator(int clientFd) const { return operators.find(clientFd) != operators.end(); }

	void addClient(int clientFd) { clients.insert(clientFd); }
	void removeClient(int clientFd) { clients.erase(clientFd); }

	void setTopic(const std::string &newTopic) { topic = newTopic; }
	std::string getTopic() const { return topic; }

	// Channel Mode Management (e.g., invite-only, topic restricted, etc.)
	void setInviteOnly(bool value) { inviteOnly = value; }
	bool isInviteOnly() const { return inviteOnly; }

	void setTopicRestricted(bool value) { topicRestricted = value; }
	bool isTopicRestricted() const { return topicRestricted; }

	// Operators Management
	void addOperator(int clientFd) { operators.insert(clientFd); }
	void removeOperator(int clientFd) { operators.erase(clientFd); }

	// Password/Key Management
	void setPassword(const std::string &pass) { password = pass; }
	bool checkPassword(const std::string &pass) const { return password == pass; }

	// User limit management
	void setUserLimit(int limit) { userLimit = limit; }
	bool isUserLimitExceeded() const { return userLimit > 0 && clients.size() >= userLimit; }
};
