#include "head.h"

int initProcPool(procStatus *procList, int procNum)
{

	//fork n process
	for(int i = 0; i < procNum; ++i) {

		//create local socket
		int localSocketFd[2];
		socketpair(AF_LOCAL, SOCK_STREAM, 0, localSocketFd);

		int procId = fork();
		if(procId == 0) {
			//子进程脱离前台进程组
			setpgid(0, 0);

			close(localSocketFd[1]);
			//子进程工作函数
			sonProcRun(localSocketFd[0]);

		} else {
			procList[i].pid_    = procId;
			procList[i].status_ = FREE;
			close(localSocketFd[0]);
			procList[i].localSocketFd_ = localSocketFd[1];
		}
	}

	return 0;
}

int toSonProc(int netFd, procStatus *procList, int num)
{
	for(int i = 0; i < num; ++i) {
		if(procList[i].status_ == FREE) {
			localMsg msg = {.netFd_         = netFd,
			                .localSocketFd_ = procList[i].localSocketFd_,
			                .exitFlag_      = false};
			localComSend(msg);
			procList[i].status_ = BUSY;
			break;
		}
	}
	return 0;
}