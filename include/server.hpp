
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
# include "commands.hpp"

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
		std::string	getUsername() const;
		std::string	getRealName() const;
		std::string	getNickname() const;
		bool		isAuthenticated() const;
		bool		isRegistered() const;
		bool		hasNickSet() const;

		// // ============ setters ============================================
		void		setFd(int fd);
		void		setIpAddress(const std::string &ipAddress);
		//sshahary
		void		setUsername(const std::string &user);
		void		setRealName(const std::string &realName);
		void		setNickname(const std::string &nickname);
		void		setAuthenticated(bool status);
		void		setRegistered(bool status);
		void		setNickSet(bool status);


	private:

		int				clientFd;				// Client file descriptor
		std::string		ipAddress;				// Client IP address
		//sshahary
		std::string username;
		std::string realName;
		std::string		nickname;				// Client nickname
		bool			authenticated;
		bool			registered;				// For tracking if both NICK and USER commands are sent
		bool			nickSet; // New flag to track if NICK is set
};

class Channel;

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

		void	handlePassCommand(int clientFd, const std::vector<std::string>& params);
		void	handleNickCommand(int clientFd, const std::vector<std::string>& params);
		void	handleUserCommand(int clientFd, const std::vector<std::string>& params);

		void		handleJoinCommand(int clientFd, const std::vector<std::string>& params);
		void		handleKickCommand(int clientFd, const std::vector<std::string>& params);
		//Utility Methods
		std::string	getChannelUsers(Channel* channel) const;
		Channel*	findChannelByName(const std::string& channelName);
		void		broadcastToChannel(Channel* channel, const std::string& message, int excludeFd);
		std::string	getClientNickname(int clientFd) const;
		void		sendToClient(int clientFd, const std::string& message);
		void		sendToChannel(const std::string& channelName, const std::string& message, int excludeFd);

		Client*		findClientByNickname(const std::string &nickname);
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
		//Messages

		void	sendError(int clientFd, const std::string& errorCode, const std::string& message);
		void sendMessage(int clientFd, const std::string& command, const std::string& message, const std::string& prefix);
		void	sendWelcomeMessage(int clientFd, const std::string& nickname);
		void	sendSuccessMessage(int clientFd, const std::string& message);
		Client*		findClientByFd(int clientFd);	// Find client by file descriptor


		int			serverPort;					// Port on which the server is running
		std::string	serverPassword;				// Password for the server
		int			serverFd;					// Server socket file descriptor
		bool		isRunning;					// Flag to control server loop
		std::string	serverName;

		std::vector<Client>				clients;	// List of connected clients
		std::vector<struct pollfd>		pollFds;	// Polling structures for clients
		//sshahary
		std::map<std::string, int>		nicknames; // Map to track nicknames to client file descriptors
		std::vector<Channel*> channels; // Stores pointers to Channel objects
};

#endif
