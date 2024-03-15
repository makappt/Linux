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
