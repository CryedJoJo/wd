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

			close(localSocketFd[1]);
			//子进程工作函数
			sonProcRun(localSocketFd[0]);

		} else {
			procList[i].pid    = procId;
			procList[i].status = FREE;
			close(localSocketFd[0]);
			procList[i].localSocketFd = localSocketFd[1];
		}
	}
}

int toSonProc(int netFd, procStatus *procList, int num)
{
	for(int i = 0; i < num; ++i) {
		if(procList[i].status == FREE) {
			localComSend(netFd, procList[i].localSocketFd);
			procList[i].status = BUSY;
			break;
		}
	}
	return 0;
}
