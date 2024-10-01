/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 23:27:02 by musenov           #+#    #+#             */
/*   Updated: 2024/10/01 22:52:27 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
#include <signal.h>
#include "Logger.hpp"

Server *g_server = NULL;

void signalHandler(int signum)
{
	if (g_server)
	{
		Logger::info("Signal received: " + Logger::intToString(signum));
		g_server->stop();
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		Logger::error("Usage: ./ircserv <port> <password>");
		return EXIT_FAILURE;
	}
	try
	{
		Config config(std::atoi(argv[1]), argv[2]);
		Server server(config);
		g_server = &server;

		signal(SIGINT, signalHandler);
		signal(SIGQUIT, signalHandler);

		server.start();

		g_server = NULL;
	}
	// to catch not only IrcExceptions but also all possible ones
	catch (const std::exception &e)
	{
		Logger::error(e.what());
		return EXIT_FAILURE;
	}
	Logger::info("Server has shut down");
	return EXIT_SUCCESS;
}

/* 
try
	{
		Bureaucrat a("Cipolino", 0); // This should throw GradeTooHighException
		std::cout << a << std::endl;
	}
	catch (const Bureaucrat::GradeTooHighException &e_high)
	{
		std::cout << e_high.what() << std::endl;
	}
	catch (const Bureaucrat::GradeTooLowException &e_low)
	{
		std::cout << e_low.what() << std::endl;
	}


void	Bureaucrat::increment_grade()
{
	if (grade_ - 1 <= 0)
		throw Bureaucrat::GradeTooHighException();
	grade_--;
}

		class GradeTooHighException : public std::exception
		{
			public:
				const char* what() const throw()
				{
					return "Grade too high";
				}
		};

		
 */

