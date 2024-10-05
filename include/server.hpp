
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
		void	acceptNewClient();				// Accept new client connection
		void	processClientData(int clientFd);	// Receive data from a client
		// void	closeAllConnections();			// Close all open file descriptors
		void	removeClient(int clientFd);		// Remove client from the list

		int								serverSocket;			// Server socket file descriptor
		bool							isRunning;				// Flag to control server loop
		Config							config;
		std::vector<struct pollfd>		pollFds;				// Polling structures for clients
		// std::vector<Client>				clients;				// List of connected clients
		std::map<int, Client>			clients;
		
		// int			serverPort;					// Port on which the server is running
		// std::string	serverPassword;				// Password for the server
		
};

#endif
