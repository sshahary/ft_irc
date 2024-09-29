#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config
{
	public:
		Config(int port, const std::string &password);
		~Config();

		int					getPort() const;
		const std::string	&getPassword() const;

	private:
		int					port;
		std::string			password;
};

#endif
