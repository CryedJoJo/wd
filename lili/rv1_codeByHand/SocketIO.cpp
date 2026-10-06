/**
 * Project Untitled
 */

#include "SocketIO.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

SocketIO::SocketIO(int fd)
    : fd_(fd)
{
}

SocketIO::~SocketIO()
{
}

int SocketIO::readn(char *buf, int len)
{
	int   left = len;
	char *pstr = buf;
	int   ret  = 0;

	while(left > 0) {
		ret = read(fd_, pstr, left);
		if(-1 == ret && errno == EINTR) {
			continue;
		} else if(-1 == ret) {
			perror("read error -1");
			return -1;
		} else if(0 == ret) {
			break;
		} else {
			pstr += ret;
			left -= ret;
		}
	}

	return len - left;
}

int SocketIO::readLine(char *buf, int len)
{
	int   left = len - 1;
	char *pstr = buf;
	int   ret = 0, total = 0;

	while(left > 0) {
		//MSG_PEEK 只拷贝看一下，不取走缓冲区数据
		ret = recv(fd_, pstr, left, MSG_PEEK);
		if(-1 == ret && errno == EINTR) {
			continue;
		} else if(-1 == ret) {
			printf("fd_ is %d\n", fd_);
			perror("readLine error -1");
			return -1;
		} else if(0 == ret) {
			break;
		} else {

			for(int idx = 0; idx < ret; ++idx) {

				if(pstr[idx] == '\n') {
					int sz = idx + 1;
					readn(pstr, sz);
					pstr += sz;
					*pstr = '\0';
					return total + sz;
				}
			}

			readn(pstr, ret);
			total += ret;
			pstr += ret;
			left -= ret;
		}
	}

	*pstr = '\0';

	return total - left;
}

int SocketIO::writen(const char *buf, int len)
{
	int         left = len;
	const char *pstr = buf;
	int         ret  = 0;

	while(left > 0) {
		ret = write(fd_, pstr, left);
		if(-1 == ret && errno == EINTR) {
			continue;
		} else if(-1 == ret) {
			perror("write error -1");
			return -1;
		} else if(0 == ret) {
			break;
		} else {
			pstr += ret;
			left -= ret;
		}
	}

	return len - left;
}
