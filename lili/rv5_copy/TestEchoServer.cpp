

#include "EchoServer.h"
#include <iostream>
#include <unistd.h>
#include <functional>

string testProc(string msg)
{
	string procedMsg = msg;
	return procedMsg;
}

int main()
{
	EchoServer server("127.0.0.1", 8888);
	server.setProc(std::move(testProc));
	server.run();

	return 0;
}