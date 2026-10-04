/**
 * Project Untitled
 */

#include "Acceptor.h"

/**
 * Acceptor implementation
 */

/**
 * @param ip
 * @param port
 */
Acceptor::Acceptor(const string &ip, unsigned short port)
    : addr_(ip, port)
    , socket_()
{
}

Acceptor::~Acceptor()
{
}

/**
 * @return int
 */
int Acceptor::accept()
{
	int fd = ::accept(socket_.getFd(), nullptr, nullptr);
	if(-1 == fd) {
		perror("accept");
		return -1;
	}
	return fd;
}

/**
 * @return void
 */
void Acceptor::init()
{
	reuseAddr();
	reusePort();
	bind();
	listen();
}

int Acceptor::getSockFd()
{
	return socket_.getFd();
}

    /**
 * @return void
 */
    void Acceptor::reuseAddr()
{
	int on  = 1;
	int ret = setsockopt(socket_.getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(ret) {
		perror("setsockopt");
		return;
	}
}

/**
 * @return void
 */
void Acceptor::reusePort()
{
	int on  = 1;
	int ret = setsockopt(socket_.getFd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	if(-1 == ret) {
		perror("setsockopt");
		return;
	}
}

/**
 * @return void
 */
void Acceptor::bind()
{
	/*

	::bind(socket_.getFd(),
	       (struct sockaddr *)&addr_,
	       sizeof(struct sockaddr));
	
	addr_ 是 InetAddress 对象，这里把对象指针当成了 sockaddr_in 的地址。正确版本（Acceptor.cc:47-49）用的是 (struct sockaddr *)_addr.getInetAddrPtr()。之所以测试能跑通，纯属巧合：InetAddress 只有一个成员且无虚函数，对象首地址恰好 == 成员地址；一旦类加了虚函数/多成员/继承，bind 就会拿到野地址。且 bug 版完全不检查 bind 返回值。
	*/

	int ret = ::bind(socket_.getFd(),
	                 (struct sockaddr *)addr_.getInetAddrPtr(),
	                 sizeof(struct sockaddr));
	if(-1 == ret) {
		perror("bind");
		return;
	}
}

/**
 * @return void
 */
void Acceptor::listen()
{

	/*
	::listen(socket_.getFd(), 10);
	不检查返回值，backlog 10（正确版 128）。bind/listen 失败时程序毫无感知，随后 accept() 返回 -1，TcpConnection(-1) 中 getsockname/getpeername 失败后 addr 未初始化，toString() 输出乱码。
	*/

	int ret = ::listen(socket_.getFd(), 128);
	if(-1 == ret) {
		perror("listen");
		return;
	}
}