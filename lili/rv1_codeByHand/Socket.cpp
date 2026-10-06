/**
 * Project Untitled
 */

#include "Socket.h"

#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

Socket::Socket(/* args */)
{
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if(fd_ < 0){
		perror("socket");
		return;
	}
}

Socket::Socket(int fd)
    : fd_(fd)
{
}

Socket::~Socket()
{
	close(fd_);
}

int Socket::getFd()
{
	return fd_;
}
