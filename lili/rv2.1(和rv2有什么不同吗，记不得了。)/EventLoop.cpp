/**
 * Project Untitled
 */
#include "EventLoop.h"
#include <sys/epoll.h>
#include <iostream>
// using func = std::function<void(TcpConnectionPtr)>; //头文件有写 为什么这儿不写 下面会报错

/**
 * EventLoop implementation
 */

/**
 * @param acceptor
 */
EventLoop::EventLoop(Acceptor &acceptor)
    : events_()
    , cons_()
    , acceptor_(acceptor)
{
	events_.resize(100);
	initEpoll();
	int listenFd = acceptor_.getSockFd();
	addEpollFd(listenFd);
}

EventLoop::~EventLoop()
{
}

/**
 * @return void
 */
void EventLoop::loop()
{
	isLooping_ = true;

	while(isLooping_) {
		waitEpollFd();
	}
}

/**
 * @return void
 */
void EventLoop::unLoop()
{
	isLooping_ = false;
}

/**
 * @return void
 */
void EventLoop::initEpoll()
{
	epollFd_ = ::epoll_create(1);
}

/**
 * @return void
 */
void EventLoop::waitEpollFd()
{
	int ret;
	do {
		ret = epoll_wait(epollFd_, &(*events_.begin()), events_.size(), 3000);
	} while((-1 == ret && errno == EINTR));

	if(0 == ret) {
		std::cout << ">>>epoll wait time out..." << std::endl;
	} else if(-1 == ret) {
		perror("epoll wait error -1");
		return;
	} else {
		for(int i = 0; i < ret; ++i) {
			int curFd = events_[i].data.fd;
			if(curFd == acceptor_.getSockFd()) {
				//新链接
				handNewConnection();
			} else {

				handMessage(curFd);
			}
		}
	}
}

/**
 * @return void
 */
void EventLoop::addEpollFd(int fd)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events  = EPOLLIN;
	epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
}

/**
 * @return void
 */
void EventLoop::delEpollFd(int fd)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events  = EPOLLOUT;
	epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event);
}

/**
 * @return void
 */
void EventLoop::handMessage(int fd)
{
	auto it = cons_.find(fd);
	if(it != cons_.end()) {
		bool flag = it->second->isClosed();
		if(flag) {
			it->second->closeCb(); //提示断开 直接用了，不用 handClose
			delEpollFd(fd);        //从epoll中移除
			cons_.erase(it);       //从map链接中移除
		} else {
			it->second->onMessageCb();
		}
	} else {
		std::cout << "该连接是不存在" << std::endl;
		return;
	}
}

/**
 * @return void
 */
void EventLoop::handNewConnection()
{
	int conFd = acceptor_.accept();

	cons_[conFd] = TcpConnectionPtr(new TcpConnection(conFd));

	cons_[conFd]->setNewConnectionCb(newConnectionCb_);
	cons_[conFd]->setOnMessageCb(onMsgCb_);
	cons_[conFd]->setCloseCb(closeCb_);

	addEpollFd(conFd);

	cons_[conFd]->newConnectionCb();
}

/**
 * @return void
 */
void EventLoop::handClose(int fd)
{
	cons_[fd]->closeCb();
}

void EventLoop::setThreeCb(func &&newConnection, func &&onMsg, func &&close)
{
	setNewConnectionCb(std::move(newConnection));
	setOnMessageCb(std::move(onMsg));
	setCloseCb(std::move(close));
}

void EventLoop::setNewConnectionCb(func &&cb)
{
	newConnectionCb_ = std::move(cb);
}
void EventLoop::setOnMessageCb(func &&cb)
{
	onMsgCb_ = std::move(cb);
}
void EventLoop::setCloseCb(func &&cb)
{
	closeCb_ = std::move(cb);
}