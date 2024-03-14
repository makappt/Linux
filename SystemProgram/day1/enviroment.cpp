// 增删查环境变量
#include <iostream>
#include <stdlib.h>

using namespace std;

extern char **environ;

int main()
{
    // 打印环境变量
    // for (int i = 0; environ[i] != nullptr; i++)
    // {
    //     cout << environ[i] << endl;
    // }
    // 查询环境变量
    char *envValue = getenv("ABCD");
    if (envValue == nullptr)
    {
        cout << "the env is not existed! \n";
    }
    else
    {
        cout << "ABCD = " << envValue << '\n';
    }
    // 添加环境变量
    int ret = setenv("ABCD", "jianzhe", 1);
    if (ret == 0)
    {
        cout << "setenv is successful!\n";
    }
    else
    {
        cout << "setenv error\n";
    }

    envValue = getenv("ABCD");
    if (envValue == nullptr)
    {
        cout << "the env is not existed! \n";
    }
    else
    {
        cout << "ABCD = " << envValue << '\n';
    }

    ret = unsetenv("ABCD");
    if (ret == 0)
    {
        cout << "unsetenv is successful\n";
    }
    else
    {
        cout << "unsetenv is failed!\n";
    }

    envValue = getenv("ABCD");
    if (envValue == nullptr)
    {
        cout << "the env is not existed! \n";
    }
    else
    {
        cout << "ABCD = " << envValue << '\n';
    }

    return 0;
}