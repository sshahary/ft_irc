#include "Client.hpp"

/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

Client::Client() : clientFd(-1) {}

Client::~Client() {}

/*_____________________________________________________________________________
								CORE FUNCTIONS
_____________________________________________________________________________*/

int Client::getFd() const { return clientFd; }

void Client::setFd(int fd) { clientFd = fd; }

std::string Client::getIpAddress() const { return ipAddress; }

void Client::setIpAddress(const std::string &ip) { ipAddress = ip; }
