#include "head.h"

int addEpoll(int epollFd, int fd)
{

	struct epoll_event event;
	event.data.fd = fd;
	event.events  = EPOLLIN;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);

	return 0;
}

int initEpoll(int *epollFd, procStatus *procList, int socketFd)
{
	*epollFd = epoll_create(1);
	addEpoll(*epollFd, socketFd);

	for(int i = 0; i < 3; ++i) {
		addEpoll(*epollFd, procList[i].localSocketFd_);
	}

	return 0;
}