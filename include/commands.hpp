#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include <set>
#include "server.hpp"
// class Commands
// {
// 	public:
// 	// handleCommandPass();
// };

class Channel
{
private:
	std::string channelName;
	std::string topic;
	std::string password;
	int		userLimit;        // Declared before inviteOnly
	bool	inviteOnly;      // Declared after userLimit
	bool	topicRestricted;
	std::set<int> clients; // Store the file descriptors of clients in the channel
	std::set<int> operators; // Store operator file descriptors

public:
	// Constructor
	Channel(const std::string &name);

	// Client Management
	bool isClientInChannel(int clientFd) const;
	void addClient(int clientFd);
	void removeClient(int clientFd);
	std::string getName() const;

	// Operator Management
	bool isOperator(int clientFd) const;
	void addOperator(int clientFd);
	void removeOperator(int clientFd);

	// Topic Management
	void setTopic(const std::string &newTopic);
	std::string getTopic() const;

	// Channel Mode Management
	void setInviteOnly(bool value);
	bool isInviteOnly() const;

	void setTopicRestricted(bool value);
	bool isTopicRestricted() const;

	// Password Management
	void setPassword(const std::string &pass);
	bool checkPassword(const std::string &pass) const;

	// User Limit Management
	void setUserLimit(int limit);
	bool isUserLimitExceeded() const;
};

#endif


