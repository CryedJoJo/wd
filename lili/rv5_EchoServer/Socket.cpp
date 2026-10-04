/**
 * Project Untitled
 */

#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Socket implementation
 */

Socket::Socket()
{
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if(fd_ < 0) {
		perror("socket");
		return;
	}
}

/**
 * @param fd
 */
 Socket::Socket(int fd)
:fd_(fd)
{
}

Socket::~Socket()
{
	//修改前的代码：
	// ~Socket(){}  // 不 close(fd_)。strace 实测：创建 100 个 Socket → socket() 100 次、close() 0 次，服务器长期运行 fd 耗尽
	//修改过后的代码：
	close(fd_);
}

/**
 * @return int
 */
int Socket::getFd()
{
	return fd_;
}