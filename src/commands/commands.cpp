/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:14:59 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/09 20:19:50 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"

void Server::handlePassCommand(int clientFd, const std::vector<std::string>& params)
{
	if (params.size() < 1)
	{
		std::cerr << RED << "No password provided" << WHI << std::endl;
		return ;
	}
	std::string providedPassword = params[0];
	if (providedPassword == serverPassword)
	{
		std::cout << GRE << "Client authenticated successfully" << WHI << std::endl;
		// Set client authenticated (not shown, but you would likely have a flag in Client)
	}
	else
	{
		std::cerr << RED << "Invalid password" << WHI << std::endl;
		removeClient(clientFd);
	}
}
