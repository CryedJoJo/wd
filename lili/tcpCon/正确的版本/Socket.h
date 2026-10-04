#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "NonCopyable.h"

/*

 没有 NonCopyable（正确版禁止拷贝），Socket 被拷贝后两个对象持同一 fd，析构/关闭互相冲突

*/
class Socket
: NonCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shutDownWrite();

private:
    int _fd;
};

#endif
