// 循环创建子进程
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    // 循环创建5个子进程
    int i;
    for (i = 0; i < 5; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            continue;
        }
        else
        {
            cout << "这是第 " << i + 1 << " 个子进程\n";
            break;
        }
    }
    for (i = 0; i < 5; i++)
    {
        wait(nullptr);
    }
    return 0;
}
