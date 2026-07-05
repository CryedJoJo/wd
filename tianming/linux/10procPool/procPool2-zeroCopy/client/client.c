#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>  //close
#include <sys/ipc.h> // mmap

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/mman.h>
#include <fcntl.h>

int main()
{

	int socketFd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in ipv4Addr;
	ipv4Addr.sin_family      = AF_INET;
	ipv4Addr.sin_addr.s_addr = inet_addr("192.168.43.132");
	ipv4Addr.sin_port        = htons(atoi("9999"));

	connect(socketFd, (struct sockaddr *)&ipv4Addr, sizeof(ipv4Addr));

	int  nameSize     = 0;
	char fileName[60] = {0};

	//recv file name
	recv(socketFd, &nameSize, sizeof(int), MSG_WAITALL);
	recv(socketFd, fileName, nameSize, MSG_WAITALL);

	/* 	int key = ftok("/text", 1000); */

	/* 	int   shm_fd = shm_open("/test", O_CREAT | O_RDWR, 0666);
	 */
	// 这个是共享内存的代码，现在不是要共享内存。。。

	// recv file size
	int fileSize = 0;
	recv(socketFd, &fileSize, sizeof(int), MSG_WAITALL);
	printf("line:%d , fileName:%s, nameLen: %d, fileLen:%d \n", __LINE__, fileName, nameSize, fileSize);

	//recv file
	int fileFd = open(fileName, O_RDWR | O_CREAT, 0600);
	ftruncate(fileFd, fileSize);

	void *p = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd, 0);

	recv(socketFd, p, fileSize, MSG_WAITALL);

	munmap(p, fileSize);

	close(socketFd);
	close(fileFd);

	return 0;
}