//6. 头文件无 include guard，多次包含会导致类重复定义
//修改前的代码：无 #ifndef 保护
//修改过后的代码：
#ifndef _ECHOSERVER_H
#define _ECHOSERVER_H

//16. proc 使用 std::function 但依赖 ThreadPool.h 传递 <functional>，缺少直接包含
//修改前的代码：无 #include <functional>
//修改过后的代码：
#include <functional>
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
