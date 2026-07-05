#include "head.h"

int localComSend(localMsg localmsg)
{
	struct msghdr msgh;
	bzero(&msgh, sizeof(msgh));

	char exitFlagVal[1] = {localmsg.exitFlag_ ? 1 : 0};
	struct iovec iovs[1];
	iovs[0].iov_base = exitFlagVal;
	iovs[0].iov_len  = 1;

	msgh.msg_iov    = iovs;
	msgh.msg_iovlen = 1;

	if(!localmsg.exitFlag_) {
		char ctl[CMSG_SPACE(sizeof(int))];
		bzero(ctl, sizeof(ctl));
		struct cmsghdr *cms = (struct cmsghdr *)ctl;
		cms->cmsg_len       = CMSG_LEN(sizeof(int));
		cms->cmsg_level     = SOL_SOCKET;
		cms->cmsg_type      = SCM_RIGHTS;
		*(int *)CMSG_DATA(cms) = localmsg.netFd_;

		msgh.msg_control    = cms;
		msgh.msg_controllen = CMSG_SPACE(sizeof(int));
	}

	sendmsg(localmsg.localSocketFd_, &msgh, 0);
	return 0;
}

int localComRecv(localMsg *localmsg)
{
	struct msghdr msgh;
	bzero(&msgh, sizeof(msgh));

	char buf[60] = {0};
	struct iovec iovs[1];
	iovs[0].iov_base = buf;
	iovs[0].iov_len  = sizeof(buf);

	msgh.msg_iov    = iovs;
	msgh.msg_iovlen = 1;

	char ctl[CMSG_SPACE(sizeof(int))];
	bzero(ctl, sizeof(ctl));
	msgh.msg_control    = ctl;
	msgh.msg_controllen = sizeof(ctl);

	int ret = recvmsg(localmsg->localSocketFd_, &msgh, 0);
	if(ret == 0) {
		printf("recv error 0, exit(-1);\n");
		exit(-1);
	}

	localmsg->exitFlag_ = (buf[0] != 0);

	if(!localmsg->exitFlag_) {
		struct cmsghdr *cms = CMSG_FIRSTHDR(&msgh);
		if(cms && cms->cmsg_level == SOL_SOCKET && cms->cmsg_type == SCM_RIGHTS) {
			localmsg->netFd_ = *(int *)CMSG_DATA(cms);
		}
	}
	return 0;
}