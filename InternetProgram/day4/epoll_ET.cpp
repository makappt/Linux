// epoll ET
#include <fcntl.h>
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

    int epfd = epoll_create(128);
    struct epoll_event temp, client[FD_SETSIZE];
    temp.events = EPOLLIN | EPOLLET;  // 设置ET模式_ET
    // temp.events = EPOLLIN;
    temp.data.fd = listenfd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &temp);
    int ret, n;
    clientAddrLen = sizeof(clientAddr);
    while (true) {
        ret = epoll_wait(epfd, client, 128, -1);
        for (int i = 0; i < ret; ++i) {
            if (listenfd == client[i].data.fd) {
                clientfd = Accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
                //     设置非阻塞
                int flags = fcntl(clientfd, F_GETFL, -1);
                flags |= O_NONBLOCK;
                fcntl(clientfd, F_SETFL, flags);
                client[i].data.fd = clientfd;
                temp.events = EPOLLIN;
                temp.data.fd = clientfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &temp);
            } else {
                while (true) {
                    n = read(client[i].data.fd, buf, sizeof(buf));
                    if (n > 0) {
                        write(STDOUT_FILENO, buf, n);
                    } else if (n == -1 && errno == EAGAIN) {
                        break;  // 没有更多数据
                    } else if (n == 0) {
                        // 客户端关闭了连接
                        close(client[i].data.fd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, client[i].data.fd, NULL);
                        break;
                    } else {
                        // 处理其他错误
                        perror("Read error");
                        close(client[i].data.fd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, client[i].data.fd, NULL);
                        break;
                    }
                }
            }
        }
    }

    return 0;
}