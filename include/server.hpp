
#ifndef SERVER_HPP
# define SERVER_HPP

# include "Common.hpp"
# include "Client.hpp"
# include "Config.hpp"

class Server
{
	public:

		Server(const Config& config);
		~Server();

		void	start();
		void	stop();

	private:

		void	createSocket();					// Create and bind server socket
		void	waitForConnections();			// Main server loop to handle clients
		void	acceptNewClient();				// Accept new client connection
		void	receiveData(int clientFd);		// Receive data from a client
		void	closeAllConnections();			// Close all open file descriptors
		void	removeClient(int clientFd);		// Remove client from the list

		Config							config;
		int								serverSocket;			// Server socket file descriptor
		std::vector<struct pollfd>		pollFds;				// Polling structures for clients
		std::map<int, Client>			clients;				// List of connected clients
		bool							isRunning;				// Flag to control server loop
		
		// int			serverPort;					// Port on which the server is running
		// std::string	serverPassword;				// Password for the server
};

#endif
