
#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>

# include <vector>
# include <string>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdexcept>
# include <sstream>

# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>
# include <map>

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
		//sshahary
		Client(int fd, const std::string &ipAddress);
		

		// // ============ getters ============================================
		int			getFd() const;
		std::string	getIpAddress() const;
		//sshahary
		std::string	getNickname() const;

		// // ============ setters ============================================
		void		setFd(int fd);
		void		setIpAddress(const std::string &ipAddress);
		//sshahary
		void		setNickname(const std::string &nickname);

	private:

		int				clientFd;				// Client file descriptor
		std::string		ipAddress;				// Client IP address
		//sshahary
		std::string		nickname;				// Client nickname
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

		void 	handlePassCommand(int clientFd, const std::vector<std::string>& params);
		void	handleNickCommand(int clientFd, const std::vector<std::string>& params);

	private:

		void	createSocket();					// Create and bind server socket
		void	waitForConnections();			// Main server loop to handle clients
		void	acceptNewClient();				// Accept new client connection
		void	receiveData(int clientFd);		// Receive data from a client
		void	closeAllConnections();			// Close all open file descriptors
		void	removeClient(int clientFd);		// Remove client from the list

		//sshahary
		bool	isNicknameTaken(const std::string& nickname) const;  // Check if nickname is taken
		void	updateNicknameTracking(const std::string& oldNickname, const std::string& newNickname, int clientFd);
		void	removeNicknameTracking(const std::string& nickname);
		void	sendError(int clientFd, const std::string& errorCode, const std::string& message);

		int			serverPort;					// Port on which the server is running
		std::string	serverPassword;				// Password for the server
		int			serverFd;					// Server socket file descriptor
		bool		isRunning;					// Flag to control server loop

		std::vector<Client>				clients;	// List of connected clients
		std::vector<struct pollfd>		pollFds;	// Polling structures for clients
		//sshahary
		std::map<std::string, int> nicknames; // Map to track nicknames to client file descriptors
};

#endif
