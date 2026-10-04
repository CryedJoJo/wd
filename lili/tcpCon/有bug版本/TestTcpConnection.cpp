#include "TcpConnection.h"
#include "Acceptor.h"

#include <iostream>
#include <unistd.h>
#include <string>

using std::string;

void test()
{
	Acceptor acceptor("127.0.0.1", 8888);
	acceptor.init();
	TcpConnection con(acceptor.accept());

	//加调试,打印本端与对端的ip与端口号
	std::cout << con.toString() << " has connected" << std::endl;

	while(1) {
		std::string msg = con.receive();
		std::cout << ">>recv msg from client: " << msg << std::endl;
		con.send(msg);
		std::cout << msg.size() << std::endl;
		sleep(3);

	}
}

int main()
{

	test();

	return 0;
}