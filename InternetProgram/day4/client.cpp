#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "wrap.h"

int main() {
    struct sockaddr_in serverAddr;
    int sock = Socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6666);
    Inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    Connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    std::cout << "Connected to the server." << std::endl;

    const char* message = "Hello from client";
    std::cout << "Message sent" << std::endl;
    while (true) {
        struct sockaddr_in serverAdd;
        sleep(1);
        write(sock, message, strlen(message));
        std::cout << "Message sent" << std::endl;
    }
}