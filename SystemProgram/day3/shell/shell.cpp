#include <iostream>
#include <regex>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <signal.h>

using namespace std;

std::vector<std::string> spliteCommand(const std::string &str)
{
    std::regex pattern("\\s+"); // 匹配一个或多个空格
    std::vector<std::string> result{
        std::sregex_token_iterator(str.begin(), str.end(), pattern, -1), {}};
    return result;
}

// 将vector转换为char*数组
char **vectorToChar(const vector<string> &vec)
{
    char **args = new char *[vec.size() + 1];
    for (int i = 0; i < vec.size(); i++)
    {
        args[i] = new char[vec[i].size() + 1];
        strcpy(args[i], vec[i].c_str());
    }
    args[vec.size()] = NULL;
    return args;
}

// void handler(int sig)
// {
//     wait(NULL);
// }

int main()
{
    string command;
    vector<string> comVec;
    while (true)
    {
        cout << "input your command: ";
        getline(cin, command);
        if (command == "\n")
            continue;
        comVec = spliteCommand(command);
        string grep = "|";
        int grepCount = count_if(comVec.begin(), comVec.end(), [](const string &str)
                                 { return str == "|"; });
        char **args = vectorToChar(comVec);
        if (grepCount == 0)
        {
            pid_t pid = fork();
            if (pid > 0)
            {
                sleep(1);
            }
            if (pid == 0)
            {
                execvp(args[0], args);
            }
        }
        else
        {
        }

        // struct sigaction act;
        // act.sa_handler = handler;
        // sigemptyset(&act.sa_mask);
        // act.sa_flags = 0;
        // sigaction(SIGCHLD, &act, NULL);

        cout << endl;
        delete[] args;
        comVec.clear();
    }

    return 0;
}