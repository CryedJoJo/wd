/**
 * Project Untitled
 */

#include "TcpConnection.h"
#include <string>
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
	int  ret = sockIO_.readLine(buf, sizeof(buf));
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