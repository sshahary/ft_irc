/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 11:52:39 by sshahary          #+#    #+#             */
/*   Updated: 2024/10/12 16:20:47 by snagulap         ###   ########.fr       */
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



