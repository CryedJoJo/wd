/**
 * Project Untitled
 */

#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "Socket.h"
#include "SocketIO.h"

#include "InetAddress.h"
class TcpConnection {

public:
	explicit TcpConnection(int fd);
	~TcpConnection();
	string receive();
	void   send(const string &msg);
	string toString();

private:
	InetAddress getPeerAddress();

	InetAddress getLocalAddress();

	/* data */
	Socket      socket_;
	SocketIO    socketIO_;
	InetAddress localAddr_;
	InetAddress peerAddr_;
};

#endif //_TCPCONNECTION_H