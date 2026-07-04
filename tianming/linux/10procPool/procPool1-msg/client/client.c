#include <57header.h>

int main(int argc,char*argv[])
{
    char *port = "9999";
	char *ip   = "192.168.43.132";

	int net_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    connect(net_fd, (struct sockaddr *)&addr, sizeof(addr));


    char fileName[60] = {0};
    recv(net_fd, fileName, sizeof(fileName), 0);
    printf("msg: %s \n", fileName);

    // int file_fd = open(fileName, O_RDWR|O_CREAT, 0600);

    // char buf[1024] = {0};
    // recv(net_fd, buf, sizeof(buf), 0);

    // printf("buf: %s \n", buf);
    // write(file_fd, buf, strlen(buf));

    close(net_fd);
    return 0;
}

