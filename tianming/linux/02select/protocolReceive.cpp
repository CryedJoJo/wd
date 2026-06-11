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
    int rfd = open("1.pipe", O_RDONLY);
    if (rfd == -1) {
        // perror - 打印errno对应的错误信息到stderr
        // 原型: void perror(const char *s);
        perror("open");
        return 1;
    }

    char buf[4096];
    int len;
    while(1){
        // read - 从文件描述符读取指定字节数
        // 原型: ssize_t read(int fd, void *buf, size_t count);
        // 返回值: >0 实际读到的字节数, =0 读到文件尾(管道写端关闭), -1 出错
        // 注意: read不保证一次读完count字节，需循环读取直到收满
        //先读长度
        ssize_t n = read(rfd, &len, sizeof(len));
        if (n <= 0) {
            break;
        }

        //再读数据
        ssize_t total = 0;
        while (total < len) {
            ssize_t ret = read(rfd, buf + total, len - total);
            if (ret <= 0) {
                break;
            }
            total += ret;
        }

        buf[total] = '\0';
        // printf - 格式化输出到stdout
        // 原型: int printf(const char *format, ...);
        // 返回值: 成功返回输出的字符数，失败返回负数
        printf("%s", buf);
        // fflush - 刷新文件流缓冲区，强制输出
        // 原型: int fflush(FILE *stream);
        // 返回值: 成功返回0，失败返回EOF
        fflush(stdout);
    }

    // close - 关闭文件描述符
    // 原型: int close(int fd);
    // 关闭后fd不再可用，管道写端收到SIGPIPE或写入返回-1
    close(rfd);
    return 0;
}
