#include "wrap.h"

#include <cstdio> // Include the necessary header file

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
