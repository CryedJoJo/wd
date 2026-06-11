// 问：父进程 wait 子进程退出时，为什么没有等待 3s？
// 答：因为 execl 成功后会将子进程完全替换为 readDir 的镜像，不再返回。
//    sleep(3) 位于 execl 之后，永远不会被执行到。
//    子进程实际运行 readDir（执行 ls），瞬间完成退出，父进程 wait 立即返回。
//    只有当 execl 失败时才会执行 sleep(3)，但这里 execl 成功。

#include <sys/wait.h>
#include <stdio.h>

int main(){
    if(fork() == 0){
		execl("./readDir", "./readDir", NULL);
		// 问：为什么之前没写 NULL 也没出问题？
		// 答：execl 通过 NULL 哨兵确定参数结尾。没写 NULL 时 va_arg 会读到栈上垃圾值，
		//    碰巧为 0（NULL）便不会崩溃。这是未定义行为，依赖运气，依赖编译器/优化等级。
		sleep(3);
		return 0;
	} else {
		wait(NULL);
		printf("end\n");
	}
	return 0;
}