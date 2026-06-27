#include <stdio.h>
#include <string.h>
#include <unistd.h> //read write close STDIN_FILENO
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <arpa/inet.h> //转换函数
#include <netinet/in.h> //socketaddr_in

int main(){

	// 192.168.43.132
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in socketaddr;
	socketaddr.sin_family    = AF_INET;
	socketaddr.sin_addr.s_addr = inet_addr("192.168.43.132");
	socketaddr.sin_port        = htons(atoi("8080"));
	
	//connect
	connect(socket_fd, (struct sockaddr *)&socketaddr, sizeof(socket_fd));

	//set
	fd_set set;

	FD_ZERO(&set);

	while(1){
        // fd set
		FD_SET(socket_fd, &set);
		FD_SET(STDIN_FILENO, &set);

		//select
		select(10, &set, NULL, NULL, NULL);

		//监听返回
        if(FD_ISSET(STDIN_FILENO, &set)){
			char buf[60];
			read(STDIN_FILENO, buf, sizeof(buf));
			send(socket_fd, buf, sizeof(buf), 0);
		}

		if(FD_ISSET(socket_fd, &set)){
			char buf[60];
			recv(socket_fd, buf, sizeof(buf), 0);
			printf("%s\n");
		}
	}

    //close
	close(socket_fd);
	return 0;
}