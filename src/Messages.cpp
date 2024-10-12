/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 11:52:39 by sshahary          #+#    #+#             */
/*   Updated: 2024/10/12 12:12:40 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendError(int clientFd, const std::string& errorCode, const std::string& message)
{
	std::string errorMessage = ":" + errorCode + " " + message + "\r\n";
	send(clientFd, errorMessage.c_str(), errorMessage.length(), 0);
}
