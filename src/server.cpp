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

Server::~Server()
{
	// stop();
}

/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/

void Server::stop()
{
	isRunning = false;
	// closeAllConnections();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		Logger::connection("Closing connection with client: " + \
		Logger::intToString(clients[i].getFd()));
		close(clients[i].getFd());
	}
	if (serverSocket != -1)
	{
		Logger::info("Shutting down the server: " + \
		Logger::intToString(serverSocket));
		close(serverSocket);
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
	// closeAllConnections();
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

// Accept a new client connection
void Server::acceptNewClient()
{
	Client				newClient;
	struct sockaddr_in	clientAddress;
	struct pollfd		clientPollFd;

	socklen_t clientLen = sizeof(clientAddress);
	int clientFd = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLen);
	if (clientFd == -1)
	{
		Logger::error("Failed to accept client connection");
		return ;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		Logger::error("Failed to set non-blocking mode for client");
		close(clientFd);
		return ;
	}

	newClient.setFd(clientFd);
	newClient.setIpAddress(inet_ntoa(clientAddress.sin_addr));
	clients.push_back(newClient);

	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN;
	pollFds.push_back(clientPollFd);

	Logger::connection("Client connected: " + newClient.getIpAddress());
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
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getFd() == clientFd)
		{
			clients.erase(clients.begin() + i);
			break;
		}
	}
}

/* // Method to close all client connections and the server socket fd
void Server::closeAllConnections()
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		Logger::connection("Closing connection with client: " + \
		Logger::intToString(clients[i].getFd()));
		close(clients[i].getFd());
	}
	if (serverSocket != -1)
	{
		Logger::info("Shutting down the server: " + \
		Logger::intToString(serverSocket));
		close(serverSocket);
	}
} */
