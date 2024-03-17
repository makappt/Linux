#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    int fd[2];
    char buf;

    if (pipe(fd) == -1)
    {
        cout << "pipe failed!" << endl;
        exit(1);
    }

    // 创建一个子进程，子进程负责读，父进程负责写
    pid_t pid = fork();
    if (pid == 0)
    {                 // 子进程
        close(fd[1]); // 关闭写端
        while (read(fd[0], &buf, 1) > 0)
        {
            cout << buf;
        }
        cout << endl;
        close(fd[0]); // 关闭读端
    }
    else
    {
        close(fd[0]); // 关闭读端
        const char *str = "jianzhe,nihao";
        write(fd[1], str, strlen(str)); // 使用strlen而不是sizeof
        close(fd[1]);                   // 关闭写端，触发EOF
        wait(nullptr);                  // 回收子进程
    }

    return 0;
}

// pipe 的读写行为
/*
读：1）写端全部关闭，返回读取到的字节数
    2）写端未全部关闭，阻塞等待

写： 1）读端全部关闭，异常结束
    2）读端未全部关闭   1）缓冲区已满，写端阻塞
                      2)缓冲区未满，继续读取
*/