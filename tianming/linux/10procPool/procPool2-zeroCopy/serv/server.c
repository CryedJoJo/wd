#include "head.h"

int main()
{

	//init pool
	procStatus procList[3];
	initProcPool(procList, 3);

	//init socket
	int socketFd;
	initSocket(&socketFd, "192.168.43.132", "9999");

	//init epoll
	int epollFd;
	initEpoll(&epollFd, procList, socketFd);

	while(1) {
		printf("server start...\n");
		struct epoll_event eventList[5];
		int                num = epoll_wait(epollFd, eventList, 5, -1);
		printf("num = %d\n", num);
		for(int i = 0; i < num; ++i) {
			if(eventList[i].data.fd == socketFd) {
				printf("%s :new connetion recv...\n", __FILE__);
				int netFd = accept(socketFd, NULL, NULL);
				toSonProc(netFd, procList, 3);
				close(netFd); 
				//父子进程都需要关闭netFd，不然就是同时持有同一个netFd
				//会导致无法四次挥手
			} else {

				char buf[2] = {0};
				recv(eventList[i].data.fd, buf, sizeof(buf), 0);

				printf("son proc task finish...\n");

				for(int j = 0; j < 3; ++j) {
					if(procList[j].localSocketFd == eventList[i].data.fd) {
						procList[j].status = FREE;
						break;
					}
				}
			}
		}
	}

	close(socketFd);
	return 0;
}