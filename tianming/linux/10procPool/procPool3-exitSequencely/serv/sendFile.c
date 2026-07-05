#include "head.h"

int sendFile(int netFd)
{
	char *fileName = "1b.txt";

	int fileFd = open(fileName, O_RDWR);

	//send file name
	int nameLen = strlen(fileName);
	send(netFd, &nameLen, sizeof(int), MSG_NOSIGNAL);
	send(netFd, fileName, nameLen, MSG_NOSIGNAL);

	//获取文件信息
	struct stat st;
	memset(&st, 0, sizeof(st));
	fstat(fileFd, &st);

	//send file length
	int fileSize = (int)st.st_size;
	send(netFd, &fileSize, sizeof(int), MSG_NOSIGNAL);

	// mmap 方式
	// void *p = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd, 0);
	// send(netFd, p, st.st_size, MSG_NOSIGNAL);
	// munmap(p, st.st_size);

	//sendfile 方式
	sendfile(netFd, fileFd, 0, st.st_size);

	close(fileFd);

	return 0;
}