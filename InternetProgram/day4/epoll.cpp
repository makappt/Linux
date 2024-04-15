#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "wrap.h"

int main() {
    int listenfd, clientfd;
    socklen_t clientAddrLen;
    struct sockaddr_in serverAddr, clientAddr;
    char buf[BUFSIZ];
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6666);
    Inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    Bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    Listen(listenfd, 128);
    /* 创建红黑树，得到句柄 */
    int epfd = epoll_create(128);
    /* temp用于设置单个fd的属性 */
    struct epoll_event temp, client[FD_SETSIZE];
    temp.events = EPOLLIN;
    temp.data.fd = listenfd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &temp);
    int ret, n;
    clientAddrLen = sizeof(clientAddr);
    while (true) {
        ret = epoll_wait(epfd, client, 128, -1);
        for (int i = 0; i < ret; ++i) {
            if (listenfd == client[i].data.fd) {
                clientfd = Accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
                client[i].data.fd = clientfd;
                temp.events = EPOLLIN;
                temp.data.fd = clientfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &temp);
            } else {
                n = Read(client[i].data.fd, buf, sizeof(buf));
                if (n == 0) {
                    Close(client[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client[i].data.fd, &temp);
                } else {
                    Write(STDOUT_FILENO, buf, n);
                }
            }
        }
    }

    return 0;
}