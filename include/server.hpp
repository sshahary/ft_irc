
#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>

# include <vector>
# include <string>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdexcept>

# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>

// Color constants for output
# define RED "\033[1;31m"
# define WHI "\033[0;37m"
# define GRE "\033[1;32m"
# define YEL "\033[1;33m"

// Client class to handle individual client connections
class Client
{
	public:

		// // ============ orthodox canonical form ============================
		Client();
		

		// // ============ getters ============================================
		int			getFd() const;
		std::string	getIpAddress() const;

		// // ============ setters ============================================
		void		setFd(int fd);
		void		setIpAddress(const std::string &ipAddress);

	private:

		int				clientFd;				// Client file descriptor
		std::string		ipAddress;				// Client IP address
};

// Server class to manage server operations
class Server
{
	public:

		// // ============ orthodox canonical form ============================
		Server(int port, const std::string &password);
		~Server();

		void	start();						// Start the server
		void	stop();							// Stop the server
		// Commands

		void handlePassCommand(int clientFd, const std::vector<std::string>& params);

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
