// 使用守护进程实现每隔两秒记录系统实现到mytime.txt文件中
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <ctime>
#include <chrono>
#include <sys/time.h>

void sigHandler(int signal)
{
    // 获得当前系统时间
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    int fd = open("/home/zjz/mytime.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    if (fd == -1)
    {
        perror("mytime.txt open failed");
        exit(-1);
    }
    dprintf(fd, "%s", std::ctime(&now_c));

    close(fd);
}

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(-1);
    }
    if (pid > 0)
        return 0;
    pid = setsid();
    if (pid == -1)
    {
        perror("setsid failed");
        exit(-1);
    }

    umask(0002); // 设置文件模式创建掩码

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        perror("/dev/null open failed");
        exit(-1);
    }

    close(STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    int ret = chdir("/home/zjz");
    if (ret != 0) // 正确的错误检查
    {
        perror("chdir failed");
        exit(-1);
    }

    struct itimerval it;
    it.it_value.tv_sec = 2; // 初始计时器
    it.it_value.tv_usec = 0;
    it.it_interval.tv_sec = 2; // 重复间隔
    it.it_interval.tv_usec = 0;

    signal(SIGALRM, sigHandler);       // 注册信号处理函数
    setitimer(ITIMER_REAL, &it, NULL); // 激活定时器

    while (1)
        pause(); // 更
    return 0;
}