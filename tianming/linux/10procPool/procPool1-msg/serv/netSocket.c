#include "head.h"

int initSocket(int *socketFd, char *ip, char *port){
	*socketFd = socket(AF_INET, SOCK_STREAM, 0);
    
    //address
	struct sockaddr_in ipv4Addr;
	ipv4Addr.sin_family = AF_INET;
	ipv4Addr.sin_addr.s_addr = inet_addr(ip);
	ipv4Addr.sin_port        = htons(atoi(port));

    //bind
	bind(*socketFd, (struct sockaddr *)&ipv4Addr, sizeof(ipv4Addr));

	//listen
	listen(*socketFd, 10);

	return 0;
}