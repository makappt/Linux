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
    // 虽然 /dev/zero 可以用于有血缘关系的进程间通信，但对于需要跨不同进程共享数据的场景，
    // 特别是当这些进程没有血缘关系时，命名的共享内存提供了一种更灵活且广泛适用的解决方案。
    // dev/zero不能用于非血缘间的进程间的通信，可以使用POSIX的shm_open函数实现
    // 因为有血缘的进程通过fork() 会复制父进程的地址空间到子进程，包括内存映射区域。如果这个映射是以 MAP_SHARED 标志创建的，那么父子进程间就可以通过这个共享内存区域进行通信。
    // 但是没有血缘关系的进程是没有彼此的地址的，所有不能进行通信
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
