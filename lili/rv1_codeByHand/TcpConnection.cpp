/**
 * Project Untitled
 */

#include "TcpConnection.h"

TcpConnection::TcpConnection(int fd)
    : socket_(fd)
    , socketIO_(fd)
    , peerAddr_(getPeerAddress())
    , localAddr_(getLocalAddress())
{
}

TcpConnection::~TcpConnection()
{
}

string TcpConnection::receive()
{
	char buf[65535] = {};
	int  ret        = socketIO_.readLine(buf, sizeof(buf));
	if(0 == ret) {
		perror("receive");
		return {};
	}
	return string(buf);
}

void TcpConnection::send(const string &msg)
{

	int ret = socketIO_.writen(msg.c_str(), msg.size());
	if(ret < 0) {
		perror("writen");
	}
}

string TcpConnection::toString()
{
}

/**
 * @return InetAddress
 */
InetAddress TcpConnection::getPeerAddress()
{
	struct sockaddr_in addr;
	socklen_t          len = sizeof(struct sockaddr);
	//获取对端地址的函数getpeername
	int ret = getpeername(socket_.getFd(), (struct sockaddr *)&addr, &len);
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
	int ret = getsockname(socket_.getFd(), (struct sockaddr *)&addr, &len);
	if(-1 == ret) {
		perror("getsockname");
	}

	return InetAddress(addr);
}