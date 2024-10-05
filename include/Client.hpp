
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

// Client class to handle individual client connections
class Client
{
	public:

		Client();
		Client(int fd);
		~Client();
		
		int					getFd() const;
		const std::string	getIpAddress() const;
		const std::string&	getNickname() const;

		void		setFd(int fd);
		void		setIpAddress(const std::string &ipAddress);

	private:

		int				clientFd;				// Client file descriptor
		std::string		ipAddress;				// Client IP address
		std::string		nickname;
		std::string		username;
		std::string		realName;
		std::string		hostname;
};

#endif
