/**
 * Project Untitled
 */

#ifndef _SOCKET_H
#define _SOCKET_H
#include "NonCopyable.h"

class Socket :NonCopyable {
public:
	Socket();

	/**
 * @param fd
 */
	explicit Socket(int fd);

	~Socket();

	int getFd();

private:
	int fd_;
};

#endif //_SOCKET_H