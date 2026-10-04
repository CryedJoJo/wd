//6. 头文件无 include guard，多次包含会导致类重复定义
//修改前的代码：无 #ifndef 保护
//修改过后的代码：
#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "Acceptor.h"
#include "EventLoop.h"

#include <string>

class TcpServer {

public:
	TcpServer(const string &ip, int port);
	~TcpServer();
	void start();
	void setThreeCb(func &&newConnection, func &&onMsg, func &&close);

private:
	Acceptor  acceptor_;
	EventLoop eventLoop_;
	// ThreadPool threadPool_;
};

#endif