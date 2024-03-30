#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

void *func(void *num)
{
    long number = (long)num; // 使用 long 来确保足够的大小存储指针
    std::cout << number + 1 << "th pthread, pthread id = " << pthread_self() << std::endl;
    return nullptr;
}

int main()
{
    pthread_t thd[5];

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&thd[i], nullptr, func, (void *)(long)i); // 显式地转换为 long 然后转换为 void*
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(thd[i], nullptr);
    }

    return 0;
}