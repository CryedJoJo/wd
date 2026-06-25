#include <stdio.h>      /* printf */
#include <string.h>     /* memcpy */
#include <sys/ipc.h>    /* ftok, key_t, IPC_CREAT, IPC_EXCL */
#include <sys/shm.h>    /* shmget, shmat, shmdt, shmctl, SHM_RDONLY */
#include <sys/wait.h>   /* wait */
#include <unistd.h>     /* fork */

/*
 * 本文件使用的 System V 共享内存 / IPC 相关接口与宏说明：
 * - ftok(path, proj_id) -> key_t
 *     生成用于 IPC 的 key。path 应指向一个已存在的文件，proj_id 为 1..255 的小整数。
 * - shmget(key, size, shmflg) -> int (shmid)
 *     创建或获取一个共享内存段。常用标志有 IPC_CREAT、IPC_EXCL，权限位如 0600/0666 等。
 * - shmat(shmid, shmaddr, shmflg) -> void*
 *     将共享内存段映射到进程地址空间。若失败返回 (void*)-1。
 * - shmdt(shmaddr) -> int
 *     解除映射（本示例未调用，但生产代码应在用完后调用以释放进程映射）。
 * - shmctl(shmid, IPC_RMID, NULL)
 *     删除共享内存段（由进程在不再需要共享内存时执行以避免内核资源泄露）。
 * - 常用宏：IPC_CREAT, IPC_EXCL, SHM_RDONLY
 *
 * 建议的使用规范（接口使用规则见下方具体位置）：
 * 1) 使用 ftok 时确保 path 可访问且 proj_id 唯一以避免 key 冲突。
 * 2) 调用 shmget 后要检查返回值是否为 -1，出错需处理（如 errno 打印并退出）。
 * 3) shmat 返回值需与 (void*)-1 比较以检测错误；用完后请调用 shmdt 解除映射。
 * 4) 共享内存的同步需要额外机制（如信号量、信号或 wait/kill 等），不要依赖内存可见性的时序。
 * 5) 创建的共享内存在不再使用时应由负责的进程调用 shmctl(..., IPC_RMID, NULL) 删除。
 */

int main()
{
    /*
     * ftok 使用规则：
     * - path 必须指向一个已经存在的文件（通常使用可访问的常驻文件）。
     * - proj_id 应在 1..255 范围内，且与其他使用同一路径的 proj_id 区分以减少冲突。
     * - ftok 只是生成 key 的一种方便方式，不能保证全局唯一；在并发或多租户环境下需谨慎选取。
     */
    key_t key = ftok("./text", 1);

    /*
     * shmget 使用规则：
     * - size 必须大于 0，通常按页对齐更安全。
     * - 权限位由调用进程决策（如 0600 表示仅所有者读写）。
     * - 若希望在已存在时失败可使用 IPC_EXCL 联合 IPC_CREAT。
     * - 成功返回 shmid（非负整数），失败返回 -1（需检查 errno）。
     */
    int id = shmget(key, 4096, 0600 | IPC_CREAT);

    /*
     * shmat 使用规则：
     * - 若成功返回映射地址；失败返回 (void*)-1，需检测并处理错误。
     * - flags 可使用 SHM_RDONLY 指定只读映射，或 0 表示读写映射。
     * - 使用完成后应调用 shmdt(pos) 解除映射；若不再需要整个段还应调用 shmctl(..., IPC_RMID, NULL) 删除段。
     */
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
