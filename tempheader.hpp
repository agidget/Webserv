//
// Created by Gaynelle Flores on 1/25/22.
//

#ifndef SERVER_TEMPHEADER_HPP
#define SERVER_TEMPHEADER_HPP
#include <sys/poll.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <vector>
#include <map>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "ClientSocket.hpp"
#include "ResponseCreator.hpp"

typedef struct s_pollSockets {
	int fdsNum;
	int validFdsNum;
	int *listeningSockets;
	struct pollfd *fds;
} t_pollSockets;

#endif //SERVER_TEMPHEADER_HPP
