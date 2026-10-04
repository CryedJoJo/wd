/**
 * Project Untitled
 */

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include "TcpConnection.h"
#include "Acceptor.h"

#include <vector>
#include <memory>
#include <map>
#include <functional>

using std::vector;
using std::shared_ptr;
using std::map;
using func = std::function<void(TcpConnectionPtr)>;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class EventLoop {
public:
	/**
 * @param acceptor
 */
	EventLoop(Acceptor &acceptor);

	~EventLoop();

	void loop();

	void unLoop();

	void setThreeCb(func &&newConnection, func &&onMsg, func &&close);

	void setNewConnectionCb(func &&cb);
	void setOnMessageCb(func &&cb);
	void setCloseCb(func &&cb);

private:
	vector<struct epoll_event> events_;
	map<int, TcpConnectionPtr> cons_;
	Acceptor	              &acceptor_;
	int                        epollFd_;
	bool                       isLooping_;

	func newConnectionCb_;
	func onMsgCb_;
	func closeCb_;

	void initEpoll();

	void waitEpollFd();

	void addEpollFd(int fd);

	void delEpollFd(int fd);

	void handMessage(int fd);

	void handNewConnection();

	void handClose(int fd);
};

#endif //_EVENTLOOP_H