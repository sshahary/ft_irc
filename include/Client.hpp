
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

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

#endif
