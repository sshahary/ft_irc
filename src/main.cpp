/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 23:27:02 by musenov           #+#    #+#             */
/*   Updated: 2024/09/29 19:37:10 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
// #include <cstdlib>
#include <signal.h>

Server* g_server = NULL;

void signalHandler(int signum)
{
	if (g_server)
		g_server->stop();
	exit(signum);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
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
	}
	catch (const std::exception &e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Server has shut down" << std::endl;
	return EXIT_SUCCESS;
}

