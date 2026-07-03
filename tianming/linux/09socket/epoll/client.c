#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h> //close
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h> //stoi

int main()
{
	//address
	struct sockaddr_in ipv4Addr;
	ipv4Addr.sin_family      = AF_INET;
	ipv4Addr.sin_port        = htons(atoi("8888"));
	ipv4Addr.sin_addr.s_addr = inet_addr("192.168.43.132");

	//socket
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);

	// //bind /* client 不能bind ！！！ 不然会一直socket就绪*/
	// bind(sockFd, (struct sockaddr *)&ipv4Addr, sizeof(ipv4Addr));

	//connet
	connect(sockFd, (struct sockaddr *)&ipv4Addr, sizeof(ipv4Addr));

	//epoll_create
	int epollFd = epoll_create(1);

	//epoll event bind socketFd
	struct epoll_event sockEvent;
	sockEvent.data.fd = sockFd;
	sockEvent.events  = EPOLLIN;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, sockFd, &sockEvent);

	//epoll event bind STDIN_FILENO
	struct epoll_event stdInEvent;
	stdInEvent.data.fd = STDIN_FILENO;
	stdInEvent.events  = EPOLLIN;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, STDIN_FILENO, &stdInEvent);

	bool closeFlag = false;

	while(1) {

		//epoll_wait
		sleep(1);
		struct epoll_event eventList[5];
		int                num = epoll_wait(epollFd, eventList, 5, -1);
		printf("epoll hit...\n");

		for(int i = 0; i < num; ++i) {
			printf("for in loop...\n");

			if(eventList[i].data.fd == STDIN_FILENO) {
				//send msg
				printf("send msg\n");
				char buf[60] = {0};
				read(STDIN_FILENO, buf, sizeof(buf));
				send(sockFd, buf, sizeof(buf), 0);
			} else if(eventList[i].data.fd == sockFd) {
				// receive msg
				printf("receive msg\n");
				char msg[70] = {0};
				int  ret     = recv(sockFd, msg, sizeof(msg), 0);
				if(ret == 0) {
					printf("server closed...\n");
					closeFlag = true;
					break;
				}
				printf("%s\n", msg);
			}
		}

		if(closeFlag) {
			break;
		}
	}

	close(sockFd);
	epoll_ctl(epollFd, EPOLL_CTL_DEL, sockFd, 0);
	epoll_ctl(epollFd, EPOLL_CTL_DEL, STDIN_FILENO, 0);
	return 0;
}