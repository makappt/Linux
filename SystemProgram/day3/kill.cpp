#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstring>
int main()
{
    int ret = kill(getpid(),SIGKILL);
    if(ret == -1)
    {
        std::cout << "kill failed : " << std::strerror(errno) << std::endl;
        exit(-1);
    }
    return 0;

}
