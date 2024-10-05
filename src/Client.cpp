#include "Client.hpp"

/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

// Client::Client() : clientFd(-1) {}

Client::Client() : clientFd(-1) {}

Client::Client(int fd) : clientFd(fd) {}

Client::~Client() {}

/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/




/*_____________________________________________________________________________
								GETTERS
_____________________________________________________________________________*/



int Client::getFd() const { return clientFd; }

const std::string Client::getIpAddress() const { return ipAddress; }

const std::string& Client::getNickname() const { return nickname; }



/*_____________________________________________________________________________
								SETTERS
_____________________________________________________________________________*/



void Client::setFd(int fd) { clientFd = fd; }

void Client::setIpAddress(const std::string &ip) { ipAddress = ip; }



