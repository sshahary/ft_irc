# IRC (Internet Relay Chat) 

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Commands](#commands)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This project is part of the 42 curriculum and is focused on building a custom IRC (Internet Relay Chat) server. IRC is an application layer protocol that facilitates real-time chat, using a server-client model where users can communicate via channels and private messages.

The purpose of this project is to gain experience with networking, sockets, the IRC protocol, and handling concurrency in C++.

## Features

- **Multi-client support**: Multiple users can connect and chat simultaneously.
- **Channel management**: Users can create, join, and leave channels.
- **Private messages**: Users can send private messages to other users.
- **Basic IRC commands**: Support for core IRC commands like `JOIN`, `PART`, `PRIVMSG`, etc.
- **Error handling**: Graceful handling of errors and unexpected events.
- **Real-time communication**: Clients receive updates as soon as messages are sent.

## Installation

### Prerequisites

Before you begin, ensure you have the following installed on your system:

- **C++ Compiler**: GCC or Clang
- **Make**: Build tool
- **Git**: Version control

### Clone the Repository

```bash
git clone https://github.com/sshahary/ft_irc.git
cd ft_irc
```
### Build the Project
Use the `Makefile` provided to compile the project. Simply run:
```
make
```
This will generate the executable for the IRC server.
### Usage
To start the IRC server, run the following command after building the project:
```
./ircserv <port> <password>
```
- `<port>`: The port number where the server will listen for client connections.
- `<password>`: A password that clients will need to provide in order to connect to the server.
#### Example:
```
./ircserv 6667 my_secure_password
```
Once the server is running, clients can connect using an IRC client like KVRIC, WeeChat, irssi, or HexChat, providing the correct port and password.

### Connecting to the IRC Server
To connect using an IRC client, use a command like the following:
```
nc localhost 6667 my_secure_password
```
You can replace `localhost` with the server's IP address if connecting remotely.

### Commands
The IRC server supports several basic IRC commands, including:

- **/JOIN**  `<#channel>`: Join a specific channel.
- **/PART** `<#channel>`: Leave a specific channel.
- **/PRIVMSG** `<user/channel>`: Send a private message to a user or channel.
- **/NICK** `<nickname>`: Set or change your nickname.
- **/QUIT**: Disconnect from the IRC server.
- **/TOPIC** `<#channel> [new topic]`: View or set the topic of a channel.
- ### /KICK `<#channel> <user>`: Remove a user from a channel (admin only).
and rest figure out yourself :)


#### Remove recursively .DS_Store from current dir and its sub-dirs


```bash
find . -type f -name '.DS_Store' -exec rm -f {} +
```

#!/bin/bash

# Define the new directory where the files will be copied
DEST_DIR="./cpp_hpp_files"

# Create the new directory if it doesn't exist
if [ ! -d "$DEST_DIR" ]; then
  mkdir "$DEST_DIR"
fi

# Find all .cpp and .hpp files and copy them into the new directory
find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec cp {} "$DEST_DIR" \;

# Output the result
echo "All .cpp and .hpp files have been copied to $DEST_DIR"
