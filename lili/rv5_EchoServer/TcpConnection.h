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
#include <atomic>

class TcpConnection;
class EventLoop;
using std::shared_ptr;
using std::string;
using std::atomic;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using func             = std::function<void(const TcpConnectionPtr &)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	/**
 * @param fd
 */
	explicit TcpConnection(int fd, EventLoop *eventLoopPtr);

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

	void handleNewConnectionCb();
	void handleOnMessageCb();
	void handleCloseCb();

	void sendInLoop(string msg);

	//2. pendingCb 捕获裸 this，存在悬空指针风险
	//修改前的代码：无关闭标记，连接关闭后 pending 回调仍会向已复用 fd 写数据
	//修改过后的代码：handClose 时置位，pending 回调执行 send 前检查
	//注意：closed_ 必须是 atomic —— markClosed 在 eventloop 线程写，
	//而 sendInLoop 的直发回退路径（loop 停止时）在 worker 线程读，存在跨线程访问
	void markClosed()
	{
		closed_.store(true);
	}

private:
	SocketIO    sockIO_;
	Socket      sock_;
	InetAddress peerAddr_;
	InetAddress localAddr_;

	func newConnectionCb_;
	func onMsgCb_;
	func closeCb_;

	EventLoop *eventLoopPtr_;

	atomic<bool> closed_{false};
};

#endif //_TCPCONNECTION_H