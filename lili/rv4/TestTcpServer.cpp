

#include "TcpServer.h"
#include "ThreadPool.h"

#include <iostream>
#include <unistd.h>
// #include <string>
// using std::string;

ThreadPool *gPool;

void proccess(string msg)
{
	std::cout << "proc client msg:" << msg << std::endl;
}

void onMessage(TcpConnectionPtr con)
{

	string msg = con->receive();
	std::cout << "recv msg from client: " << msg << std::endl;
	gPool->addTask(std::bind(proccess, msg));

	con->send("sever received msg\n");
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