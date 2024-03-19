// 创建5个子进程，杀死3号子进程
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <cstdlib>
#include <string.h>
#include <cerrno>
#include <fcntl.h>
#include <cstring>

int main()
{
    pid_t pid;
    int i;
    for (i = 0; i < 5; i++)
    {
        pid_t spid = fork();
        if (spid == -1)
        {
            std::cout << "fork failed :" << std::strerror(errno) << std::endl;
            exit(-1);
        }
        if (spid == 0)
        {
            break;
        }
        else
        {
            if (i == 2)
            {
                pid = spid;
            }
        }
    }

    if (i < 5)
    {
        while (1)
        {
            std::cout << "child : " << i << " pid = " << getpid() << std::endl;
            sleep(1);
        }
    }
    else
    {
        sleep(1);
        kill(pid, SIGKILL);
        while (1)
            ;
    }

    return 0;
}