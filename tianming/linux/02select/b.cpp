#include <stdio.h>
#include <string.h> //bzero
#include <fcntl.h>  //open
#include <unistd.h> //write read
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>

int main()
{
	int read_pipe  = open("1.pipe", O_RDONLY);
	int write_pipe = open("2.pipe", O_WRONLY);

	char   buf[60];
	fd_set set;

	while(1) {
		bzero(buf, sizeof(buf));
		FD_ZERO(&set);

		FD_SET(STDIN_FILENO, &set);
		FD_SET(read_pipe, &set);

		select(10, &set, NULL, NULL, NULL);

		if(FD_ISSET(STDIN_FILENO, &set)) {
			int ret = read(STDIN_FILENO, buf, sizeof(buf));
			if(ret == 1) {
				break;
			}
			write(write_pipe, buf, sizeof(buf));
		}

		if(FD_ISSET(read_pipe, &set)) {
			int ret = read(read_pipe, buf, sizeof(buf));
			if(ret == -1) {
				break;
			}
			printf("a:%s", buf);
		}
	}

	close(write_pipe);
	close(read_pipe);
}
