// 通过文件进行由血缘的进程间的通信
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int fd1, fd2;

    pid_t pid = fork();
    if (pid == -1)
    {
        std::cout << "fork failed!" << std::endl;
        exit(0);
    }

    if (pid == 0) // 子进程负责写
    {
        fd2 = open("file.txt", O_RDWR);
        if (fd2 == -1)
        {
            std::cout << "fd2 open failed\n";
            exit(0);
        }
        const char *str = "hello,world!\n";
        write(fd2, str, strlen(str));
        close(fd2);
    }
    else
    {
        fd1 = open("file.txt", O_RDWR);
        if (fd1 == -1)
        {
            std::cout << "fd2 open failed\n";
            exit(0);
        }
        char buf[1024];
        ssize_t n = read(fd1, &buf, sizeof(buf));
        write(STDOUT_FILENO, buf, n);
        close(fd1);
        wait(nullptr);
    }

    return 0;
}