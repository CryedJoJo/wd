/**
 * Project Untitled
 */

#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"

#include <string>

using std::string;

class TcpConnection {
public:
	/**
 * @param fd
 */
	explicit TcpConnection(int fd);

	~TcpConnection();

	string receive();

	string toString();

	/**
 * @param msg
 */
	void send(const string &msg);

	InetAddress getPeerAddress();

	InetAddress getLocalAddress();

private:

	SocketIO    sockIO_;
	Socket      sock_;
	InetAddress peerAddr_;
	InetAddress localAddr_;
};

#endif //_TCPCONNECTION_H