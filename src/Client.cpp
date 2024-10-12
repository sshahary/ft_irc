#include "Client.hpp"

/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

// Client::Client() : clientFd(-1) {}

/* Client::Client() : clientFd(-1) {}

Client::Client(int fd) : clientFd(fd) {} */

Client::Client() : clientFd(-1), authenticated(false), registered(false) {}

Client::Client(int fd) : clientFd(fd), authenticated(false), registered(false) {}

Client::~Client() {}

/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/




/*_____________________________________________________________________________
								GETTERS
_____________________________________________________________________________*/



// int Client::getFd() const { return clientFd; }

// const std::string Client::getIpAddress() const { return ipAddress; }

const std::string& Client::getNickname() const { return nickname; }

int Client::getFd() const { return clientFd; }

const std::string& Client::getUsername() const { return username; }
const std::string& Client::getRealName() const { return realName; }
const std::string& Client::getHostname() const { return hostname; }
bool Client::isAuthenticated() const { return authenticated; }
bool Client::isRegistered() const { return registered; }



/*_____________________________________________________________________________
								SETTERS
_____________________________________________________________________________*/



// void Client::setFd(int fd) { clientFd = fd; }

// void Client::setIpAddress(const std::string &ip) { ipAddress = ip; }


void Client::setNickname(const std::string& newNickname) { nickname = newNickname; }
void Client::setUsername(const std::string& newUsername) { username = newUsername; }
void Client::setRealName(const std::string& newRealName) { realName = newRealName; }
void Client::setHostname(const std::string& newHostname) { hostname = newHostname; }
void Client::setAuthenticated(bool isAuthenticated) { authenticated = isAuthenticated; }
void Client::setRegistered(bool isRegistered) { registered = isRegistered; }
