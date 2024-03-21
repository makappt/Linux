// 使用setitimer 代替alarm函数，统计计算机一秒内的读数
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    struct itimerval new_value, old_value;
    // it_value是第一个信号参数的时间，使用毫秒和秒精确控制
    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_usec = 0;
    // it_interval是后面信号发出的间隔时间，如果不手动停止setitimer函数，就会一直发射信号
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_usec = 0;

    int i = 0;
    // old_value为传出参数，用于保存之前的配置，如果想恢复之前的数据，可以使用old_value来恢复
    setitimer(ITIMER_REAL, &new_value, &old_value); // 如果不保存之前的配置，old_value处可以使用nullptr代替
    while (1)
    {
        cout << i++ << endl;
    }

    return 0;
}