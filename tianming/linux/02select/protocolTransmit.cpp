#include <stdio.h>   // FILE, stdin/stdout/stderr, fgets, printf, perror, fflush
#include <string.h>  // strlen, strchr, strcpy, memset 等字符串操作
#include <unistd.h>  // read, write, close, STDIN_FILENO 等POSIX系统调用
#include <fcntl.h>   // open, O_RDONLY/O_WRONLY/O_RDWR 等文件控制宏
#include <stdlib.h>  // exit, malloc, free, atoi 等通用工具函数

int main(){
    // open - 打开文件/管道，返回文件描述符
    // 原型: int open(const char *pathname, int flags);
    // flags: O_RDONLY(只读) O_WRONLY(只写) O_RDWR(读写)
    // 返回值: 成功返回>=0的文件描述符，失败返回-1并设置errno
    int wfd = open("1.pipe", O_WRONLY);
    if (wfd == -1) {
        // perror - 打印errno对应的错误信息到stderr
        // 原型: void perror(const char *s);
        // 输出: "s: 错误描述"
        perror("open");
        return 1;
    }

    char buf[4096];
    while(1){
        //获取STDIN_FILENO 标准输入的数据
        //以换行为边界
        // fgets - 从文件流中读取一行（遇到换行或EOF停止）
        // 原型: char *fgets(char *s, int size, FILE *stream);
        // 参数: s(缓冲区) size(缓冲区大小) stream(文件流)
        // 特点: 会自动在末尾加'\0'，最多读 size-1 个字符
        // 返回值: 成功返回s，失败或到达文件尾返回NULL
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }

        // strlen - 计算字符串长度（不含结尾'\0'）
        // 原型: size_t strlen(const char *s);
        // 返回值: 字符串的字节数
        int len = strlen(buf);

        // write - 向文件描述符写入指定字节数
        // 原型: ssize_t write(int fd, const void *buf, size_t count);
        // 返回值: 成功返回写入的字节数，失败返回-1
        // 注意: 管道/PIPE默认最大写入PIPE_BUF字节（通常4096），不能保证一次写完所有数据
        //发送字符串长度
        write(wfd, &len, sizeof(len));

        //发送字符串
        write(wfd, buf, len);
    }

    // close - 关闭文件描述符
    // 原型: int close(int fd);
    // 返回值: 成功返回0，失败返回-1
    // 关闭后fd不再可用，管道读端读到0表示写端已关闭
    close(wfd);
    return 0;
}
