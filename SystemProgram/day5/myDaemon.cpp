// 使用setsid创建守护进程
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
// 创建守护进程步骤
/*
    1.创建子进程
    2.在子进程中创建会话
    3.更改文件目录到根目录，确保程序不会因为工作目录不存在失败
    4.重设文件权限掩码，防止继承的文件创建屏蔽字拒绝某些权限
    5.关闭文件描述符或重定向文件描述符
    6.开始执行守护进程核心工作
    7.守护进程退出处理程序模型
*/

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(-1);
    }
    if (pid > 0)
        return 0;
    pid = setsid();
    if (pid == -1)
    {
        perror("setsid failed");
        exit(-1);
    }
    int ret = chdir("/home");
    if (ret == -1)
    {
        perror("chdir failed");
        exit(-1);
    }
    umask(0002); // 更改文件权限掩码
    int fd = open("/dev/null", O_RDWR);
    close(STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    while (1)
        ;
    return 0;
}