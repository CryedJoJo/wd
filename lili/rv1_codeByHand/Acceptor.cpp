/**
 * Project Untitled
 */

#include "Acceptor.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

Acceptor::Acceptor(const string &ip, int port)
    : socket_()
    , addr_(ip, port)
{
	init();
}

Acceptor::~Acceptor()
{
}

int Acceptor::accept()
{
	int fd = ::accept(socket_.getFd(), nullptr, nullptr);

	if(fd < 0) {
		perror("accept");
		return -1;
	}
	return fd;
}

void Acceptor::init()
{
	reuseAddr();
	reusePort();
	bind();
	listen();
}

void Acceptor::bind()
{
	int ret = ::bind(socket_.getFd(),
	                 (struct sockaddr*)addr_.getInetAddrPtr(),
	                 sizeof(struct sockaddr));
	if(ret < 0) {
		perror("bind");
	}
}

void Acceptor::listen()
{
	int ret = ::listen(socket_.getFd(), 10);
	if(ret < 0) {
		perror("listen");
		return;
	}
}

/**
 * @return void
 */
void Acceptor::reuseAddr()
{
	int on  = 1;
	int ret = setsockopt(socket_.getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(ret) {
		perror("setsockopt");
		return;
	}
}

/**
 * @return void
 */
void Acceptor::reusePort()
{
	int on  = 1;
	int ret = setsockopt(socket_.getFd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	if(-1 == ret) {
		perror("setsockopt");
		return;
	}
}