#include <stdio.h>
#include <sys/wait.h>

int main()
{
	printf("-");
	fork();
	printf("-");
	fork();
	printf("%d\n", getpid());
}