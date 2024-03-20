#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#if 0
void process_file_segment(int fd_src, int fd_dst, off_t start, size_t length)
{
    char buffer[4096];
    size_t bytes_read, total_read = 0;
    while (total_read < length)
    {
        size_t bytes_to_read = sizeof(buffer);

        if (length - total_read < sizeof(buffer))
        {
            bytes_to_read = length - total_read;
        }
        bytes_read = pread(fd_src, buffer, bytes_to_read, start + total_read);
        if (bytes_read <= 0)
        {
            perror("pread failed\n");
            break;
        }
        ssize_t bytes_written = pwrite(fd_dst, buffer, bytes_read, start + total_read);
        if (bytes_written <= 0)
        {
            perror("pwrite failed\n");
            break;
        }
        total_read += bytes_written;
    }
}
#endif
// 使用mmap映射区进行偏移
void process_file_segment(int fd_src, int fd_dsc, size_t offset, size_t length)
{
    void *src = mmap(nullptr, length, PROT_READ, MAP_SHARED, fd_src, 0);
    if (src == MAP_FAILED)
    {
        perror("src mmap failed!");
        exit(-1);
    }
    void *dsc = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dsc, 0);
    if (dsc == MAP_FAILED)
    {
        perror("dsc mmap failed!");
        exit(-1);
    }
    // 指针偏移，按字节偏移
    memcpy((char *)dsc + offset, (char *)src + offset, length);
    munmap(dsc, length);
    munmap(src, length);
}
int main()
{
    int fd_src = open("src.txt", O_RDONLY);
    int fd_dst = open("copy.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);

    struct stat sb;
    fstat(fd_src, &sb);
    off_t filesize = sb.st_size;
    int n_processes = 4;
    off_t block_size = filesize / n_processes;

    int ret = ftruncate(fd_dst, filesize);
    if (ret == -1)
    {
        perror("ftruncate failed");
        exit(-1);
    }

    for (int i = 0; i < n_processes; ++i)
    {
        if (fork() == 0)
        {
            off_t start = i * block_size;
            size_t length = (i == n_processes - 1) ? (filesize - start) : block_size;
            process_file_segment(fd_src, fd_dst, start, length);
            exit(0);
        }
    }

    while (wait(NULL) > 0)
        ;

    close(fd_src);
    close(fd_dst);

    return 0;
}
