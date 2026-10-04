#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollFd())
, _evtList(1024)
, _isLooping(false)
, _acceptor(acceptor)
{
    //获取listenfd（也就是socket返回的文件描述符）
    //并且将listenfd放在红黑树上进行监听
    int listenfd = _acceptor.fd();
    addEpollReadFd(listenfd);
}

EventLoop::~EventLoop()
{
    close(_epfd);
}

//循环与非循环
void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

//封装epoll_wait的函数
void EventLoop::waitEpollFd()
{
    int nready = 0;
    do{
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    }while((-1 == nready && errno == EINTR));

    if(-1 == nready)
    {
        perror("-1 == nready");
        return;
    }
    else if(0 == nready)
    {
        cout << ">>epoll_wait timeout!!!" << endl;
    }
    else 
    {
        //考虑一个问题，vector有可能越界的问题，需要手动扩容
        //nready == -1(int--->size_t)
        //for(size_t idx = 99; idx > 0; idx -=2)
        //{
        //
        //}
        if((int)_evtList.size() == nready)
        {
            _evtList.resize(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            int listenfd = _acceptor.fd();
            if(fd == listenfd)
            {
                //有新的连接请求上来
                handleNewConnection();
            }
            else
            {
                //处理老的连接
                handleMessage(fd);
            }
        }
    }
}

//处理新的连接
void EventLoop::handleNewConnection()
{
    //调用Acceptor的accept函数，如果该函数的返回值
    //connfd是正常值，就表明三次握手建立成功了
    int connfd = _acceptor.accept();
    if(connfd < 0)
    {
        perror("handleNewConnection");
        return;
    }
    //监听connfd
    addEpollReadFd(connfd);

	//创建出TcpConnection的对象
	/* TcpConnection con(connfd); */
	TcpConnectionPtr con(new TcpConnection(connfd));
    registCb(con);
	//将connfd与TcpConnection的键值对存放在map中
    /* _conns.insert(std::make_pair(connfd, con)); */
    _conns[connfd] = con;
}

//处理老的连接上的数据的收发
void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if(it != _conns.end())
    {
        /*

        //通过连接接收客户端的数据
        string msg = it->second->receive();
        cout << ">>recv from client : " << msg << endl;
        //可以处理msg
        //处理完msg之后，可以将处理之后的msg
        //发给客户端
        it->second->send(msg);

        */
        
        
        //为了不改变源码，让三个半 事件的逻辑放到外部回调中，这儿执行外部回调代码 三个事件：链接、消息到达、链接关闭。半个事件：消息发送 

        //执行三个半中的 消息到达
		it->second->runOnMsgCb();
	} else {
		cout << "该连接是不存在" << endl;
        return;
	}
}

//封装epoll_create函数
int EventLoop::createEpollFd()
{
    int fd = epoll_create(10);
    if(fd < 0)
    {
        perror("createEpollFd");
        return -1;
    }

    return fd;
}

//将文件描述符放在红黑树上进行监听
void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret < 0)
    {
        perror("addEpollReadFd");
        return;
    }
}

//将文件描述符从红黑树上取消监听
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0)
    {
        perror("delEpollReadFd");
        return;
    }

}
