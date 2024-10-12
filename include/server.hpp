
#ifndef SERVER_HPP
# define SERVER_HPP

# include "Common.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Config.hpp"
# include "IrcCommands.hpp"
#include <map>
#include <string>
#include <vector>
#include <poll.h>

class Channel;

class Server
{
	public:
		Server(const Config& config);
		~Server();

		void	start();
		void	stop();

		const std::string& getServerName() const;
		const std::string& getPassword() const;
		const std::map<int, Client>& get_clients() const;

		bool		isNickInUse(const std::string& nickname);	// NICK
		void		updateNickname(Client& client, const std::string& oldNick, const std::string& newNick);	// NICK

		// Messages
		void		sendError(int clientFd, const std::string& errorCode, const std::string& message);
		void		sendTargetError(int clientFd, const std::string& errorCode, const std::string& target, const std::string& message);

		// Client Management
    	Client* getClient(int fd);
    	Client* getClientByNickname(const std::string& nickname);
    	void 		addNickname(const std::string& nickname, int fd);
    	void 		removeNickname(const std::string& nickname);

    	// Channel Management
    	Channel* getChannel(const std::string& channelName);
    	void 		addChannel(Channel* channel);
    	void 		removeChannel(const std::string& channelName);

		void 		sendRawMessage(int clientFd, const std::string& message);


	private:
		void		createSocket();					// Create and bind server socket
		void		acceptNewClient();				// Accept new client connection
		void		processClientData(int clientFd);	// Receive data from a client
		void		removeClient(int clientFd);		// Remove client from the list

		int								serverSocket;			// Server socket file descriptor
		bool							isRunning;				// Flag to control server loop
		Config							config;
		std::vector<struct pollfd>		pollFds;				// Polling structures for clients
		std::map<int, Client>			clients;
		std::string						serverName;
		IrcCommands						ircCommands;
		std::map<std::string, Channel*> channels;

		std::map<std::string, int> nickToFd;	// NICK

};

#endif
