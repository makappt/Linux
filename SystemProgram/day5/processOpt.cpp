/*
进程组相关函数
getpgrp：获得当前进程进程组id
getpgid: 获得指定进程进程组id
setpgid：改变指定进程的所属进程组
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        exit(-1);
    }
    if (pid == 0)
    {
        std::cout << "子进程id = " << getpid() << std::endl;
        std::cout << "子进程的组id = " << getpgrp() << std::endl;
        std::cout << "子进程的组id = " << getpgid(0) << std::endl;
        sleep(5);
        std::cout << "---------------" << std::endl;
        std::cout << "子进程id = " << getpid() << std::endl;
        std::cout << "子进程的组id = " << getpgrp() << std::endl;
        std::cout << "子进程的组id = " << getpgid(0) << std::endl;
    }
    if (pid > 0)
    {
        sleep(1);
        setpgid(pid, pid);
        while (1)
            ;
    }
    return 0;
}