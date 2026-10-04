#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <unistd.h>
#include <functional>

using std::cout;
using std::endl;
using std::function;

void onMsg(TcpConnection &con)
{
	std::cout << "onMsg in" << __FILE__ << " line:" << __LINE__ << std::endl;
	string msg = con.receive();
	con.send(msg);
	sleep(2);
}

void test()
{
	Acceptor acceptor("127.0.0.1", 8888);
	acceptor.ready(); //此时处于监听状态
	                  //
	EventLoop loop(acceptor);
	loop.initCb(std::move(onMsg));

	loop.loop();
}

int main(int argc, char **argv)
{
	test();
	return 0;
}
