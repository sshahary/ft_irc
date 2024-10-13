#ifndef IRCCOMMANDS_HPP
#define IRCCOMMANDS_HPP

#include <string>
#include <vector>

class Server;
class Client;

enum CommandType
{
	CMD_PING,
	CMD_PASS,
	CMD_NICK,
	CMD_USER,
	CMD_JOIN,
	CMD_PRIVMSG,
	CMD_UNKNOWN
};

class IrcCommands
{
	public:
		IrcCommands(Server& server);
		~IrcCommands();

		void ircCommandsDispatcher(Client& client, const std::string& message);
		void sendToClient(const Client& client, const std::string& message);

	private:
		Server& server;
		std::vector<std::string> splitMessage(const std::string& message);
		int getCommandType(const std::string& command);
		void broadcastToAll(const std::string& message); // NICK
		void welcomeClient(Client& client); // USER

		void handlePing(Client& client, const std::vector<std::string>& params);
		void handlePass(Client& client, const std::vector<std::string>& params);
		void handleNick(Client& client, const std::vector<std::string>& params);
		void handleUser(Client& client, const std::vector<std::string>& params);
		void handlePrivmsg(Client& client, const std::vector<std::string>& params);
};

#endif
