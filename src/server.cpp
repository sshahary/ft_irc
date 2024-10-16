#include "Server.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "IrcException.hpp"
#include "IrcCommands.hpp"


/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

Server::Server(const Config &config) :	serverSocket(-1),
										isRunning(true),
										config(config),
										serverName("ircserv"),
										ircCommands(*this)
{
	Logger::info( "Server created with port: " + \
				Logger::intToString(config.getPort()) + \
				" and password: " + config.getPassword() );
}

Server::~Server() {}

/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/

void Server::stop()
{
	isRunning = false;
	
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		int clientFd = it->first; // Key (file descriptor) of the map entry
		Logger::connection("Closing connection with client: " + \
		Logger::intToString(clientFd));
		close(clientFd); // Close the client socket
	}

	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		delete it->second;
	}
	channels.clear();

	if (serverSocket != -1)
	{
		Logger::info("Shutting down the server: " + \
		Logger::intToString(serverSocket));
		close(serverSocket); // Close the server socket
	}
}

void Server::start()
{
	createSocket();
	while (isRunning)
	{
		int pollStatus = poll(&pollFds[0], pollFds.size(), -1);
		if (pollStatus == -1)
		{
			if (errno == EINTR)
				break;
			else
				throw IrcException("Polling error");
		}
		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocket)
					acceptNewClient();
				else
					processClientData(pollFds[i].fd);
			}
		}
	}
}

// Create and bind server socket
void Server::createSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
		throw IrcException("Failed to create socket");

	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw IrcException("Failed to set socket options");

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw IrcException("Failed to set non-blocking mode");

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(config.getPort());

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		throw IrcException("Failed to bind server socket");

	if (listen(serverSocket, SOMAXCONN) == -1)
		throw IrcException("Failed to listen on socket");

	struct pollfd serverPollFd;
	serverPollFd.fd = serverSocket;
	serverPollFd.events = POLLIN;
	pollFds.push_back(serverPollFd);

	Logger::connection("Server listening on port " + Logger::intToString(config.getPort()));
}

void Server::acceptNewClient()
{
	struct sockaddr_in clientAddr;
	pollfd clientPollFd;

	socklen_t clientLen = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
	if (clientSocket < 0) {
		Logger::error("Failed to accept client connection");
		return;
	}
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		Logger::error("Failed to set non-blocking mode for client");
		close(clientSocket);
		return ;
	}
	clientPollFd.fd = clientSocket;
	clientPollFd.events = POLLIN;
	pollFds.push_back(clientPollFd);
	clients[clientSocket] = Client(clientSocket);
	Logger::info("New client connected: " + std::to_string(clientSocket));
}

// Receive data from a connected client
/* void Server::processClientData(int clientFd)
{
	char	buffer[1024];

	memset(buffer, 0, sizeof(buffer));
	ssize_t bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived <= 0)
	{
		Logger::connection("Client disconnected: " + Logger::intToString(clientFd));
		removeClient(clientFd);
		close(clientFd);
	}
	else
	{
		
		buffer[bytesReceived] = '\0';
		Logger::chat("Client <" + Logger::intToString(clientFd) + "> sent: " + buffer);
		std::string message(buffer);
		ircCommands.ircCommandsDispatcher(clients[clientFd], message);
	}
} */

void Server::processClientData(int clientFd)
{
	char buffer[1024];
	ssize_t bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	
	if (bytesReceived <= 0)
	{
		Logger::connection("Client disconnected: " + Logger::intToString(clientFd));
		removeClient(clientFd);
		close(clientFd);
		return;
	}

	buffer[bytesReceived] = '\0';
	clients[clientFd].inputBuffer += std::string(buffer, bytesReceived);

	processClientBuffer(clientFd);
}

void Server::processClientBuffer(int clientFd)
{
	Client& client = clients[clientFd];
	size_t pos;

	while ((pos = client.inputBuffer.find('\n')) != std::string::npos)
	{
		std::string command = client.inputBuffer.substr(0, pos);
		
		// Remove '\r' if present (for \r\n line endings)
		if (!command.empty() && command[command.length() - 1] == '\r')
			command.erase(command.length() - 1);

		Logger::chat("Client <" + Logger::intToString(clientFd) + "> sent: " + command);
		
		// Process the complete command
		ircCommands.ircCommandsDispatcher(client, command);

		// Remove the processed command from the buffer
		client.inputBuffer.erase(0, pos + 1);
	}
}

// Method to remove a client from the server's lists
void Server::removeClient(int clientFd)
{
	for (size_t i = 0; i < pollFds.size(); ++i)
	{
		if (pollFds[i].fd == clientFd)
		{
			pollFds.erase(pollFds.begin() + i);
			break;
		}
	}
	// Remove client from the std::map<int, Client>
	std::map<int, Client>::iterator it = clients.find(clientFd);
	if (it != clients.end()) 
	{
		// Remove nickname mapping if set
		if (!it->second.getNickname().empty())
		{
			removeNickname(it->second.getNickname());
		}
		clients.erase(it); // Erase the client using the iterator
	}
}

const std::string& Server::getServerName() const
{
	return serverName;
}

const std::string& Server::getPassword() const
{
	return config.getPassword();
}

const std::map<int, Client>& Server::get_clients() const
{
	return clients;
}

/*_____________________________________________________________________________
								NICK COMMAND
_____________________________________________________________________________*/


bool Server::isNickInUse(const std::string& nickname)
{
	return nickToFd.find(nickname) != nickToFd.end();
}

void Server::updateNickname(Client& client, const std::string& oldNick, const std::string& newNick)
{
	if (!oldNick.empty())
		nickToFd.erase(oldNick);
	nickToFd[newNick] = client.getFd();
}

/*_____________________________________________________________________________
								NEW MEMBER FUNCTIONS
_____________________________________________________________________________*/

// Function to send raw messages to clients
void Server::sendRawMessage(int clientFd, const std::string& message)
{
	ssize_t totalSent = 0;
	ssize_t messageLength = message.length();
	const char* msg = message.c_str();

	while (totalSent < messageLength)
	{
		ssize_t sent = send(clientFd, msg + totalSent, messageLength - totalSent, 0);
		if (sent == -1)
		{
			Logger::error("Failed to send message to client FD " + Logger::intToString(clientFd));
			break;
		}
		totalSent += sent;
	}
}

// Function to retrieve a client by file descriptor
Client* Server::getClient(int fd)
{
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it != clients.end())
	{
		return &(it->second);
	}
	return nullptr;
}

// Function to retrieve a channel by name
Channel* Server::getChannel(const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		return it->second;
	}
	return nullptr;
}

// Function to add a channel
void Server::addChannel(Channel* channel)
{
	if (channel)
	{
		channels[channel->getName()] = channel;
		Logger::info("Channel created: " + channel->getName());
	}
}

void Server::removeChannel(const std::string& channelName) {
	std::map<std::string, Channel*>::iterator it = channels.find(channelName);
	if (it != channels.end()) {
		delete it->second;
		channels.erase(it);
		Logger::info("Channel removed: " + channelName);
	}
}

// Function to retrieve a client by nickname
Client* Server::getClientByNickname(const std::string& nickname)
{
	std::map<std::string, int>::iterator it = nickToFd.find(nickname);
	if (it != nickToFd.end())
	{
		return getClient(it->second);
	}
	return NULL;
}

// Function to add a nickname
void Server::addNickname(const std::string& nickname, int fd)
{
	nickToFd[nickname] = fd;
}

// Function to remove a nickname
void Server::removeNickname(const std::string& nickname)
{
	nickToFd.erase(nickname);
}

bool Server::isChannel(const std::string& channelName)
{
	return channels.find(channelName) != channels.end();
}
