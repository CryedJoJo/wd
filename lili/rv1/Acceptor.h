/**
 * Project Untitled
 */

#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "InetAddress.h"
#include "Socket.h"

#include <string>

using std::string;

class Acceptor {
public:
	/**
 * @param ip
 * @param port
 */
	Acceptor(const string &ip, unsigned short port);

	~Acceptor();

	int accept();
	void init();

private:
	InetAddress addr_;
	Socket      socket_;

	
	void reuseAddr();

	void reusePort();

	void bind();

	void listen();
};

#endif //_ACCEPTOR_H