#include "head.h"

int sonProcRun(int localSocketFd)
{
	while(1) {
		printf("son proc work...\n");
		int netFd;
		localComRecv(&netFd, localSocketFd);

		//消息处理逻辑
		// sonProcDoTask(netFd);

		printf("son proc %d do task: net_fd %d\n", getpid(), netFd);
		send(netFd, "hello", 5, 0);

		close(netFd);
		send(localSocketFd, "1", 1, 0);
	}
	return 0;
}

int sonProcDoTask(int netFd)
{
	printf("son proc %d do task: net_fd %d\n", getpid(), netFd);
	send(netFd, "hello", 5, 0);
	return 0;
}