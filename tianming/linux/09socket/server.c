#include <stdio.h> //snprintf
#include <sys/select.h>
#include <sys/socket.h>
#include <stdlib.h>     //stoi
#include <netinet/in.h> //socketaddr_in
#include <arpa/inet.h>  //inet_addr
#include <strings.h>    //bzero
#include <string.h>     //strlen
#include <stdbool.h>    //bool
#include <unistd.h>     //close

struct con {
	int  fd_;
	bool alive;
};

int main()
{
	//socket fd
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);

	//端口复用
	int opt = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//address and convert
	struct sockaddr_in socketAd;
	socketAd.sin_family      = AF_INET;
	socketAd.sin_port        = htons(atoi("8081"));
	socketAd.sin_addr.s_addr = inet_addr("192.168.43.132");

	//bind
	bind(socketFd, (struct sockaddr *)&socketAd, sizeof(socketAd));

	//listen
	listen(socketFd, 20);

	//set
	fd_set set, temp;

	//set init
	FD_ZERO(&set);
	FD_ZERO(&temp);

	//FD_SET
	FD_SET(socketFd, &set);

	//con
	struct con list[1024];
	int        pos = 0;
	bzero(list, sizeof(list));

	while(1) {
		printf("start 。。。\n");
		//copy set to temp
		memcpy(&temp, &set, sizeof(set));

		//select
		int ret = select(100, &temp, NULL, NULL, NULL);

		//accept
		if(FD_ISSET(socketFd, &temp)) {
			int conFd = accept(socketFd, NULL, NULL);

			send(conFd, &conFd, sizeof(int), 0);
			printf("new connetcion ...\n");
			for(int i = 0; i < 1024; ++i) {
				if(!list[i].alive) {

					list[i].fd_   = conFd;
					list[i].alive = true;
					if(i > pos) {
						pos = i;
					}
					printf("cur i == %d, pos == %d\n", i, pos);
					break;
				}
			}
			FD_SET(conFd, &set);
		}

		for(int i = 0; i <= pos; ++i) {
			if(list[i].alive && FD_ISSET(list[i].fd_, &temp)) {
				printf("new msg ... from:%d\n", list[i].fd_);
				char buf[60] = {0};
				int  ret     = recv(list[i].fd_, buf, sizeof(buf), MSG_WAITALL);
				if(ret == 0) {
					list[i].alive = false;
					close(list[i].fd_);
					FD_CLR(list[i].fd_, &set);
					continue;
				}

				for(int j = 0; j <= pos; ++j) {
					if(j != i && list[j].alive) {
						char msg[70] = {0};
						snprintf(msg, sizeof(msg), "client%d:%s", list[i].fd_, buf);
						int ret = send(list[j].fd_, msg, strlen(msg), 0);
					}
				}
			}
		}
	}

	//close
	close(socketFd);

	return 0;
}