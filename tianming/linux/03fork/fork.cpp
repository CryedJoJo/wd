#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int global_var = 100;

int main()
{
    pid_t pid = fork();

    if (pid == 0) {
        printf("[child]  pid=%d, ppid=%d\n", getpid(), getppid());
        printf("[child]  global_var 改前: %d, 地址: %p\n", global_var, &global_var);
        global_var = 200;
        printf("[child]  global_var 改后: %d, 地址: %p\n", global_var, &global_var);

        printf("[child]  睡眠 2 秒, 假装干点活...\n");
        sleep(2);
        printf("[child]  退出\n");
        exit(42);
    } else if (pid > 0) {
        printf("[parent] pid=%d, child pid=%d\n", getpid(), pid);
        printf("[parent] global_var: %d, 地址: %p\n", global_var, &global_var);

        printf("[parent] 等子进程...\n");
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("[parent] 子进程正常退出, 返回值: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[parent] 子进程被信号 %d 杀死\n", WTERMSIG(status));
        }

        printf("[parent] 再见\n");
    } else {
        perror("fork");
        return 1;
    }

    return 0;
}
