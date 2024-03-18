// 创建匿名映射区
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>

using namespace std;

int main()
{
    // 创建匿名映射区 只有在Linux下才有MAP_ANONYMOUS这个宏，在类unix系统下，需要使用int fd = open("/dev/zero", O_RDWR);
    // 创建一个文件描述符才行，这个文件比较特殊，是个伪文件，不占用口空间
    void *p = mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED)
    {
        cerr << "mmap failed: " << strerror(errno) << endl;
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        cerr << "failed to fork: " << strerror(errno) << endl;
        exit(1);
    }

    if (pid > 0)
    { // 父进程
        const char *str = "nihao,jianzhe";
        memcpy(p, str, strlen(str) + 1);
        wait(nullptr); // 等待子进程结束
    }
    else
    {             // 子进程
        sleep(1); // 等待父进程写入数据
        cout << "Child reads: " << (char *)p << endl;
    }

    // 解除映射
    munmap(p, 1024);

    return 0;
}
