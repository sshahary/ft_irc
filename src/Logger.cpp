#include "Logger.hpp"
#include <iostream>
#include <sstream>

void Logger::info(const std::string &message)
{
	std::cout << "[INFO] " << message << std::endl;
}

void Logger::error(const std::string &message)
{
	std::cerr << "[ERROR] " << message << std::endl;
}

std::string Logger::intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
