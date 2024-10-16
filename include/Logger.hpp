
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger
{
	public:
		static void info(const std::string &message);
		static void error(const std::string &message);
		static void chat(const std::string &message);
		static void connection(const std::string &message);

		static std::string intToString(int value);
};

#endif
