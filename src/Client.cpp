

#include "Client.hpp"

// ============ Client - orthodox canonical form ==============================

Client::Client() : clientFd(-1) {}

// Client class implementation ================================================

int Client::getFd() const { return clientFd; }

void Client::setFd(int fd) { clientFd = fd; }

std::string Client::getIpAddress() const { return ipAddress; }

void Client::setIpAddress(const std::string &ip) { ipAddress = ip; }
