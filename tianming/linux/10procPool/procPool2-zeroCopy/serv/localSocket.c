#include "head.h"

int localComSend(int *netFd, int localSocketFd)
{
	struct msghdr msg;
	bzero(&msg, sizeof(msg));

	char        *str = "hello";
	struct iovec iovs[1];
	iovs[0].iov_base = str;
	iovs[0].iov_len  = strlen(str);

	msg.msg_iov    = iovs;
	msg.msg_iovlen = 1;

	struct cmsghdr *cms = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
	cms->cmsg_len       = CMSG_LEN(sizeof(int));
	cms->cmsg_level     = SOL_SOCKET;
	cms->cmsg_type      = SCM_RIGHTS;
	void *p             = CMSG_DATA(cms);
	int  *pfd           = (int *)p;
	*pfd                = netFd;

	msg.msg_control    = cms;
	msg.msg_controllen = CMSG_LEN(sizeof(int));

	// 发送数据
	int ret = sendmsg(localSocketFd, &msg, 0);
	return 0;
}

int localComRecv(int *netFd, int localSocketFd)
{
	struct msghdr msg;
	bzero(&msg, sizeof(msg));

	char         buf[60] = {0};
	struct iovec iovs[1];
	iovs[0].iov_base = buf;
	iovs[0].iov_len  = sizeof(buf);

	msg.msg_iov    = iovs;
	msg.msg_iovlen = 1;

	struct cmsghdr *cms = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
	cms->cmsg_len       = CMSG_LEN(sizeof(int));
	cms->cmsg_level     = SOL_SOCKET;
	cms->cmsg_type      = SCM_RIGHTS;

	msg.msg_control    = cms;
	msg.msg_controllen = CMSG_LEN(sizeof(int));

	int ret = recvmsg(localSocketFd, &msg, 0);
	if(ret == 0) {
		printf("recv error 0, exit(-1);\n");
		exit(-1);
	}

	void *p       = CMSG_DATA(cms);
	int  *p_netFd = (int *)p;
	*netFd        = *p_netFd;
	return 0;
}