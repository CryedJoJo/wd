/*
 * 用户B — 基于命名管道(FIFO)的双向聊天程序
 * 通过 "1.pipe" 接收用户A的消息，通过 "2.pipe" 发送消息给用户A
 * 需要预先创建两个管道文件: mkfifo 1.pipe 2.pipe
 *
 * 系统接口说明:
 *   open(const char *path, int flags) -> int
 *       - 打开 FIFO 文件，flags: O_RDONLY(只读)/O_WRONLY(只写)
 *       - 默认阻塞打开，直到对端同时打开同一管道
 *       - 返回: 文件描述符(fd) / -1(失败)
 *
 *   read(int fd, void *buf, size_t count) -> ssize_t
 *       - 从 fd 读取最多 count 字节到 buf
 *       - 返回: 实际读取字节数 / 0(读到EOF/对端关闭) / -1(错误)
 *
 *   write(int fd, const void *buf, size_t count) -> ssize_t
 *       - 向 fd 写入 count 字节(来自 buf)
 *       - 返回: 实际写入字节数 / -1(错误)
 *
 *   close(int fd) -> int
 *       - 关闭文件描述符，释放系统资源
 *       - 返回: 0(成功) / -1(失败)
 */

#include <stdio.h>   // printf
#include <unistd.h>  // read, write, close, STDIN_FILENO
#include <fcntl.h>   // open, O_WRONLY, O_RDONLY
#include <strings.h> // bzero

int main(int argc, char *argv[])
{
	int fd_read  = open("1.pipe", O_RDONLY);
	int fd_write = open("2.pipe", O_WRONLY);

	while(1) {
		char buf[60];
		bzero(buf, sizeof(buf));
		// 读取1.pipe: 打印到标准输出
		int read_num = read(fd_read, buf, sizeof(buf));
		if(read_num == 0) {
			printf("对方断开链接 \n");
			break;
		}
		printf("UserA: %s ", buf);

		bzero(buf, sizeof(buf));
		// 读取标准输入: 通过2.pipe发送给User_A
		int read_stdin = read(STDIN_FILENO, buf, sizeof(buf));
		if(read_stdin == 0) {
			// 用户按下ctrl+d; 输入文件终止符; 终止标准输入; read返回0
			break;
		}
		write(fd_write, buf, sizeof(buf));
	}

	close(fd_write);
	close(fd_read);
	return 0;
}
