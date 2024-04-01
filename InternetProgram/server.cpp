/**
 * @file server.cpp
 * @brief A simple server program that listens for client connections and echoes back received messages.
 */
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring> // 为了使用memset
// 接收到的消息转换为大写，然后发送回客户端
int main()
{
    struct sockaddr_in server_addr, client_addr;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // 创建socket文件描述符

    if (server_fd == 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // 设置服务器地址结构体
    memset(&server_addr, 0, sizeof(server_addr));    // 清零
    server_addr.sin_family = AF_INET;                // IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 任何地址
    server_addr.sin_port = htons(6666);              // 端口6666

    // 绑定socket到服务器地址
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // 监听连接
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port 6666" << std::endl;

    socklen_t client_len = sizeof(client_addr);
    // 接受客户端连接
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    char buffer[BUFSIZ]; // BUFSIZ来自<unistd.h>，通常足够大
    int bytes_read;

    while ((bytes_read = read(client_fd, buffer, BUFSIZ)) > 0)
    {                                                             // 读取数据
        buffer[bytes_read] = '\0';                                // 确保字符串结束
        std::cout << "Received message: " << buffer << std::endl; // 打印接收到的消息

        // 将接收到的消息转换为大写
        for (int i = 0; i < bytes_read; ++i)
        {
            buffer[i] = toupper(buffer[i]);
        }

        // 将转换后的消息发送回客户端
        // send(client_fd, buffer, bytes_read, 0);
        write(client_fd, buffer, bytes_read);
    }

    // 关闭连接
    close(client_fd);
    close(server_fd);

    return 0;
}
