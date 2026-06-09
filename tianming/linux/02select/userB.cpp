#include <stdio.h>       // printf
#include <unistd.h>      // read, write, close, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
#include <string.h>      // bzero
#include <fcntl.h>       // open, O_WRONLY, O_RDONLY
#include <sys/time.h>    // struct timeval (select 超时参数类型)
#include <sys/select.h>  // select, fd_set, FD_ZERO, FD_SET, FD_ISSET
#include <sys/types.h>   // 与 select 配合的类型定义

/*
 * 逻辑说明：
 * 本程序是双进程管道通信中的 userB 端。
 * 打开 1.pipe 用于读取，2.pipe 用于写入（与 userA 相反）。
 * 在主循环中通过 select() 同时监听 stdin 和读管道：
 *   - stdin 就绪 → 从 STDERR_FILENO（通常也是终端）读取输入，写入写管道 (2.pipe) 发送给 userA。
 *   - 读管道就绪 → 从读管道 (1.pipe) 读取 userA 发来的消息，打印到屏幕。
 * select 返回 -1 或任一管道读取失败则退出循环，关闭管道描述符后结束。
 */
int main()
{
	int read_pipe  = open("1.pipe", O_RDONLY);
	int write_pipe = open("2.pipe", O_WRONLY);

	if (read_pipe == -1) {
		perror("open read pipe failed");
		return 1;
	}
	if (write_pipe == -1) {
		perror("open write pipe failed");
		close(read_pipe);
		return 1;
	}

	//构建集合
	fd_set set;
	char   buf[60];
	printf("17\n");

	while(1) {
		//初始化集合
		bzero(buf, sizeof(buf));
		FD_ZERO(&set);

		//放入文件标识符到集合
		FD_SET(STDIN_FILENO, &set);
		FD_SET(read_pipe, &set);

		//select监听集合：nfds 应为最大 fd + 1
		int maxfd = (read_pipe > STDIN_FILENO ? read_pipe : STDIN_FILENO) + 1;
		int sel = select(maxfd, &set, NULL, NULL, NULL);
		if (sel == -1) {
			perror("select");
			break;
		}
		//处理就绪的文件描述符
		if(FD_ISSET(STDIN_FILENO, &set)) {
			// 修正：从 STDIN_FILENO 读取，非 STDERR_FILENO
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
		}

		if(FD_ISSET(read_pipe, &set)) {
			int ret = read(read_pipe, buf, sizeof(buf));
			if (ret < 0) {
				perror("read from pipe");
				break;
			}
			if (ret == 0) {
				printf("peer closed\n");
				break;
			}

			// buf 可能不含 '\0'，按实际字节输出到 stdout
			printf("userA:");
			ssize_t w = write(STDOUT_FILENO, buf, (size_t)ret);
			if (w != ret) {
				perror("write stdout");
				break;
			}
			if (buf[ret-1] != '\n') putchar('\n');
		}
	}

	//关闭管道
	close(write_pipe);
	close(read_pipe);
}
