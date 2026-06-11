#include <stdio.h>      // printf
#include <fcntl.h>      // open, O_WRONLY, O_RDONLY
#include <unistd.h>     // read, write, close, STDIN_FILENO
#include <string.h>     // bzero
#include <sys/select.h> // select, fd_set, FD_ZERO, FD_SET, FD_ISSET
#include <sys/time.h>   // struct timeval (select 超时参数类型)
#include <time.h>
#include <sys/types.h> // 与 select 配合的类型定义

//10s 没有说话踢出聊天
int main()
{
	int write_pipe = open("1.pipe", O_WRONLY);
	int read_pipe  = open("2.pipe", O_RDONLY);

	fd_set set;
	char   buf[60];

	//获取时间
	/*
	// 问题: 在 C 中 timeval 需要用 struct 关键字，直接使用 timeval 会导致编译报未定义类型错误
	timeval tv;
	*/
	struct timeval tv;
	tv.tv_sec  = 1;
	tv.tv_usec = 0;
	int now    = time(NULL);
	int last   = time(NULL);

	while(1) {
		//初始化set
		bzero(buf, sizeof(buf));
		FD_ZERO(&set);

		//放入fd到set
		FD_SET(STDIN_FILENO, &set);
		FD_SET(read_pipe, &set);

		//select监听
		/*
		// 问题1: select() 会修改 tv 为剩余时间，导致后续循环中超时失效（1秒仅生效一次）
		// 问题2: 第一个参数应为最大文件描述符+1，硬编码10不健壮
		select(10, &set, NULL, NULL, &tv);
		*/
		tv.tv_sec  = 1; // 每次循环重新设置超时时间，防止被 select 修改后失效
		tv.tv_usec = 0;
		int maxfd  = STDIN_FILENO > read_pipe ? STDIN_FILENO : read_pipe;
		select(maxfd + 1, &set, NULL, NULL, &tv);

		if(FD_ISSET(STDIN_FILENO, &set)) {
			//处理写
			int ret = read(STDIN_FILENO, buf, sizeof(buf));
			if(ret == 0) {
				break;
			}
			/*
			// 问题: read() 返回的 ret 是实际读取字节数，sizeof(buf) 是缓冲区大小（60），
			//       写入 sizeof(buf) 会发送未填充的垃圾数据（如上次残留的内容）
			write(write_pipe, buf, sizeof(buf));
			*/
			write(write_pipe, buf, ret);
			bzero(buf, sizeof(buf));
			/*
			// 问题: 只写了注释"更新last"，但缺少实际更新 last 的代码，
			//       导致用户从键盘输入时 last 未被刷新，10秒后会被误踢出
			*/
			last = time(NULL); // 用户输入说明活跃，刷新 last 防止被踢
		}

		if(FD_ISSET(read_pipe, &set)) {
			//处理写
			int ret = read(read_pipe, buf, sizeof(buf));
			if(ret == 0) {
				break;
			}
			printf("B:%s", buf);
			//更新last
			last = time(NULL);
		}
		now = time(NULL);
		if(now - last >= 10) {
			break;
		}
	}

	close(write_pipe);
	close(read_pipe);
	return 0;
}
