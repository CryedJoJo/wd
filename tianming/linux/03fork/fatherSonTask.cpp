#include <stdio.h>
#include <sys/wait.h>

int main()
{

	int pid = fork();
	if(pid == 0) {
		printf("son:%d, father:%d\n", getpid(), getppid());
	} else {
		printf("son:%d, myself:%d\n", pid, getpid());
	}
	return 0;
}