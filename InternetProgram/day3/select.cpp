#include "wrap.h"
#include <iostream>

using namespace std;

int main()
{
    int maxi, i, j, maxfd, nready, size;
    int listenfd, client[FD_SETSIZE], clientfd, sockfd;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];
    struct sockaddr_in clientAddr, serverAddr;
    socklen_t clientAddr_len;
    fd_set rset, allset;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // 实现端口复用

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6666);
    Bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    Listen(listenfd, 128);

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    while (true)
    {
        rset = allset;
        nready = select(maxfd + 1, &rset, nullptr, nullptr, nullptr); // rset是传入传出参数
        if (nready < 0)
            perr_exit("select error");
        if (FD_ISSET(listenfd, &rset)) /* 使用监听描述符监听是否有新的客户端链接请求 */
        {
            clientAddr_len = sizeof(clientAddr);
            clientfd = Accept(listenfd, (struct sockaddr *)&clientAddr, &clientAddr_len);
            printf("received from %s at PORT %d\n",
                   inet_ntop(AF_INET, &clientAddr.sin_addr, str, sizeof(str)),
                   ntohs(clientAddr.sin_port));
            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (client[i] < 0)
                {
                    client[i] = clientfd;
                    break;
                }
            }

            if (i == FD_SETSIZE)
            { /* 达到select能监控的文件个数上限 1024 */
                fputs("too many clients\n", stderr);
                exit(1);
            }
            FD_SET(clientfd, &allset); // 添加新连接到allset中
            if (clientfd > maxfd)
            {
                maxfd = clientfd;
            }
            if (i > maxi)
            {
                maxi = i;
            }
            if (--nready == 0)
            {
                continue;
            }
        }
        for (i = 0; i <= maxi; i++)
        {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset))
            {
                size = Read(sockfd, buf, sizeof(buf));
                if (size == 0)
                {
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else
                {
                    Write(STDOUT_FILENO, buf, size);
                    for (j = 0; j < size; j++)
                        buf[j] = toupper(buf[j]);
                    Write(sockfd, buf, size);
                }
                if (--nready == 0)
                    break;
            }
        }
    }
    Close(listenfd);
    return 0;
}