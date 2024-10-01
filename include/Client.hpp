
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

// Client class to handle individual client connections
class Client
{
	public:

		Client();
		~Client();
		
		int			getFd() const;
		std::string	getIpAddress() const;

		void		setFd(int fd);
		void		setIpAddress(const std::string &ipAddress);

	private:

		int				clientFd;				// Client file descriptor
		std::string		ipAddress;				// Client IP address
};

#endif
