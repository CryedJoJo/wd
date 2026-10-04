

#include "EventLoop.h"

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
	Acceptor accptor("127.0.0.1", 8888);
	accptor.init();
	EventLoop lp(accptor);

	lp.setThreeCb(
	    std::move(newConnection),
	    std::move(onMessage),
	    std::move(onClose));

	// lp.setThreeCb(
	//     newConnection,
	//     onMessage,
	//     close));

	// lp.setOnMessageCb(std::move(onMessage));

	lp.loop();
}

int main()
{

	test();

	return 0;
}