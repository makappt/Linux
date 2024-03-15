#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
using namespace std;

extern char **environ;

int main()
{
    // 将ps aux的输出结果通过dup2重定向输出到result.txt文件中
    int file = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file < 0)
    {
        cout << "open failed!" << endl;
        exit(0);
    }

    int flag = dup2(file, STDOUT_FILENO);
    if (flag < 0)
    {
        cout << "dup2 failed!" << endl;
    }
    close(file);

#if 1
    // 使用execlp查询当前系统的进程信息
    int ret = execlp("ps", "ps", "aux", nullptr);
    if (ret == -1)
    {
        cout << "execlp failed!" << endl;
        exit(0);
    }
#endif
#if 0
    const char *arr[] = {"ps", "aux", nullptr};
    // 使用execvp查询当前系统进程的信息
    if (execvp("ps", const_cast<char *const *>(arr)) == -1)
    {
        cout << "execvp failed!" << endl;
        exit(0);
    }
#endif
    return 0;
}
// p:环境变量
// v:数组
// l:list,列表

// execl:当你知道要执行的命令及其参数，并且参数数量固定时使用
// 示例：execl("/bin/ls", "ls", "-l", (char *)NULL);

//  execv：适用于参数数量可变，且参数在程序运行时确定的情况
// 示例：execv("/bin/ls", argv)

//  execlp: 不知道程序的完整路径，需要系统搜索PATH环境变量来找到程序时使用,多用于系统命令
// 示例: execlp("ls", "ls", "-l", (char *)NULL)

//  execvp: 不需要知道程序的完整路径，只需要知道程序的名字即可，编辑器会自动从环境变量中寻找对应的程序
// 示例：execvp("ps", const_cast<char *const *>(arr))

//  execle：当你需要完全控制执行程序的环境，并且知道程序的完整路径时使用
// 特点：允许传递一个环境变量数组作为参数给新程序。
// 示例：execle("/bin/ls", "ls", "-l", (char *)NULL, envp);其中envp是环境变量数组。

//  execve：当你需要完全控制执行程序的环境，并且知道程序的完整路径时使用。
// 特点：是exec函数族中最灵活的函数，允许指定程序路径、参数列表和环境变量。
//  示例：execve("/bin/ls", argv, envp);