
#include "EchoServer.h"
#include <iostream>

void EchoServer::proccess(const TcpConnectionPtr &con, const string msg)
{
	std::cout << "client msg:" << msg << std::endl;
	std::cout << "processing client msg..." << std::endl;
	const string processedMsg = "processed msg " + processing_(msg);

	con->sendInLoop(processedMsg);
}

EchoServer::EchoServer(const string &ip, int port)
    : threadPool_(4, 10)
    , server_(ip, port)
{
}

EchoServer::~EchoServer()
{
}

void EchoServer::run()
{
	using namespace std::placeholders;
	server_.setThreeCb(
	    std::bind(&EchoServer::newConnection, this, _1),
	    std::bind(&EchoServer::onMessage, this, _1),
	    std::bind(&EchoServer::onClose, this, _1));

	server_.start();
}

void EchoServer::stop(){
	threadPool_.exit();
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{

	const string msg = con->receive();
	std::cout << "recv msg from client: " << msg << std::endl;
	bool ret = threadPool_.addTask(std::bind(&EchoServer::proccess, this, con, msg));
	if(!ret) {
		perror("add task");
	}

	// con->send("sever received msg\n");
}

void EchoServer::newConnection(const TcpConnectionPtr &con)
{
	std::cout << con->toString() << " connection" << std::endl;
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
	auto it = con->getPeerAddress();

	std::cout << it.getIP() << ":" << it.getPort() << " closed" << std::endl;
}

void EchoServer::setProc(proc &&process)
{
	processing_ = std::move(process);
}
