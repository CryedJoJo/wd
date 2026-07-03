#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h> //close
#include <sys/socket.h>
#include <stdlib.h>     //stoi
#include <netinet/in.h> //socketaddr_in
#include <arpa/inet.h>  //inet_addr
#include <strings.h>    //bzero
#include <string.h>     //strlen
#include <stdbool.h>    //bool

struct con {
	int  fd_;
	bool alive;
};

int main()
{
	//address
	struct sockaddr_in ipv4Addr;
	ipv4Addr.sin_family      = AF_INET;
	ipv4Addr.sin_port        = htons(atoi("8888"));
	ipv4Addr.sin_addr.s_addr = inet_addr("192.168.43.132");

	//socket
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);

	//bind
	bind(sockFd, (struct sockaddr *)&ipv4Addr, sizeof(ipv4Addr));

	//listen
	listen(sockFd, 20);

	//epoll create
	int epollFd = epoll_create(1); // 1 no mine

	//epoll event
	struct epoll_event sockEvent;
	sockEvent.data.fd = sockFd;
	sockEvent.events  = EPOLLIN; //what's mine

	//epoll ctl
	epoll_ctl(epollFd, EPOLL_CTL_ADD, sockFd, &sockEvent);

	//con list
	struct con conList[1024];
	memset(conList, 0, sizeof(conList));
	int pos = 0;

	while(1) {
		sleep(1);
		printf("start...\n");

		//create input of epoll_wait
		struct epoll_event eventList[5];
		memset(eventList, 0, sizeof(eventList));

		//epoll wait
		int num = epoll_wait(epollFd, eventList, 5, -1); // what't -1 mine

		//reverse event
		for(int i = 0; i < num; ++i) {

			struct epoll_event curEvent = eventList[i];

			if(curEvent.data.fd == sockFd) {
				//new connect
				printf("new connect\n");
				int newConFd = accept(sockFd, NULL, NULL);
				for(int i = 0; i < 1024; ++i) {
					if(!conList[i].alive) {
						conList[i].alive = true;
						conList[i].fd_   = newConFd;
						if(i > pos) {
							pos = i;
						}
						break;
					}
				}

				struct epoll_event newConEvent;
				newConEvent.data.fd = newConFd;
				newConEvent.events  = EPOLLIN;
				epoll_ctl(epollFd, EPOLL_CTL_ADD, newConFd, &newConEvent);

			} else {
				// new msg
				printf("new msg\n");
				char buf[60] = {0};
				int  ret     = recv(eventList[i].data.fd, buf, sizeof(buf), MSG_WAITALL);
				if(ret == 0) {
					for(int j = 0; j <= pos; ++j) {
						if(eventList[i].data.fd == conList[j].fd_ && conList[j].alive) {
							epoll_ctl(epollFd, EPOLL_CTL_DEL, curEvent.data.fd, NULL);
							conList[j].alive = false;
							close(conList[j].fd_);
							break;
						}
					}
					continue;
				}

				// for() send
				for(int j = 0; j <= pos; ++j) {
					if(eventList[i].data.fd == conList[j].fd_ || !conList[j].alive) {
						continue;
					}
					char msg[70] = {0};
					snprintf(msg, sizeof(msg), "client%d:%s", conList[j].fd_, buf);
					int ret = send(conList[j].fd_, msg, strlen(msg), 0);
				}
			}
		}
	}

	// close(1);
	return 0;
}