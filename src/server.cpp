#include "Server.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "IrcException.hpp"

/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

Server::Server(const Config &config) :	serverSocket(-1),
										isRunning(true),
										config(config)
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
void Server::processClientData(int clientFd)
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
		clients.erase(it); // Erase the client using the iterator
	}
}

const std::string& Server::getServerName() const
{
	return serverName;
}
