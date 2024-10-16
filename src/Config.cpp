
#include "Config.hpp"

/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

Config::Config(int port, const std::string &password) :	port(port), 
														password(password) {}

Config::~Config() {}

/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/


int Config::getPort() const
{
	return port;
}

const std::string& Config::getPassword() const
{
	return password;
}
