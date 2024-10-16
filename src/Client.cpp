#include "Client.hpp"

/*_____________________________________________________________________________
								ORTHODOX CANONICAL FORM
_____________________________________________________________________________*/

// Client::Client() : clientFd(-1) {}

/* Client::Client() : clientFd(-1) {}

Client::Client(int fd) : clientFd(fd) {} */

Client::Client() : clientFd(-1), authenticated(false), registered(false), nickSet(false){}

Client::Client(int fd) : clientFd(fd), authenticated(false), registered(false), nickSet(false){}

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
bool Client::hasNickSet() const { return nickSet; }



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
void Client::setNickSet(bool isNickSet) { nickSet = isNickSet;}


void Client::sendRawMessage(const std::string& message) {
    if (clientFd != -1) {
        ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
        if (bytesSent == -1) {
            // Log or handle the error appropriately
            perror("Failed to send message to client");
        }
    }
}

void Client::sendToClient(const std::string& message) const {
    send(clientFd, message.c_str(), message.length(), 0);
}



// void Channel::sendNamesList(Client* client, Server& server) {
//     std::string names;
//     for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
//         Client* channelClient = it->second;
//         if (isOperator(channelClient)) {
//             names += "@";
//         }
//         names += channelClient->getNickname() + " ";
//     }

//     std::string prefix = ":" + server.getHostname() + " ";
//     std::string channelSymbol = "="; // For public channels
//     client->sendMessage(prefix + "353 " + client->getNickname() + " " + channelSymbol + " " + _name + " :" + names + "\r\n"); // RPL_NAMREPLY
//     client->sendMessage(prefix + "366 " + client->getNickname() + " " + _name + " :End of /NAMES list\r\n"); // RPL_ENDOFNAMES
// }