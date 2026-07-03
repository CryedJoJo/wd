#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
	char *port = "8081";
	char *ip   = "192.168.43.132";

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family      = AF_INET;
	sockaddr.sin_port        = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	connect(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

	int  num  = 0;
	int *pNum = &num;
	recv(socket_fd, pNum, sizeof(int), 0);
	printf("------ l am client%d ------\n", num);

	fd_set set;
	FD_ZERO(&set);

	while(1) {
		// printf("me %d:", num);
		FD_SET(STDIN_FILENO, &set);
		FD_SET(socket_fd, &set);

		select(10, &set, NULL, NULL, NULL);

		if(FD_ISSET(STDIN_FILENO, &set)) {
			char buf[60] = {0};
			read(STDIN_FILENO, buf, sizeof(buf));
			send(socket_fd, buf, sizeof(buf), 0);
		}

		if(FD_ISSET(socket_fd, &set)) {
			char buf[70] = {0};
			int ret = recv(socket_fd, buf, sizeof(buf), 0);
			if(ret == 0) {
				printf("服务器断开链接 \n");
				break;
			}
			printf("%s \n", buf);
		}
	}

	close(socket_fd);
	return 0;
}
