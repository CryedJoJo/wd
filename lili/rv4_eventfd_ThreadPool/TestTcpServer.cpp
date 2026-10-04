

#include "TcpServer.h"
#include "ThreadPool.h"

#include <iostream>
#include <unistd.h>
#include <functional>
// #include <string>
// using std::string;

ThreadPool *gPool;

using pendingCb = std::function<void()>;

// void realSend(TcpConnectionPtr con, string msg)
// {
// 	con->send(msg);
// }

void proccess(TcpConnectionPtr con, string msg)
{
	std::cout << "proc client msg:" << msg << std::endl;

	//msg sendInLoop
	std::cout << "processing..." << std::endl;

	string processedMsg = "processed msg " + msg;
	using std::placeholders::_1;
	con->sendInLoop(std::bind(&TcpConnection::send, con, processedMsg));
}

void onMessage(TcpConnectionPtr con)
{

	string msg = con->receive();
	std::cout << "recv msg from client: " << msg << std::endl;
	bool ret = gPool->addTask(std::bind(proccess, con, msg));
	if(!ret) {
		perror("add task");
	}

	// con->send("sever received msg\n");
}

void newConnection(TcpConnectionPtr con)
{
	std::cout << con->toString() << " connection" << std::endl;
}

void onClose(TcpConnectionPtr con)
{
	auto it = con->getPeerAddress();

	std::cout << it.getIP() << ":" << it.getPort() << " closed" << std::endl;
}

void test()
{

	TcpServer server("127.0.0.1", 8888);

	server.setThreeCb(
	    std::move(newConnection),
	    std::move(onMessage),
	    std::move(onClose));
	ThreadPool pool(4, 10);
	gPool = &pool;
	// gPool->exit();
	server.start();
}

int main()
{

	test();

	return 0;
}