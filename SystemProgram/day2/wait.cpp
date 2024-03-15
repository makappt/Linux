// 判断孤儿进程和僵尸进程
// 使用wait和waitpid回收子进程
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    int i = 0;
    pid_t pid, wpid;
    int status;
    // 循环创建多个子进程
    for (i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            cout << "fork failed !\n";
            exit(1);
        }
        if (pid == 0)
        {
            cout << "创建第 " << i << " 个子进程,pid = " << getpid() << " ppid = " << getppid() << endl;
            break;
        }
        if (pid > 0)
        {
            // 创建僵尸进程，子进程先于父进程结束，但是父进程并没有回收它，
            // 子进程的PCB残留在内核中,状态为z+
        }
    }

#if 0
    cout << "-------------------------------" << endl;
    if (i < 5)
    {
        // 创建孤儿进程，孤儿进程为父进程先于子进程结束，子进程被init进程回收
        sleep(i);
        cout << "第 " << i << " 个子进程,pid = " << getpid() << " ppid = " << getppid() << endl;
    }
#endif

    // 使用wait回收子进程
    // while (wait(nullptr) != -1)
    //     ;

    // 使用waitpid回收子进程
    if (pid > 0)
    {
        // 父进程等待所有子进程结束
        while (true)
        {
            pid = waitpid(-1, &status, WNOHANG);
            if (pid == -1)
            {
                // 所有子进程都已经结束
                break;
            }
            else if (pid == 0)
            {
                // 有子进程仍在运行
                cout << "Child is still running\n";
                sleep(1); // 等待一段时间再次检查
            }
            else
            {
                // 一个子进程已结束
                cout << "Child with PID " << pid << " exited\n";
            }
        }
        cout << "sleep 10s" << endl;
        sleep(10);
    }

    return 0;
}