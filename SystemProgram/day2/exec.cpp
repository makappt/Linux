#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
using namespace std;

extern char** environ;

int main()
{
    
    //将ps aux 的输出结果通过dup2重定向输出到result.txt文件中
    int file = open("result.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(file < 0)
    {
        cout << "open failed!" << endl;
        exit(0);
    }
    
    int flag = dup2(file,STDOUT_FILENO);
    if(flag < 0)
    {
        cout << "dup2 failed!" << endl;
    }
    close(file);

    //使用execlp查询当前系统的进程信息
    int ret = execlp("ps","ps","aux",nullptr);
    if(ret == -1)
    {
        cout << "execlp failed!" << endl;
        exit(0);
    }

    return 0;
}
