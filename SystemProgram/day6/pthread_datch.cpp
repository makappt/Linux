// 两种方法实现线程分离
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

void *func(void *num)
{
}
/*
注意！！！
一旦线程被创建，就没有标准的、直接的方法来查询其分离状态
注意，通过线程属性分离线程，使用的pthread_attr_getdetachstate获得的属性不是线程的属性，是attr属性对象的分离属性
*/
int main()
{
    pthread_t thd;
#if 0
    // 通过更改线程属性实现线程分离
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&thd, &attr, func, nullptr);
    if (ret != 0)
    {
        std::cout << "pthread_create failed";
        exit(-1);
    }
    int detachstate;
    ret = pthread_attr_getdetachstate(&attr, &detachstate);
    if (ret != 0)
    {
        std::cout << "ptread_getdetachstate failed";
        exit(-1);
    }
    if (detachstate == PTHREAD_CREATE_DETACHED)
    {
        std::cout << "thread is detached" << std::endl;
    }
    else if (detachstate == PTHREAD_CREATE_JOINABLE)
    {
        std::cout << "thread is joinable";
    }

#endif

#if 1
    int ret = pthread_create(&thd, nullptr, func, nullptr);
    if (ret != 0)
    {
        std::cout << "pthread_create failed";
        exit(-1);
    }
    ret = pthread_detach(thd);
    if (ret != 0)
    {
        std::cout << "pthread_detach is failed" << std::endl;
        exit(-1);
    }
    else if (ret == 0)
    {
        std::cout << "pthread_detach successful" << std::endl;
    }
#endif
    return 0;
}
