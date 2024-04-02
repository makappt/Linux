#include "wrap.h"
#include <cstdio>
#include <cerrno>

int Socket(int domain, int type, int protocol)
{
    int fd = socket(domain, type, protocol);
    if (fd == -1)
    {
        perror("socket failed: ");
        exit(-1);
    }
    return fd;
}

int Bind(int sockfd, const sockaddr *addr, socklen_t addrlen)
{
    int ret = bind(sockfd, addr, addrlen);
    if (ret == -1)
    {
        perror("bind failed: ");
        exit(-1);
    }
    return ret;
}

int Listen(int sockfd, int backlog)
{
    int ret = listen(sockfd, backlog);
    if (ret == -1)
    {
        perror("listen failed: ");
        exit(-1);
    }
    return ret;
}

int Connect(int sockfd, const sockaddr *addr, socklen_t addrlen)
{
    int ret = connect(sockfd, addr, addrlen);
    if (ret == -1)
    {
        perror("connect failed: ");
        exit(-1);
    }
    return ret;
}

int Accept(int sockfd, sockaddr *addr, socklen_t *addrlen)
{
    int ret = accept(sockfd, addr, addrlen);
    if (ret == -1)
    {
        perror("accept failed: ");
        exit(-1);
    }
    return ret;
}

int Inet_pton(int af, const char *src, in_addr *dst)
{
    int ret = inet_pton(af, src, dst);
    if (ret == 0 || ret == -1)
    {
        perror("inet_pton failed: ");
        exit(-1);
    }
    return ret;
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;
again:
    if ((n = read(fd, ptr, nbytes)) == -1)
    {
        if (errno == EINTR)
            goto again;
        else
            return -1;
    }
    return n;
}

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
        perror("close error");
    return ret;
}

ssize_t Readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = (char *)vptr;
    nleft = n;

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
