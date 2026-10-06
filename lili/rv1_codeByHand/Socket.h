/**
 * Project Untitled
 */

#ifndef _SOCKET_H
#define _SOCKET_H




class Socket {

public:
	Socket(/* args */);
	explicit Socket(int fd);
	~Socket();
	int getFd();

private:
	/* data */
	int fd_;
};

#endif //_SOCKET_H