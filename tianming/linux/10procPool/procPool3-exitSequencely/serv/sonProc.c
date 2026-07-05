#include "head.h"

int sonProcRun(int localSocketFd)
{
	while(1) {
		printf("son proc work...\n");
		localMsg msg;
		msg.localSocketFd_ = localSocketFd;
		localComRecv(&msg);

		if(msg.exitFlag_) {
			printf("son proc %d exit...\n", getpid());

			//子进程释放资源
			// sleep(1);
			//...
			//

			exit(0);
		}

		//消息处理逻辑
		// sonProcDoTask(msg.netFd_);

		printf("son proc %d do task: net_fd %d\n", getpid(), msg.netFd_);
		sendFile(msg.netFd_);

		close(msg.netFd_);
		send(msg.localSocketFd_, "1", 1, 0);
	}
	return 0;
}
