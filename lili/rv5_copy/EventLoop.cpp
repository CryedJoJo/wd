/**
 * Project Untitled
 */
#include "EventLoop.h"
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h> //::read()
#include <stdio.h>

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

	// count_   = 0;
	eventfd_ = ::eventfd(0, 0);
	addEpollFd(eventfd_);
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
			int      curFd = events_[i].data.fd;
			uint32_t evts  = events_[i].events;
			if(curFd == acceptor_.getSockFd()) {
				//新链接
				handNewConnection();
			} else if(curFd == eventfd_) { //eventfd
				readEventFd();
				doPendingCb();
			} else if(cons_.find(curFd) != cons_.end()) {

				//deepseek:错误/挂断事件优先处理：对端重置或半关闭，直接走关闭流程
				if(evts & (EPOLLERR | EPOLLHUP)) {
					handClose(curFd);
				} else if(evts & EPOLLIN) {
					handMessage(curFd);
				}

			} else {
				char error[16];
				sprintf(error, "unknown fd %d", curFd);
				perror(error);
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
	int ret       = epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
	if(ret < 0) {
		perror("addEpollFd");
		return;
	}
}

/**
 * @return void
 */
void EventLoop::delEpollFd(int fd)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events  = EPOLLOUT;
	int ret       = epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event);
	if(ret < 0) {
		perror("delEpollFd");
		return;
	}
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
			handClose(fd);
		} else {
			it->second->handleOnMessageCb();
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
	if(conFd < 0) {
		perror("handNewConnection");
		return;
	}

	cons_[conFd] = TcpConnectionPtr(new TcpConnection(conFd, this));

	cons_[conFd]->setNewConnectionCb(newConnectionCb_);
	cons_[conFd]->setOnMessageCb(onMsgCb_);
	cons_[conFd]->setCloseCb(closeCb_);

	addEpollFd(conFd);

	cons_[conFd]->handleNewConnectionCb();
}

/**
 * @return void
 */
void EventLoop::handClose(int fd)
{
	auto it = cons_.find(fd);
	if(it != cons_.end()) { //ds说这儿要加it判空语句， 但是在调用handClose之前的代码都已经判断过了
		it->second->handleCloseCb();
		delEpollFd(fd);  //从epoll中移除
		cons_.erase(it); //从map链接中移除
	}
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

void EventLoop::writeEventFd()
{
	uint64_t one = 1;
	ssize_t  s   = ::write(eventfd_, &one, sizeof(uint64_t));
	if(s != sizeof(uint64_t)) {
		perror("write eventfd");
	}
}

void EventLoop::readEventFd()
{
	uint64_t one = 1;
	ssize_t  s   = ::read(eventfd_, &one, sizeof(uint64_t));
	if(s != sizeof(uint64_t)) {
		perror("read eventfd");
	}
}

void EventLoop::addPendingCb(pendingCb &&pdCb)
{
	lock_guard<mutex> ul(mutex_);
	pendings_.emplace_back(std::move(pdCb));
}

void EventLoop::doPendingCb()
{

	vector<pendingCb> tmp;
	{
		lock_guard<mutex> ul(mutex_);
		// ssize_t            s = ::read(eventfd_, &count_, sizeof(uint64_t)); eventfd 本身就是原子的，不需要加锁
		swap(tmp, pendings_);
	}

	for(auto &pdCb : tmp) {
		pdCb();
	}
}

bool EventLoop::isLooping()
{
	return isLooping_;
}