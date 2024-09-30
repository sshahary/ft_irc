/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 23:27:02 by musenov           #+#    #+#             */
/*   Updated: 2024/09/30 16:27:03 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
// #include <cstdlib>
#include <signal.h>
#include "Logger.hpp"

Server *g_server = NULL;

/* void signalHandler(int signum)
{
	if (g_server)
		g_server->stop();
	exit(signum);
} */

void signalHandler(int signum)
// extern "C" void signalHandler(int signum)
{
	if (g_server)
	{
		Logger::info("Signal received: " + Logger::intToString(signum) + ". Shutting down server gracefully.");
		g_server->stop();
	}
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

		g_server = NULL;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Server has shut down" << std::endl;
	return EXIT_SUCCESS;
}











// Server *g_server = NULL;

// extern "C" void signalHandler(int signum)
// {
// 	if (g_server)
// 	{
// 		Logger::info("Signal received: " + Logger::intToString(signum) + ". Shutting down server gracefully.");
// 		g_server->stop();
// 	}
// }

// int main(int argc, char *argv[])
// {
// 	signal(SIGINT, signalHandler);   // Handle CTRL + C
// 	signal(SIGQUIT, signalHandler);  /* Handle CTRL + \    */

// 	try
// 	{
// 		Config config;
// 		if (!config.parseArguments(argc, argv))
// 		{
// 			std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
// 			return EXIT_FAILURE;
// 		}

// 		Server server(config);
// 		g_server = &server;

// 		server.run();

// 		g_server = NULL;
// 	}
// 	catch (const std::exception &e)
// 	{
// 		Logger::error(e.what());
// 		return EXIT_FAILURE;
// 	}

// 	return EXIT_SUCCESS;
// }


