#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

int main()
{

	key_t key = ftok("/txt", 1);
	int   id  = shmget(key, 4096, 0600 | IPC_CREAT);
	void *pos = shmat(id, NULL, 0);

	if(fork() == 0) {
		char *buf = "hello";
		strcpy(pos, buf);
	} else {
		wait(NULL);
		char buf[1024];
		strcpy(buf, pos);
		printf("son:%s\n", buf);
	}

	return 0;
}