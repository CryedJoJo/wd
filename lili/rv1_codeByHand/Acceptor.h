/**
 * Project Untitled
 */

#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "Socket.h"
#include "InetAddress.h"

class Acceptor {

public:
	Acceptor(const string &ip, int port);
	~Acceptor();

	int accept();

private:
	void init();
	void bind();
	void listen();
	void reuseAddr();

	void reusePort();

	/* data */
	Socket      socket_;
	InetAddress addr_;
};

#endif //_ACCEPTOR_H