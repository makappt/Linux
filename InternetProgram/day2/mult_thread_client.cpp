#include "wrap.h"
#include <iostream>

#define IPSERVER "127.0.0.1"
#define PORT 8888

int main()
{
    int server_fd;
    struct sockaddr_in server_addr;

    bzero(&server_addr, sizeof(server_addr));
    server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    Inet_pton(AF_INET, IPSERVER, &server_addr.sin_addr);
    Connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, stdin) != NULL)
    {
        Write(server_fd, buf, strlen(buf));
        int n = Read(server_fd, buf, BUFSIZ);
        if (n == 0)
        {
            printf("the other side has been closed.\n");
            break;
        }
        else
            Write(STDOUT_FILENO, buf, n);
    }
    return 0;
}