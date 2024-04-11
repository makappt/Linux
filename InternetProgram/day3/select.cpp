/* select实现多路IO转接 */
#include <algorithm>
#include <iostream>
#include "wrap.h"

#define IPSERVER "127.0.0.1"
#define PORT 6666

int main() {
    /* Create a client[] array to make up for the problem of selecting looping 1024 times each time */
    int clientfd, client[FD_SETSIZE], listenfd, maxi, maxfd;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen;
    fd_set rset, allset;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, IPSERVER, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(PORT);
    Bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    Listen(listenfd, 128);

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    maxi = -1;

    for (int i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    int count;
    clientAddrLen = sizeof(clientAddr);
    while (true) {
        rset = allset;
        count = select(maxfd + 1, &rset, nullptr, nullptr, nullptr);
        if (FD_ISSET(listenfd, &rset)) {
            int i;
            clientfd = Accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = clientfd;
                    break;
                }
            }
            if (maxi == FD_SETSIZE) {
                perr_exit("文件描述符已满");
                continue;
            }
            maxi = std::max(i, maxi);
            maxfd = std::max(clientfd, maxfd);

            FD_SET(clientfd, &allset);
            if (count == 1)
                continue;
        }
        for (int i = 0; i <= maxi; i++) {
            if (FD_ISSET(client[i], &rset)) {
                int n = Read(client[i], buf, BUFSIZ);
                if (n == 0) {
                    Close(client[i]);
                    FD_CLR(client[i], &allset);
                    client[i] = -1;
                }
                Write(STDOUT_FILENO, buf, n);
                for (int j = 0; j < n; ++j) {
                    buf[j] = toupper(buf[j]);
                }
                Write(client[i], buf, n);
            }
        }
    }
    return 0;
}