

#include "TcpServer.h"

#include <iostream>
#include <unistd.h>
// #include <string>
// using std::string;

void onMessage(TcpConnectionPtr con)
{

	string msg = con->receive();
	std::cout << "recv msg from client: " << msg << std::endl;
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
	server.start();
}

int main()
{

	test();

	return 0;
}