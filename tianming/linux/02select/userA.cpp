#include <stdio.h>       // printf
#include <fcntl.h>       // open, O_WRONLY, O_RDONLY
#include <unistd.h>      // read, write, close, STDIN_FILENO
#include <string.h>      // bzero
#include <sys/select.h>  // select, fd_set, FD_ZERO, FD_SET, FD_ISSET
#include <sys/time.h>    // struct timeval (select 超时参数类型)
#include <sys/types.h>   // 与 select 配合的类型定义

/*
 * 逻辑说明：
 * 本程序是双进程管道通信中的 userA 端。
 * 打开 1.pipe 用于写入，2.pipe 用于读取。
 * 在主循环中通过 select() 同时监听 stdin 和读管道：
 *   - stdin 就绪 → 从键盘读取输入，写入写管道 (1.pipe) 发送给 userB。
 *   - 读管道就绪 → 从读管道 (2.pipe) 读取 userB 发来的消息，打印到屏幕。
 * 任一管道读取失败则退出循环，关闭管道描述符后结束。
 */
int main()
{
	//打开管道
	int write_pipe = open("1.pipe", O_WRONLY);
	int read_pipe  = open("2.pipe", O_RDONLY);

	if (write_pipe == -1) {
		perror("open write pipe failed");
		return 1;
	}
	if (read_pipe == -1) {
		perror("open read pipe failed");
		close(write_pipe);
		return 1;
	}

	//构造集合
	fd_set set;
	char   buf[60];

	while(1) {
		//初始化集合
		FD_ZERO(&set);
		bzero(buf, sizeof(buf));

		//放入描述符
		FD_SET(STDIN_FILENO, &set);
		FD_SET(read_pipe, &set);

		//监听：nfds 应为最大 fd + 1
		int maxfd = (read_pipe > STDIN_FILENO ? read_pipe : STDIN_FILENO) + 1;
		int selret = select(maxfd, &set, NULL, NULL, NULL);
		if (selret == -1) {
			perror("select");
			break;
		}

		//处理就绪
		if(FD_ISSET(STDIN_FILENO, &set)){
			//写就绪
			int ret = read(STDIN_FILENO, buf, sizeof(buf));
			if (ret < 0) {
				perror("read stdin");
				break;
			}
			if (ret == 0) {
				printf("stdin closed\n");
				break;
			}

			// 只写入实际读取到的字节数
			ssize_t w = write(write_pipe, buf, (size_t)ret);
			if (w != ret) {
				perror("write to pipe");
				break;
			}
			continue;
		}

		if(FD_ISSET(read_pipe, &set)){
			int ret = read(read_pipe, buf, sizeof(buf));
			if (ret < 0) {
				perror("read from pipe");
				break;
			}
			if (ret == 0) {
				printf("peer closed\n");
				break;
			}

			// buf 可能不含 '\0'，按实际字节输出
			printf("userB:");
			ssize_t w = write(STDOUT_FILENO, buf, (size_t)ret);
			if (w != ret) {
				perror("write stdout");
				break;
			}
			if (buf[ret-1] != '\n') putchar('\n');
		}
	}
	close(write_pipe);
	close(read_pipe);
	//关闭管道
}
