#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "TcpConnection.h"

using std::vector;
using std::map;
using std::shared_ptr;
class TcpConnection;

using func = std::function<void(TcpConnection &)>;

class Acceptor;//前向声明

class EventLoop
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    //循环与非循环
    void loop();
    void unloop();

    //封装epoll_wait的函数
    void waitEpollFd();

    //处理新的连接
    void handleNewConnection();

    //处理老的连接上的数据的收发
    void handleMessage(int fd);

    //封装epoll_create函数
    int createEpollFd();

    //将文件描述符放在红黑树上进行监听
    void addEpollReadFd(int fd);
    
    //将文件描述符从红黑树上取消监听
    void delEpollReadFd(int fd);

    //注册 TcpConnection的onMsgCb_ 回调函数
    void registCb(TcpConnectionPtr &con){
		con->setOnMsgCb(std::move(onMsgcb_));
	}

	void initCb(func &&onMsgCb){
		onMsgcb_ = std::move(onMsgCb);
	}

private:
    int _epfd;//是epoll_create创建出来的文件描述符
    vector<struct epoll_event> _evtList;//存放满足条件的文件描述符的数据结构
    bool _isLooping;//标识循环运行与否
    Acceptor &_acceptor;//连接器Acceptor的引用
    map<int, TcpConnectionPtr> _conns;//存放文件描述符与TcpConnection的键值对

	func onMsgcb_;
};

#endif
