// 添加或删除环境变量
#include <iostream>
using namespace std;
extern char **environ;

int main()
{
    // 打印环境变量
    for (int i = 0; environ[i] != nullptr; i++)
    {
        cout << environ[i] << endl;
    }

    return 0;
}