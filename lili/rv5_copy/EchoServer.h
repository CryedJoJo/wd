#ifndef _ECHOSERVER_H
#define _ECHOSERVER_H

#include "ThreadPool.h"
#include "TcpServer.h"

using proc = std::function<std::string(const string)>;

class EchoServer {
public:
	EchoServer(const string &ip, int port);
	~EchoServer();
	void run();
	void stop();
	void setProc(proc &&process);

private:
	void onMessage(const TcpConnectionPtr &con);
	void newConnection(const TcpConnectionPtr &con);
	void onClose(const TcpConnectionPtr &con);
	void proccess(const TcpConnectionPtr &con, string msg);

	/* data */
	ThreadPool threadPool_;
	TcpServer  server_;
	proc       processing_;
};

#endif //_ECHOSERVER_H
