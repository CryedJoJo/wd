#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>

int main()
{
	int write_pipe = open("1.pipe", O_WRONLY);
	int read_pipe  = open("2.pipe", O_RDONLY);

	struct timeval tv;
	fd_set         set;
	char   buf[60];

	while(1) {
		bzero(&buf, sizeof(buf));
		FD_ZERO(&set);

		FD_SET(read_pipe, &set);
		FD_SET(STDIN_FILENO, &set);

		int ret = select(10, &set, NULL, NULL, NULL);
		if(ret == -1) {
			break;
		}

		if(FD_ISSET(STDIN_FILENO, &set)) {
			int ret = read(STDIN_FILENO, buf, sizeof(buf));
			if(ret == -1) {
				break;
			}
			write(write_pipe, buf, sizeof(buf));
		}

		if(FD_ISSET(read_pipe, &set)) {
			int ret = read(read_pipe, buf, sizeof(buf));
			if(ret == -1) {
				break;
			}
			printf("b:%s", buf);
		}
	}
	close(write_pipe);
	close(read_pipe);
}