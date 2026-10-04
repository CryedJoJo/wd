/**
 * Project Untitled
 */

#include "TcpConnection.h"
#include <iostream>
#include <sstream>

/**
 * TcpConnection implementation
 */

/**
 * @param fd
 */
TcpConnection::TcpConnection(int fd)
    : sockIO_(fd)
    , sock_(fd)
    , peerAddr_(getPeerAddress())
    , localAddr_(getLocalAddress())
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
	char buf[65535] = {0};
	int  ret        = sockIO_.readLine(buf, sizeof(buf));
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
	struct sockaddr_in addr;
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
	struct sockaddr_in addr;
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

void TcpConnection::newConnectionCb()
{
	if(newConnectionCb_) {
		newConnectionCb_(shared_from_this());
	} else {
		perror("newConnectionCb_ is nullptr");
	}
}
void TcpConnection::onMessageCb()
{
	if(onMsgCb_) {
		onMsgCb_(shared_from_this());
	} else {
		perror("newConnectionCb_ is nullptr");
	}
}
void TcpConnection::closeCb()
{
	if(closeCb_) {
		closeCb_(shared_from_this());
	} else {
		perror("newConnectionCb_ is nullptr");
	}
}