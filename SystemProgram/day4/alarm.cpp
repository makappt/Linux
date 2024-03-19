#include <iostream>
#include <unistd.h>
int main()
{
    alarm(1);//闹钟时间到达时终止程序 发生SIGALARM信号
    int i = 0;
    while(1)
    {
        std::cout << i++ << std::endl;
    }
    
}
