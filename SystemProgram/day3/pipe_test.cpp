// 练习：使用管道实现父子进程间通信，完成：ls | wc –l。假定父进程实现ls，子进程实现wc。
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        cout << "Pipe failed!" << endl;
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        cout << "Fork failed!" << endl;
        exit(1);
    }

    if (pid == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO); // Redirect standard input to the pipe
        close(fd[0]);
        execlp("wc", "wc", "-l", nullptr);
        exit(1);
    }
    else
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO); // Redirect standard output to the pipe
        close(fd[1]);
        execlp("ls", "ls", nullptr);
        wait(nullptr);
        exit(1);
    }
}
