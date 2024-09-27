
#ifndef SERVER_HPP
# define SERVER_HPP

# include "Common.hpp"
# include "Client.hpp"

// Server class to manage server operations
class Server
{
	public:

		// // ============ orthodox canonical form ============================
		Server(int port, const std::string &password);
		~Server();

		void	start();						// Start the server
		void	stop();							// Stop the server

	private:

		void	createSocket();					// Create and bind server socket
		void	waitForConnections();			// Main server loop to handle clients
		void	acceptNewClient();				// Accept new client connection
		void	receiveData(int clientFd);		// Receive data from a client
		void	closeAllConnections();			// Close all open file descriptors
		void	removeClient(int clientFd);		// Remove client from the list

		int			serverPort;					// Port on which the server is running
		std::string	serverPassword;				// Password for the server
		int			serverFd;					// Server socket file descriptor
		bool		isRunning;					// Flag to control server loop

		std::vector<Client>				clients;	// List of connected clients
		std::vector<struct pollfd>		pollFds;	// Polling structures for clients
};

#endif
