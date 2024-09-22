#include "../../include/server.hpp"

bool Client::isAuthenticated() const
{
	return authenticated;
}

void Client::setAuthenticated(bool status)
{
	authenticated = status;
}

bool Client::isRegistered() const
{
	return registered;
}

void Client::setRegistered(bool status)
{
	registered = status;
}


Client* Server::findClientByFd(int clientFd)
{
	// Iterate through the list of connected clients
	for (size_t i = 0; i < clients.size(); ++i)
	{
		// Check if the client's file descriptor matches
		if (clients[i].getFd() == clientFd)
		{
			return &clients[i];  // Return a pointer to the client
		}
	}
	return NULL;  // If no client matches, return NULL
}

bool Client::hasNickSet() const
{
	return nickSet;
}

void Client::setNickSet(bool status)
{
	nickSet = status;
}

std::string Client::getUsername() const
{
	return username;
}

void Client::setUsername(const std::string &user)
{
	username = user;
}

std::string Client::getRealName() const
{
	return realName;
}

void Client::setRealName(const std::string &realName)
{
	this->realName = realName;
}
