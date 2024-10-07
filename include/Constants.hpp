#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

const std::string CRLF = "\r\n";

// IrcCommands::welcomeClient()
const std::string RPL_WELCOME = "001";
const std::string RPL_YOURHOST = "002";
const std::string RPL_CREATED = "003";
const std::string RPL_MYINFO = "004";

// PING
const std::string ERR_NOORIGIN = "409";
const std::string ERR_UNKNOWNCOMMAND = "421";

// PASS, USER
const std::string ERR_NEEDMOREPARAMS = "461";
const std::string ERR_ALREADYREGISTERED = "462";
const std::string ERR_PASSWDMISMATCH = "464";

// NICK
const std::string ERR_NONICKNAMEGIVEN = "431";
const size_t MAX_NICKNAME_LENGTH = 9;
const std::string ERR_ERRONEUSNICKNAME = "432";
const std::string ERR_NICKNAMEINUSE = "433";


#endif
