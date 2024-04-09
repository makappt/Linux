// 多进程高并发服务器
#include "wrap.h"
#include <signal.h>
#include <iostream>
#include <sys/wait.h>

using namespace std;

#define PORT 8888
#define IPADDR "127.0.0.1"

void wait_child(int signal)
{
    while (waitpid(0, nullptr, WNOHANG) > 0)
        ;
}

int main()
{
    pid_t pid;
    struct sockaddr_in server_addr, client_addr;
    int sfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr)); // 清零
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    Inet_pton(AF_INET, IPADDR, &server_addr.sin_addr);
    Bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    Listen(sfd, 128);
    socklen_t client_length = sizeof(client_addr);
    char buf[BUFSIZ];
    while (true)
    {
        int client_fd =
            Accept(sfd, (struct sockaddr *)&client_addr, &client_length);
        printf("client ip: %s, port: %d\n",
               inet_ntop(AF_INET, &client_addr.sin_addr, buf, sizeof(buf)),
               ntohs(client_addr.sin_port));

        pid = fork();
        if (pid == 0)
        {
            Close(sfd);
            while (1)
            {
                int ret = Read(client_fd, buf, sizeof(buf));
                if (ret == 0)
                {
                    printf("the other side has been closed.\n");
                    break;
                }
                Write(STDOUT_FILENO, buf, ret);

                for (int i = 0; i < ret; i++)
                    buf[i] = toupper(buf[i]);

                Write(client_fd, buf, ret);
            }
            Close(client_fd);
            return 0;
        }
        else if (pid > 0)
        {
            Close(client_fd);
        }
        else
            perr_exit("fork");
    }

    return 0;
}