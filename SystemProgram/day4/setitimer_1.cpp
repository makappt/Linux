// 使用setitimer 配合signal函数实现简单的信号绑定，与QT的信号机制一样
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>

using namespace std;

int i = 1; // 只能使用全局变量

void signalHander(int signal)
{
    cout << i++ << endl;
}

int main()
{
    struct itimerval it;
    it.it_value.tv_sec = 1;
    it.it_value.tv_usec = 500000;

    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;

    signal(SIGALRM, signalHander);
    setitimer(ITIMER_REAL, &it, nullptr);
    while (1)
    {
    }
    return 0;
}