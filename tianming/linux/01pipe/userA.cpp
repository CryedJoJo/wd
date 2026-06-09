#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	int write_fd = open("1.pipe", O_WRONLY);
	int read_fd  = open("2.pipe", O_RDONLY);

	while(1) {
		char buff[60];
		//清空buff
		bzero(buff, sizeof(buff));
		//从STDIN_FILEON标准输入中读取数据，写入buff
		int read_stdin = read(STDIN_FILENO, buff, sizeof(buff));
		//如果读到0，就break
		if(read_stdin == 0) {
			break;
		}
		//从buff中往write_fd中写数据
		int write_buf = write(write_fd, buff, sizeof(buff));

		//清空buff
		bzero(buff, sizeof(buff));
		//从read_fd中读取数据，写入buff
		int read_buf = read(read_fd, buff, sizeof(buff));
		//如果读到0 就break
		if(read_buf == 0) {
			break;
		}
		printf("B:%s", buff);
	}

	//关闭pipe
	close(write_fd);
	close(read_fd);
}