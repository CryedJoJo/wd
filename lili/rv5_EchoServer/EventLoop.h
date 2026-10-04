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
#include <sys/eventfd.h> //eventfd
#include <mutex>
#include <atomic>

using std::vector;
using std::shared_ptr;
using std::map;
using func             = std::function<void(const TcpConnectionPtr &)>;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

using pendingCb = std::function<void()>;
using std::mutex;
using std::lock_guard;
using std::atomic;

class EventLoop {
public:
	/**
 * @param acceptor
 */
	EventLoop(Acceptor &acceptor);

	~EventLoop();

	void loop();

	void unLoop();

	void setNewConnectionCb(func &&cb);
	void setOnMessageCb(func &&cb);
	void setCloseCb(func &&cb);

	void writeEventFd();

	void addPendingCb(pendingCb &&pdCb);

	bool isLooping();

private:
	vector<struct epoll_event> events_;
	map<int, TcpConnectionPtr> cons_;
	Acceptor	              &acceptor_;
	int                        epollFd_;
	//3. isLooping_ 未初始化，loop() 执行前 isLooping() 读到随机值，属未定义行为
	//修改前的代码：
	// atomic<bool> isLooping_;
	//修改过后的代码：
	atomic<bool>               isLooping_{false};

	func newConnectionCb_;
	func onMsgCb_;
	func closeCb_;

	//eventfd 用于thread pool中线程与eventLoop 状态通信
	// uint64_t          count_; 累计唤醒的eventfd才有必要搞一个全局的count,这儿只是用到了eventfd的通知功能，不需要感知累计了多少次，使用可以把count_ 去掉
	int               eventfd_;
	vector<pendingCb> pendings_;
	mutex             mutex_;

	void initEpoll();

	void waitEpollFd();

	void addEpollFd(int fd);

	void delEpollFd(int fd);

	void handMessage(int fd);

	void handNewConnection();

	void handClose(int fd);

	void doPendingCb();

	void readEventFd();
};

#endif //_EVENTLOOP_H