// 杀死线程
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

void *func(void *num)
{
    while (1)
    {
        sleep(3);
        pthread_testcancel(); // 设置取消点，pthread_cancel函数杀死线程需要到达取消点才成功，取消点可以大致认为是系统调用
    }
}

int main()
{
    pthread_t thd;
    pthread_create(&thd, nullptr, func, nullptr);
    pthread_cancel(thd);
    sleep(1);
    int ret = pthread_join(thd, nullptr);
    if (ret == 0)
    {
        std::cout << "pthread_cancel successful" << std::endl;
    }
    else
    {
        std::cout << "pthread_cancel failed" << std::endl;
    }

    return 0;
}