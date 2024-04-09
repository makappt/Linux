#ifndef WRAP_H
#define WRAP_H

#include <arpa/inet.h>
#include <error.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
int Inet_pton(int af, const char* src, in_addr* dst);
ssize_t Read(int fd, void* ptr, size_t nbytes);
ssize_t Write(int fd, const void* ptr, size_t nbytes);
int Close(int fd);
ssize_t Readn(int fd, void* vptr, size_t n);
ssize_t Writen(int fd, const void* vptr, size_t n);
ssize_t my_read(int fd, char* ptr);
ssize_t Readline(int fd, void* vptr, size_t maxlen);

#endif