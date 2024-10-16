#include "Logger.hpp"
#include <iostream>
#include <sstream>
#include "Common.hpp"

void Logger::info(const std::string &message)
{
	std::cout << BLU << "[INFO] " << message << WHI << std::endl;
}

void Logger::error(const std::string &message)
{
	std::cerr << RED << "[ERROR] " << message << WHI << std::endl;
}

void Logger::chat(const std::string &message)
{
	std::cout << YEL << "[CHAT] " << message << WHI << std::endl;
}

void Logger::connection(const std::string &message)
{
	std::cout << GRE << "[CONNECTION] " << message << WHI << std::endl;
}

std::string Logger::intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
