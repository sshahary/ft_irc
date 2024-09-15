#include "../../include/server.hpp"

bool Client::isAuthenticated() const { return authenticated; }

void Client::setAuthenticated(bool status) { authenticated = status; }
