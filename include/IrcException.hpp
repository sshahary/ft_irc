/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:25:37 by musenov           #+#    #+#             */
/*   Updated: 2024/10/01 18:27:33 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEPTION_HPP
#define IRCEXCEPTION_HPP

#include <exception>
#include <string>

class IrcException : public std::exception
{
	public:
		IrcException(const std::string &e_message);
		virtual ~IrcException() throw();

		virtual const char *what() const throw();

	private:
		std::string exception_message;
};

#endif
