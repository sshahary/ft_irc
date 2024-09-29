#include "Config.hpp"

Config::Config(int port, const std::string &password) : port(port), password(password) {}

Config::~Config() {}

int Config::getPort() const
{
	return port;
}

const std::string& Config::getPassword() const
{
	return password;
}
