/**
 * Project Untitled
 */

#ifndef _SOCKETIO_H
#define _SOCKETIO_H

#include <string>

using std::string;

class SocketIO {
public:
	SocketIO();

	/**
 * @param fd
 */
	explicit SocketIO(int fd);

	~SocketIO();

	/**
 * @param buf
 */
	int readLine(char *buf, int len);

	/**
 * @param buf
 * @param n
 */
	int readn(char *buf, int n);

	/**
 * @param buf
 */
	int writen(const char *buf, int len);

private:
	int fd_;
};

#endif //_SOCKETIO_H