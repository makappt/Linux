// 使用mmap实现无血缘关系的进程间的通信
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <cerrno>
#include <sstream>
#include <cstring>

using namespace std;

int main()
{
    const char *name = "/my_share_memory";
    int size = 4096;

    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        cout << "open failed : " << strerror(errno) << endl;
        exit(-1);
    }

    if (ftruncate(fd, size) == -1)
    {
        cout << "ftuncate failed!" << strerror(errno) << endl;
        exit(-1);
    }

    void *p = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED)
    {
        cout << "mmap failed : " << strerror(errno) << endl;
        exit(-1);
    }
    int i = 0;
    while (1)
    {
        sleep(1);
        std::stringstream ss;
        ss << "nihao, " << i++;
        string str = ss.str();
        cout << "正在写入数据 " << str << endl;
        memcpy(p, str.c_str(), str.length() + 1);
    }

    munmap(p, size);

    return 0;
}