#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sys_err(char *str)
{
    perror(str);
    exit(1);
}

void do_sig_child(int signo)
{
    int status;
    pid_t pid;

    //    if ((pid = waitpid(0, &status, WNOHANG)) > 0) {
    while ((pid = waitpid(0, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
            printf("------------child %d exit %d\n", pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
    }
}

int main(void)
{
    pid_t pid;
    int i;
    // 阻塞SIGCHLD
    for (i = 0; i < 10; i++)
    {
        if ((pid = fork()) == 0)
            break;
        else if (pid < 0)
            sys_err("fork");
    }

    if (pid == 0)
    { // 10个子进程
        int n = 1;
        while (n--)
        {
            printf("child ID %d\n", getpid());
            sleep(1);
        }
        return i + 1;
    }
    else if (pid > 0) // 即使子进程在还没有创建联系之前就结束了，但是while循环还是能够将全部进程回收
    {
        // SIGCHLD阻塞
        struct sigaction act;

        act.sa_handler = do_sig_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, NULL);
        // 解除对SIGCHLD的阻塞

        while (1)
        {
            printf("Parent ID %d\n", getpid());
            sleep(1);
        }
    }

    return 0;
}
