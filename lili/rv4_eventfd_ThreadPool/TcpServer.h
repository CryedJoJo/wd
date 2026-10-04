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