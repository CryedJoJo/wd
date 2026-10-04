/**
 * Project Untitled
 */

#include "SocketIO.h"

#include <unistd.h> //close
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h> //recv

/**
 * SocketIO implementation
 */

SocketIO::SocketIO()
{
}

/**
 * @param fd
 */
SocketIO::SocketIO(int fd)
    : fd_(fd)
{
}

SocketIO::~SocketIO()
{
	// close(fd_); SocketIO不负责fd的close，fd的close由Socket类负责
}

/**
 * @param buf
 * @return int
 */
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

/**
 * @param buf
 * @param n
 * @return int
 */
int SocketIO::readn(char *buf, int len)
{
	//修改前的代码：
	// int   left = len - 1;
	//注释：这儿 -1 是错的，上课讲的代码是没有 -1 的，但是上传的代码里面有 -1。
	//加上 -1 会导致缓冲区一直有一个数据没被读出来，导致缓冲区一直可读，但是缓冲区中的这个数据
	//要么是换行 '\n' 或者 '\0' ，啥也不显示。结果就是服务端一直爆打空消息
	//修改过后的代码：
	int left = len;

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

/**
 * @param buf
 * @return void
 */
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