
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int pipefd[2];
	pipe(pipefd);

	if(fork() == 0) {
		close(pipefd[1]); // 只保留读
		char buf[5];
		read(pipefd[0], buf, 5);

		printf("child get message: %s \n", buf);
	} else {
		close(pipefd[0]); // 只保留写
		write(pipefd[1], "hello", 5);
		wait(NULL);
	}
	return 0;
}