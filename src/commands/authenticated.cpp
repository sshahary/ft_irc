#include "../../include/server.hpp"

bool Client::isAuthenticated() const { return authenticated; }

void Client::setAuthenticated(bool status) { authenticated = status; }

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
	return NULL;  // If no client matches, return nullptr
}
