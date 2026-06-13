#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	key_t key = ftok("./text", 1);

	int id = shmget(key, 4096, 0600 | IPC_CREAT);

	void *pos = shmat(id, NULL, 0);

	if(fork() == 0) {
		char *buf = "hello";
		memcpy(pos, buf, 6);
	} else {
		wait(NULL);
		char buf[6];
		memcpy(buf, pos, 6);
		printf("read:%s\n", buf);
	}
	return 0;
}