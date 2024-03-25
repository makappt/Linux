#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int n = 0;
pid_t pid;

void sys_err(char *str)
{
    perror(str);
    exit(1);
}

void do_sig_child(int num)
{
    printf("I am child  %d\t%d\n", getpid(), n);
    n += 2;
    kill(getppid(), SIGUSR2); // 通知父进程
}

void do_sig_parent(int num)
{
    printf("I am parent %d\t%d\n", getpid(), n);
    n += 2;
    kill(pid, SIGUSR1); // 通知子进程
}

int main(void)
{
    struct sigaction act;

    if ((pid = fork()) < 0)
        sys_err("fork");
    else if (pid > 0)
    { // 父进程
        n = 1;
        act.sa_handler = do_sig_parent;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGUSR2, &act, NULL); // 父进程注册SIGUSR2信号处理函数

        do_sig_parent(0); // 父进程首先数数

        while (1)
        {
            pause(); // 父进程等待信号
        }
    }
    else
    { // 子进程
        n = 2;
        act.sa_handler = do_sig_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGUSR1, &act, NULL); // 子进程注册SIGUSR1信号处理函数

        while (1)
        {
            pause(); // 子进程等待信号
        }
    }

    return 0;
}
