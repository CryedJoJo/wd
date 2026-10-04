/**
 * Project Untitled
 */

#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <sstream>

/**
 * TcpConnection implementation
 */

/**
 * @param fd
 */
TcpConnection::TcpConnection(int fd, EventLoop *eventLoopPtr)
    : sockIO_(fd)
    , sock_(fd)
    , peerAddr_(getPeerAddress())
    , localAddr_(getLocalAddress())
    , eventLoopPtr_(eventLoopPtr)
{
	std::cout << "fd is " << fd << std::endl;
}

TcpConnection::~TcpConnection()
{
}

/**
 * @return string
 */
string TcpConnection::receive()
{
	//14. 忽略 readLine 返回值，读出错与正常读到空数据无法区分
	//修改前的代码：
	/*
	char buf[65535] = {0};
	int  ret        = sockIO_.readLine(buf, sizeof(buf));
	return buf;
	*/
	//修改过后的代码：
	char buf[65535] = {0};
	int  ret        = sockIO_.readLine(buf, sizeof(buf));
	if(ret < 0) {
		perror("TcpConnection::receive readLine error");
		return string();
	}
	return buf;
}

/**
 * @return string
 */
string TcpConnection::toString()
{
	std::ostringstream oss;
	oss << localAddr_.getIP() << ":"
	    << localAddr_.getPort() << "---->"
	    << peerAddr_.getIP() << ":"
	    << peerAddr_.getPort();

	return oss.str();
}

bool TcpConnection::isClosed() //2026年10月1日 19:10:26 为了在 eventloop的handMessage中 获取到这个con是否已经断开，断开就将con从epoll中摘除，执行onCloseCb
{
	char buff[100] = {0};
	int  ret       = ::recv(sock_.getFd(), buff, sizeof(buff), MSG_PEEK);

	return (0 == ret);
}

/**
 * @param msg
 * @return void
 */
void TcpConnection::send(const string &msg)
{
	sockIO_.writen(msg.c_str(), msg.size());
}

/**
 * @return InetAddress
 */
InetAddress TcpConnection::getPeerAddress()
{
	//7. getpeername 失败时 addr 未初始化，toString()/onClose 打印乱码
	//修改前的代码：
	// struct sockaddr_in addr;
	//修改过后的代码：
	struct sockaddr_in addr{};
	socklen_t          len = sizeof(struct sockaddr);
	//获取对端地址的函数getpeername
	int ret = getpeername(sock_.getFd(), (struct sockaddr *)&addr, &len);
	if(-1 == ret) {
		perror("getpeername");
	}

	return InetAddress(addr);
}

/**
 * @return InetAddress
 */
InetAddress TcpConnection::getLocalAddress()
{
	//7. getsockname 失败时 addr 未初始化，toString()/onClose 打印乱码
	//修改前的代码：
	// struct sockaddr_in addr;
	//修改过后的代码：
	struct sockaddr_in addr{};
	socklen_t          len = sizeof(struct sockaddr);
	//获取本端地址的函数getsockname
	int ret = getsockname(sock_.getFd(), (struct sockaddr *)&addr, &len);
	if(-1 == ret) {
		perror("getsockname");
	}

	return InetAddress(addr);
}

void TcpConnection::setNewConnectionCb(func &cb)
{
	newConnectionCb_ = cb;
}
void TcpConnection::setOnMessageCb(func &cb)
{
	onMsgCb_ = cb;
}
void TcpConnection::setCloseCb(func &cb)
{
	closeCb_ = cb;
}

void TcpConnection::handleNewConnectionCb()
{
	if(newConnectionCb_) {
		newConnectionCb_(shared_from_this());
	} else {
		perror("newConnectionCb_ is nullptr");
	}
}
void TcpConnection::handleOnMessageCb()
{
	if(onMsgCb_) {
		onMsgCb_(shared_from_this());
	} else {
		perror("newConnectionCb_ is nullptr");
	}
}
void TcpConnection::handleCloseCb()
{
	if(closeCb_) {
		closeCb_(shared_from_this());
	} else {
		perror("newConnectionCb_ is nullptr");
	}
}

void TcpConnection::sendInLoop(string msg)
{
	//2. pendingCb 捕获裸 this，存在悬空指针风险
	//修改前的代码：
	/*
	if(eventLoopPtr_->isLooping()) {
		eventLoopPtr_->addPendingCb(std::bind(&TcpConnection::send, this, msg));
		eventLoopPtr_->writeEventFd();
	}
	*/
	//问题：连接关闭后 cons_ 释放最后一个 shared_ptr，对象析构；doPendingCb 再执行 bind(this) 的回调 → UAF；
	//     且 fd 可能已被新连接复用，数据写错 socket。isLooping() 为 false 时消息被静默丢弃
	//修改过后的代码：pendingCb 只捕获 weak_ptr，执行时 lock 并检查关闭标记；loop 未运行时直接发送（无并发读写）
	std::weak_ptr<TcpConnection> weakSelf = weak_from_this();
	pendingCb pdCb = [weakSelf, msg = std::move(msg)]() {
		auto self = weakSelf.lock();
		if(self && !self->closed_.load()) {
			self->send(msg);
		}
	};

	if(eventLoopPtr_->isLooping()) {
		eventLoopPtr_->addPendingCb(std::move(pdCb));
		eventLoopPtr_->writeEventFd();
	} else {
		//10. 消息丢失：loop 未启动或已停止时，eventloop 线程不会读写该 fd，worker 线程直接发送是安全的，不再丢弃
		pdCb();
	}
}