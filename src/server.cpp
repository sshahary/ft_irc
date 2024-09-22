
#include "server.hpp"

// ============ Client - orthodox canonical form ==============================

Client::Client() : clientFd(-1), ipAddress(""), nickname("") {}

Client::Client(int fd, const std::string &ipAddress)			//sshahary
	: clientFd(fd), ipAddress(ipAddress), nickname("") {}
// Client class implementation ================================================

int Client::getFd() const { return clientFd; }

void Client::setFd(int fd) { clientFd = fd; }

std::string Client::getIpAddress() const { return ipAddress; }

void Client::setIpAddress(const std::string &ip) { ipAddress = ip; }

//sshahary

std::string Client::getNickname() const { return nickname; }

void Client::setNickname(const std::string &nick) { nickname = nick; }

// ============ Server - orthodox canonical form ==============================

// Server::Server(int port) : serverPort(port), serverFd(-1), isRunning(true) {}
Server::Server(int port, const std::string &password) : serverPort(port), 
														serverPassword(password), 
														serverFd(-1), 
														isRunning(true)
{
	std::cout << "Server created with port: " << serverPort << " and password: " << serverPassword << std::endl;
}

Server::~Server() {stop();}

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
	// Find and remove the client from the nickname tracking
	Client* client = findClientByFd(clientFd);
	if (client)
	{
		if (!client->getNickname().empty())
			removeNicknameTracking(client->getNickname());
	}
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
	close(clientFd);
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
		 // Find the client and check if they are authenticated
		Client* client = findClientByFd(clientFd);
		if (client == NULL)
			return;
		std::string command;
		std::vector<std::string> params;
		std::istringstream iss(buffer);
		iss >> command;

		std::string param;
		while (iss >> param)
			params.push_back(param);

		// Handle the parsed command
		 if (command == "PASS")
			handlePassCommand(clientFd, params);
		else if (command == "NICK")
			handleNickCommand(clientFd, params);
		else if (command == "USER")
			handleUserCommand(clientFd, params);
		// else if (!client->isRegistered())
		// 	sendError(clientFd, "ERR_NOTREGISTERED", "You must authenticate and register before sending other commands");
		// else
		// 	sendError(clientFd, "ERR_UNKNOWNCOMMAND", "Unknown command or not implemented");

		// Send response back to the client (optional, based on command processing result)
		// For example, you can return acknowledgment or an error message depending on the command handling logic.
		std::string response = "Command processed successfully"; // Placeholder response
		if (send(clientFd, response.c_str(), response.length(), 0) == -1)
		{
			std::cout << "send() error: " << strerror(errno) << std::endl;
		}
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

