/**
 * Project Untitled
 */

#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"

#include <string>
#include <functional>
#include <memory>

class TcpConnection;
using std::shared_ptr;
using std::string;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using func             = std::function<void(TcpConnectionPtr)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	/**
 * @param fd
 */
	explicit TcpConnection(int fd);

	~TcpConnection();

	string receive();

	string toString();

	bool isClosed();

	/**
 * @param msg
 */
	void send(const string &msg);

	InetAddress getPeerAddress();

	InetAddress getLocalAddress();

	void setNewConnectionCb(func &cb);
	void setOnMessageCb(func &cb);
	void setCloseCb(func &cb);

	void newConnectionCb();
	void onMessageCb();
	void closeCb();

private:
	SocketIO    sockIO_;
	Socket      sock_;
	InetAddress peerAddr_;
	InetAddress localAddr_;

	func newConnectionCb_;
	func onMsgCb_;
	func closeCb_;
};

#endif //_TCPCONNECTION_H