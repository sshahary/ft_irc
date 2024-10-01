/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:33:12 by musenov           #+#    #+#             */
/*   Updated: 2024/10/01 18:27:55 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcException.hpp"

IrcException::IrcException(const std::string &e_message)
	: exception_message(e_message)
{
}

IrcException::~IrcException() throw()
{
}

const char *IrcException::what() const throw()
{
	return exception_message.c_str();
}
