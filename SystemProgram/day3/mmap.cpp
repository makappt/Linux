// PROT_READ PROT_WRITE PROT_READ|PROT_WRITE 映射区读写权限
// MAP_PRIVATE MAP_SHARED 是否修改磁盘文件信息
// MAP_FAILED 失败返回值

#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <cerrno>
#include <cstring>
using namespace std;
int main()
{
    // 创建打开文件
    int fd = open("mymmap.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1)
    {
        std::cerr << "Failed to open file: " << strerror(errno) << std::endl;
        return 1;
    }
    // lseek ftruncate  获取文件大小，扩容
    int len = ftruncate(fd, 4); // 返回值不是文件的大小!!!,ftruncate的返回值是0或-1
    if (len == -1)
    {
        std::cerr << "ftruncate failed: " << std::strerror(errno) << '\n';
        exit(0);
    }
    // 创建映射区
    void *p = mmap(nullptr, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED)
    {
        std::cerr << "mmap failed: " << std::strerror(errno) << '\n';
        exit(0);
    }
    strcpy((char *)p, "adc\n");
    // 关闭映射区
    munmap(p, 4); // 4为映射区的大小
    close(fd);
    return 0;
}