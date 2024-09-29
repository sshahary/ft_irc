/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshahary <sshahary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:52:36 by sshahary          #+#    #+#             */
/*   Updated: 2024/09/29 15:52:42 by sshahary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 100;

// Maximum lengths
const size_t MAX_NICKNAME_LENGTH = 9;
const size_t MAX_USERNAME_LENGTH = 10;
const size_t MAX_REALNAME_LENGTH = 50;
const size_t MAX_CHANNEL_NAME_LENGTH = 50;
const size_t MAX_MESSAGE_LENGTH = 512;

// Default server values
const int DEFAULT_PORT = 6667;
const std::string DEFAULT_SERVER_NAME = "ircserv";

// Numeric Reply Codes
const std::string RPL_WELCOME = "001";
const std::string RPL_YOURHOST = "002";
const std::string RPL_CREATED = "003";
const std::string RPL_MYINFO = "004";
const std::string RPL_BOUNCE = "005";
const std::string RPL_UMODEIS = "221";
const std::string RPL_LUSERCLIENT = "251";
const std::string RPL_LUSEROP = "252";
const std::string RPL_LUSERUNKNOWN = "253";
const std::string RPL_LUSERCHANNELS = "254";
const std::string RPL_LUSERME = "255";
const std::string RPL_AWAY = "301";
const std::string RPL_UNAWAY = "305";
const std::string RPL_NOWAWAY = "306";
const std::string RPL_WHOISUSER = "311";
const std::string RPL_WHOISSERVER = "312";
const std::string RPL_WHOISOPERATOR = "313";
const std::string RPL_WHOISIDLE = "317";
const std::string RPL_ENDOFWHOIS = "318";
const std::string RPL_WHOISCHANNELS = "319";
const std::string RPL_CHANNELMODEIS = "324";
const std::string RPL_NOTOPIC = "331";
const std::string RPL_TOPIC = "332";
const std::string RPL_INVITING = "341";
const std::string RPL_NAMREPLY = "353";
const std::string RPL_ENDOFNAMES = "366";
const std::string RPL_MOTD = "372";
const std::string RPL_MOTDSTART = "375";
const std::string RPL_ENDOFMOTD = "376";
const std::string RPL_YOUREOPER = "381";

// Error Reply Codes
const std::string ERR_NOSUCHNICK = "401";
const std::string ERR_NOSUCHSERVER = "402";
const std::string ERR_NOSUCHCHANNEL = "403";
const std::string ERR_CANNOTSENDTOCHAN = "404";
const std::string ERR_TOOMANYCHANNELS = "405";
const std::string ERR_WASNOSUCHNICK = "406";
const std::string ERR_NOORIGIN = "409";
const std::string ERR_NORECIPIENT = "411";
const std::string ERR_NOTEXTTOSEND = "412";
const std::string ERR_UNKNOWNCOMMAND = "421";
const std::string ERR_NOMOTD = "422";
const std::string ERR_NONICKNAMEGIVEN = "431";
const std::string ERR_ERRONEUSNICKNAME = "432";
const std::string ERR_NICKNAMEINUSE = "433";
const std::string ERR_USERNOTINCHANNEL = "441";
const std::string ERR_NOTONCHANNEL = "442";
const std::string ERR_USERONCHANNEL = "443";
const std::string ERR_NOTREGISTERED = "451";
const std::string ERR_NEEDMOREPARAMS = "461";
const std::string ERR_ALREADYREGISTERED = "462";
const std::string ERR_PASSWDMISMATCH = "464";
const std::string ERR_CHANNELISFULL = "471";
const std::string ERR_UNKNOWNMODE = "472";
const std::string ERR_INVITEONLYCHAN = "473";
const std::string ERR_BANNEDFROMCHAN = "474";
const std::string ERR_BADCHANNELKEY = "475";
const std::string ERR_NOPRIVILEGES = "481";
const std::string ERR_CHANOPRIVSNEEDED = "482";
const std::string ERR_CANTKILLSERVER = "483";
const std::string ERR_NOOPERHOST = "491";
const std::string ERR_UMODEUNKNOWNFLAG = "501";
const std::string ERR_USERSDONTMATCH = "502";

const std::string RPL_WHOREPLY = "352";
const std::string RPL_ENDOFWHO = "315";

// Other Constants
const std::string CRLF = "\r\n";

#endif // CONSTANTS_HPP
