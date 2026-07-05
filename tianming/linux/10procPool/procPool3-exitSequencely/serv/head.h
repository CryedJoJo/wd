#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h> //close()
#include <sys/socket.h>
#include <stdlib.h>     //stoi()
#include <netinet/in.h> //socketaddr_in
#include <arpa/inet.h>  //inet_addr()
#include <strings.h>    //bzero()
#include <string.h>     //strlen()
#include <stdbool.h>    //bool

#include <sys/mman.h> //mmap()
#include <fcntl.h>

#include <sys/types.h> //O_ ...
#include <sys/stat.h>  // fstat()

#include <sys/sendfile.h> //sendfile()

#include <signal.h>
#include <sys/wait.h> //wait()

//维护子进程状态
enum {
	BUSY,
	FREE
};

typedef struct procStatus {
	pid_t pid_;
	int   status_;
	int   localSocketFd_;
} procStatus;

typedef struct localMsg {
	int  netFd_;
	int  localSocketFd_;
	bool exitFlag_;
} localMsg;

//初始化进程池
int initProcPool(procStatus *list, int procNum);
int toSonProc(int netFd, procStatus *procList, int num);

//子进程工作函数
int sonProcRun(int localSocketFd);
int sonProcDoTask(int netFd);

//本地套接字 用于父子进程传输网络条套接字数据
int localComRecv(localMsg *msg);
int localComSend(localMsg msg);

//初始化网络套接字
int initSocket(int *socketFd, char *ip, char *port);

//初始化epoll
int addEpoll(int epollFd, int fd);
int initEpoll(int *epollFd, procStatus *procList, int socketFd);

//发送文件
int sendFile(int netFd);
