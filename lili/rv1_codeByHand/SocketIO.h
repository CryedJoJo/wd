/**
 * Project Untitled
 */

#ifndef _SOCKETIO_H
#define _SOCKETIO_H

class SocketIO {

public:
	explicit SocketIO(int fd);
	~SocketIO();

	int readLine(char *buf, int size);
	int writen(const char *buf, int n);

private:
	int readn(char *buf, int n);

	/* data */
	int fd_;
};

#endif //_SOCKETIO_H