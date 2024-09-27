
#include "Server.hpp"

// ============ Server - orthodox canonical form ==============================

Server::Server(const Config& config) :	config(config), serverSocket(-1),
										isRunning(false)
{
	std::cout << "Server created with port: " << config.getPort() << " and password: " << config.getPassword() << std::endl;
}

Server::~Server()
{
	stop();
}














// Server class implementation ================================================

// Method to close all client connections and the server socket fd
void Server::closeAllConnections()
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		std::cout << RED << "Closing connection with client: " << clients[i].getFd() << WHI << std::endl;
		close(clients[i].getFd());
	}
	if (serverFd != -1)
	{
		std::cout << RED << "Shutting down server: " << serverFd << WHI << std::endl;
		close(serverFd);
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

// Receive data from a connected client
void Server::receiveData(int clientFd)
{
	char	buffer[1024];

	memset(buffer, 0, sizeof(buffer));
	ssize_t bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived <= 0)
	{
		std::cout << RED << "Client disconnected: " << clientFd << WHI << std::endl;
		removeClient(clientFd);
		close(clientFd);
	}
	else
	{
		buffer[bytesReceived] = '\0';
		std::cout << YEL << "Client <" << clientFd << "> sent: " << WHI << buffer << std::endl;
		// process received data
		// e.g.: processClientRequest(clientFd, buffer);
	}
}

// Accept a new client connection
void Server::acceptNewClient()
{
	Client				newClient;
	struct sockaddr_in	clientAddress;
	struct pollfd		clientPollFd;
	
	socklen_t clientLen = sizeof(clientAddress);

	int clientFd = accept(serverFd, (struct sockaddr *)&clientAddress, &clientLen);
	if (clientFd == -1)
	{
		std::cerr << "Failed to accept client connection" << std::endl;
		return ;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Failed to set non-blocking mode for client" << std::endl;
		close(clientFd);
		return ;
	}

	newClient.setFd(clientFd);
	newClient.setIpAddress(inet_ntoa(clientAddress.sin_addr));
	clients.push_back(newClient);

	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN;
	pollFds.push_back(clientPollFd);

	std::cout << GRE << "Client connected: " << newClient.getIpAddress() << WHI << std::endl;
}

// Main server loop to wait for and handle connections
void Server::waitForConnections()
{
	while (isRunning)
	{
		int pollStatus = poll(&pollFds[0], pollFds.size(), -1);
		if (pollStatus == -1)
			throw std::runtime_error("Polling error");

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverFd)
					acceptNewClient();
				else
					receiveData(pollFds[i].fd);
			}
		}
	}
}

// Create and bind server socket
void Server::createSocket()
{
	serverFd = socket(AF_INET, SOCK_STREAM, 0);

	if (serverFd == -1)
		throw std::runtime_error("Failed to create socket");

	int opt = 1;
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Failed to set socket options");

	if (fcntl(serverFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Failed to set non-blocking mode");

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("Failed to bind server socket");

	if (listen(serverFd, SOMAXCONN) == -1)
		throw std::runtime_error("Failed to listen on socket");

	struct pollfd serverPollFd;
	serverPollFd.fd = serverFd;
	serverPollFd.events = POLLIN;
	pollFds.push_back(serverPollFd);

	std::cout << GRE << "Server listening on port " << serverPort << WHI << std::endl;
}

void Server::start()
{
	createSocket();
	waitForConnections();
}

void Server::stop()
{
	isRunning = false;
	closeAllConnections();
}
