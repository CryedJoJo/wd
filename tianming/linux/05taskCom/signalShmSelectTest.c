#define _GNU_SOURCE
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_SUCCESS
#include <unistd.h>     // fork, pipe, read, write, close, getppid
#include <string.h>     // strncpy
#include <signal.h>     // sigaction, sigemptyset, sigaddset, sigprocmask, sigsuspend, kill
#include <sys/wait.h>   // waitpid
#include <sys/select.h> // select, FD_ZERO, FD_SET, FD_ISSET
#include <sys/time.h>   // struct timeval
#include <sys/shm.h>    // shmget, shmat, shmdt, shmctl
#include <sys/ipc.h>    // IPC_CREAT, IPC_EXCL, IPC_RMID

/* 信号处理函数：收到 SIGUSR1 时仅设置标志，主循环通过 sigsuspend 返回后检查 */
static void func(int s)
{
	printf("%d: receive signal:%d\n", getpid(), s);
}

static void fun(int s)
{
	printf("receive signal %d", s);
	exit(0);
}

int main(void)
{
	// 1. pipe
	int pfd_1[2];
	int pfd_2[2];
	pipe(pfd_1);
	pipe(pfd_2);

	// 2. shared memory
	key_t key_1 = ftok("/tmp", 1);
	key_t key_2 = ftok("/tmp", 2);

	int   sid_1 = shmget(key_1, 64, IPC_CREAT | 0666);
	int   sid_2 = shmget(key_2, 64, IPC_CREAT | 0666);
	char *shm_1 = (char *)shmat(sid_1, NULL, 0);
	char *shm_2 = (char *)shmat(sid_2, NULL, 0);

	// 3. signal setup（必须在 fork 前做）
	sigset_t mask, old;
	sigemptyset(&mask);
	// sigaddset(&mask, SIGUSR1);
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = func;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);

	// 4. fork
	pid_t pid = fork();
	if(pid == 0) {
		/* child */
		signal(2, fun);
		sleep(4);

		close(pfd_1[1]);
		close(pfd_2[0]);

		strncpy(shm_2, "msg1 from son", 63);
		shm_2[63] = '\0';
		write(pfd_2[1], "!", 1);

		/* 等待 parent SIGUSR1 */
		sigsuspend(&old);
		printf("son got SIGUSR1, msg done\n");

		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(pfd_1[0], &fds);
		struct timeval tv  = {.tv_sec = 5, .tv_usec = 0};
		int            ret = select(pfd_1[0] + 1, &fds, NULL, NULL, &tv);
		if(ret > 0 && FD_ISSET(pfd_1[0], &fds)) {
			char c;
			read(pfd_1[0], &c, 1);
			printf("child read: %s\n", shm_1);
			kill(getppid(), SIGUSR1);
		}

		close(pfd_1[0]);
		close(pfd_2[1]);
		exit(EXIT_SUCCESS);
	}

	/* parent */
	close(pfd_1[0]);
	close(pfd_2[1]);

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(pfd_2[0], &fds);
	struct timeval tv  = {.tv_sec = 5, .tv_usec = 0};
	int            ret = select(pfd_2[0] + 1, &fds, NULL, NULL, &tv);
	if(ret > 0 && FD_ISSET(pfd_2[0], &fds)) {
		char c;
		read(pfd_2[0], &c, 1);
		printf("parent read: %s\n", shm_2);
		kill(pid, SIGUSR1);
	}

	strncpy(shm_1, "msg1 from parent", 63);
	shm_1[63] = '\0';
	write(pfd_1[1], "!", 1);
	sigsuspend(&old);
	printf("parent got SIGUSR1, msg done\n");

	// 5. cleanup
	waitpid(pid, NULL, 0);
	shmdt(shm_1);
	shmdt(shm_2);
	shmctl(sid_1, IPC_RMID, NULL);
	shmctl(sid_2, IPC_RMID, NULL);
	close(pfd_1[1]);
	close(pfd_2[0]);

	return 0;
}
