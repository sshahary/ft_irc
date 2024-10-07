
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

		const std::string& getServerName() const;
		const std::string& getPassword() const;
		const std::map<int, Client>& get_clients() const;

		bool isNickInUse(const std::string& nickname);	// NICK
		void updateNickname(Client& client, const std::string& oldNick, const std::string& newNick);	// NICK

	private:
		void	createSocket();					// Create and bind server socket
		void	acceptNewClient();				// Accept new client connection
		void	processClientData(int clientFd);	// Receive data from a client
		void	removeClient(int clientFd);		// Remove client from the list

		int								serverSocket;			// Server socket file descriptor
		bool							isRunning;				// Flag to control server loop
		Config							config;
		std::vector<struct pollfd>		pollFds;				// Polling structures for clients
		std::map<int, Client>			clients;
		std::string						serverName;

		std::map<std::string, int> nickToFd;	// NICK

};

#endif
