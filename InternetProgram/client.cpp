/**
 * @brief This program demonstrates a simple TCP client that connects to a server and sends/receives messages.
 */
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
// 创建socket，连接到服务器，发送消息，接收服务器回复
int main()
{
    struct sockaddr_in serverAddr;
    // 创建socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "Socket creation failed" << std::endl;
        return -1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6666); // 服务器监听的端口
    // 将点分十进制的IP地址转换为用于网络通信的数值格式
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to the server." << std::endl;

    // 发送数据
    const char *message = "Hello from client";
    std::cout << "Message sent" << std::endl;

    // 接收服务器回复
    char buffer[BUFSIZ] = {0};
    while (1)
    {
        // send(sock, message, strlen(message), 0);
        write(sock, message, strlen(message));
        sleep(1);
        // 接收服务器回复
        int bytesReceived = read(sock, buffer, sizeof(buffer));
        if (bytesReceived == -1)
        {
            std::cout << "Read failed" << std::endl;
            return -1;
        }
        // 打印服务器回复
        std::cout << "Received from server: " << buffer << std::endl;
    }

    // 关闭socket
    close(sock);

    return 0;
}
