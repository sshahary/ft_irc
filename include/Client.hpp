/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:53:44 by snagulap          #+#    #+#             */
/*   Updated: 2024/10/12 18:17:04 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		const std::string&	getNickname() const;
		// const std::string	getIpAddress() const;
		const std::string& getUsername() const;
		const std::string& getRealName() const;
		const std::string& getHostname() const;
		bool isAuthenticated() const;
		bool isRegistered() const;
		bool hasNickSet() const;

		void setNickname(const std::string& nickname);
		void setUsername(const std::string& username);
		void setRealName(const std::string& realName);
		void setHostname(const std::string& hostname);
		void setAuthenticated(bool authenticated);
		void setRegistered(bool registered);
		void setNickSet(bool nickSet);

		void sendRawMessage(const std::string& message);
		// void		setFd(int fd);
		// void		setIpAddress(const std::string &ipAddress);

	private:

		int				clientFd;				// Client file descriptor
		// std::string		ipAddress;				// Client IP address
		std::string		nickname;
		std::string		username;
		std::string		realName;
		std::string		hostname;
		bool authenticated;
		bool registered;
		bool nickSet;
};

#endif


/* 
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
	Client();
	Client(int fd);

	int getFd() const;
	const std::string& getNickname() const;
	const std::string& getUsername() const;
	const std::string& getRealName() const;
	const std::string& getHostname() const;
	bool isAuthenticated() const;
	bool isRegistered() const;

	void setNickname(const std::string& nickname);
	void setUsername(const std::string& username);
	void setRealName(const std::string& realName);
	void setHostname(const std::string& hostname);
	void setAuthenticated(bool authenticated);
	void setRegistered(bool registered);

private:
	int fd;
	std::string nickname;
	std::string username;
	std::string realName;
	std::string hostname;
	bool authenticated;
	bool registered;
};

#endif
 */
