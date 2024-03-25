// 使用pause和alarm实现sleep函数效果，要求实现不会出现失去cpu后永久等待的情况
// 信号发射时，需要考虑到失去cpu的情况，不然容易出现问题！！！
// pause函数的危险性，任何信号都能使pause函数停止阻塞，无法解决失去cpu的情况，需要使用sigsuspend函数代替
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

using namespace std;

void sigHander(int act)
{
}

void mysleep(int time)
{
    sigset_t newMask, oldMask, processMask;
    struct sigaction newAct, oldAct;
    newAct.sa_handler = sigHander;

    sigemptyset(&newAct.sa_mask);
    newAct.sa_flags = 0;
    sigaction(SIGALRM, &newAct, &oldAct);
    sigemptyset(&newMask);
    sigaddset(&newMask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newMask, &oldMask); // 将信号屏蔽

    struct itimerval it; // 设置闹钟
    it.it_value.tv_sec = time;
    it.it_value.tv_usec = 0;
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &it, nullptr);

    processMask = oldMask;            // 将processMask设置为最初的屏蔽集
    sigdelset(&processMask, SIGALRM); // 确保将信号没有在屏蔽集当中

    sigsuspend(&processMask);
    sigpending(&oldMask);

    sigaction(SIGALRM, &oldAct, nullptr);        // 恢复默认处理函数
    sigprocmask(SIG_SETMASK, &oldMask, nullptr); // 恢复最初的屏蔽集
}

int main()
{
    while (1)
    {
        mysleep(2);
        cout << "Two seconds passed\n";
    }

    return 0;
}