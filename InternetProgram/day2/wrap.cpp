#include "wrap.h"
#include <cstdio>
#include <cerrno>

void perr_exit(const char *str)
{
    perror(str);
    exit(-1);
}

int Socket(int domain, int type, int protocol)
{
    int fd = socket(domain, type, protocol);
    if (fd == -1)
        perr_exit("socket failed: ");
    return fd;
}

int Bind(int sockfd, const sockaddr *addr, socklen_t addrlen)
{
    int ret = bind(sockfd, addr, addrlen);
    if (ret == -1)
        perr_exit("bind failed: ");
    return ret;
}

int Listen(int sockfd, int backlog)
{
    int ret = listen(sockfd, backlog);
    if (ret == -1)
        perr_exit("listen failed: ");
    return ret;
}

int Connect(int sockfd, const sockaddr *addr, socklen_t addrlen)
{
    int ret = connect(sockfd, addr, addrlen);
    if (ret == -1)
        perr_exit("connect failed: ");
    return ret;
}

int Accept(int sockfd, sockaddr *addr, socklen_t *addrlen)
{
    int ret = accept(sockfd, addr, addrlen);
again:
    if (ret < 0)
    {
        if ((errno == ECONNABORTED) || (errno == EINTR))
            goto again;
        else
            perr_exit("accept error");
    }
    return ret;
}

int Inet_pton(int af, const char *src, in_addr *dst)
{
    int ret = inet_pton(af, src, dst);
    if (ret == 0 || ret == -1)
        perr_exit("inet_pton failed: ");
    return ret;
}

/**
 * @brief Reads data from a file descriptor.
 *
 * @param fd The file descriptor to read from.
 * @param ptr A pointer to the buffer where the read data will be stored.
 * @param nbytes The number of bytes to read.
 * @return ssize_t The number of bytes read on success, -1 on failure.
 */
ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;
again:
    // read 只有读到0时才会结束
    if ((n = read(fd, ptr, nbytes)) == -1)
    {
        if (errno == EINTR)
            goto again;
        else
            return -1;
    }
    return n;
}

/**
 * Writes data to a file descriptor.
 *
 * @param fd The file descriptor to write to.
 * @param ptr A pointer to the data to be written.
 * @param nbytes The number of bytes to write.
 * @return On success, the number of bytes written is returned. On error, -1 is returned.
 */
ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = write(fd, ptr, nbytes)) == -1)
    {
        if (errno == EINTR)
            goto again;
        else
            return -1;
    }
    return n;
}

int Close(int fd)
{
    int ret = close(fd);
    if (ret == -1)
        perr_exit("close error");
    return ret;
}

/**
 * @brief Reads data from a file descriptor into a buffer.
 *
 * This function reads up to 'n' bytes from the file descriptor 'fd' into the buffer pointed to by 'vptr'.
 * It continues reading until 'n' bytes have been read or an error occurs.
 *
 * @param fd The file descriptor to read from.
 * @param vptr The pointer to the buffer where the data will be stored.
 * @param n The maximum number of bytes to read.
 * @return The total number of bytes read, or -1 if an error occurs.
 */
ssize_t Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;  // 还剩多少没有读
    ssize_t nread; // 实际读到的字节数
    char *ptr;

    ptr = (char *)vptr;
    nleft = n; // 未读取字节数

    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if (nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}

/**
 * Writes data to a file descriptor.
 *
 * @param fd The file descriptor to write to.
 * @param vptr A pointer to the data to be written.
 * @param n The number of bytes to write.
 * @return The number of bytes written on success, or -1 on failure.
 */
ssize_t Writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (char *)vptr;
    nleft = n;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

/**
 * @brief Reads a single character from a file descriptor.
 *
 * This function reads a single character from the file descriptor specified by `fd` and stores it in the memory location pointed to by `ptr`.
 *
 * @param fd The file descriptor to read from.
 * @param ptr A pointer to the memory location where the read character will be stored.
 * @return On success, returns 1. If the end of the file is reached, returns 0. If an error occurs, returns -1.
 */
ssize_t my_read(int fd, char *ptr)
{
    static int read_cnt;
    static char *read_ptr;
    static char read_buf[100];

    if (read_cnt <= 0)
    {
    again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
        {
            if (errno == EINTR)
                goto again;
            return -1;
        }
        else if (read_cnt == 0)
            return 0;
        read_ptr = read_buf;
    }
    read_cnt--;
    *ptr = *read_ptr++;
    return 1;
}

/**
 * @brief Reads a line from a file descriptor.
 *
 * This function reads a line from the specified file descriptor and stores it in the provided buffer.
 * The maximum number of characters to read is specified by the maxlen parameter.
 *
 * @param fd The file descriptor to read from.
 * @param vptr The buffer to store the read line.
 * @param maxlen The maximum number of characters to read.
 * @return The number of characters read, or -1 if an error occurred.
 */
ssize_t Readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;
    ptr = (char *)vptr;

    for (n = 1; n < maxlen; n++)
    {
        if ((rc = my_read(fd, &c)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break;
        }
        else if (rc == 0)
        {
            *ptr = 0;
            return n - 1;
        }
        else
            return -1;
    }
    *ptr = 0;
    return n;
}
