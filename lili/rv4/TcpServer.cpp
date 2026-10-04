#include "TcpServer.h"

TcpServer::TcpServer(const string &ip, int port)
    : acceptor_(ip, port)
    , eventLoop_(acceptor_)
{
}
TcpServer::~TcpServer()
{
	eventLoop_.unLoop();
}
void TcpServer::start()
{
	acceptor_.init();
	eventLoop_.loop();
}

void TcpServer::setThreeCb(func &&newConnection, func &&onMsg, func &&close)
{
	eventLoop_.setNewConnectionCb(std::move(newConnection));
	eventLoop_.setOnMessageCb(std::move(onMsg));
	eventLoop_.setCloseCb(std::move(close));
}