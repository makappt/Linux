#include <poll.h>
#include <algorithm>
#include <iostream>
#include "wrap.h"

#define IPADDR "127.0.0.1"
#define PORT 6666

int main() {
    int clientfd, listenfd, maxi, n;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen;
    struct pollfd client[FD_SETSIZE];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, IPADDR, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(PORT);
    Bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    Listen(listenfd, 128);
    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for (int i = 1; i < FD_SETSIZE; i++) {
        client[i].fd = -1;
    }

    while (1) {
        int nready = poll(client, maxi + 1, -1);
        if (nready < 0) {
            perror("poll error");
            exit(1);
        }

        if (client[0].revents & POLLIN) {  // New connection
            int i;
            clientAddrLen = sizeof(clientAddr);
            clientfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
            for (i = 1; i < FD_SETSIZE; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = clientfd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                printf("Too many clients\n");
                close(clientfd);
                continue;
            }

            client[i].events = POLLIN;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }

        for (int i = 1; i <= maxi; i++) {  // Check all clients for data
            if (client[i].fd < 0)
                continue;
            if (client[i].revents & (POLLIN | POLLERR)) {
                n = read(client[i].fd, buf, BUFSIZ);
                if (n < 0) {
                    perror("read error");
                    close(client[i].fd);
                    client[i].fd = -1;
                } else if (n == 0) {
                    printf("Client closed connection\n");
                    close(client[i].fd);
                    client[i].fd = -1;
                } else {
                    write(client[i].fd, buf, n);   // Echo back
                    write(STDOUT_FILENO, buf, n);  // Echo back
                }
            }
        }
    }
    return 0;
}
