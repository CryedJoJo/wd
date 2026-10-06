
#include "EchoServer.h"
#include <iostream>

void EchoServer::proccess(const TcpConnectionPtr &con, const string msg)
{
	std::cout << "client msg:" << msg << std::endl;
	std::cout << "processing client msg..." << std::endl;
	//9. processing_ 未设置时 调用空 std::function 抛 std::bad_function_call
	//修改前的代码：
	// const string processedMsg = "processed msg " + processing_(msg);
	//修改过后的代码：
	if(!processing_) {
		std::cerr << "processing_ is empty, echo raw msg" << std::endl;
		con->sendInLoop(msg);
		return;
	}
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
		//10. 消息丢失：任务队列满时消息被丢弃（设计取舍：不阻塞 eventloop 线程）
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
