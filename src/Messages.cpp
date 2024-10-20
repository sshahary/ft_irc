/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 11:52:39 by sshahary          #+#    #+#             */
/*   Updated: 2024/10/20 19:03:47 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendError(int clientFd, const std::string& errorCode, const std::string& message)
{
	std::string errorMessage = ":" + errorCode + " " + message + "\r\n";
	send(clientFd, errorMessage.c_str(), errorMessage.length(), 0);
}

void Server::sendTargetError(int clientFd, const std::string& errorCode, const std::string& target, const std::string& message)
{
	std::string errorMessage = ":" + serverName + " " + errorCode + " " + target + " :" + message + "\r\n";
	send(clientFd, errorMessage.c_str(), errorMessage.length(), 0);
}
