#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> splitCommand(const std::string &str)
{
    std::regex ws_re("\\s+"); // 匹配一个或多个空格
    std::vector<std::string> result{
        std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1), {}};
    return result;
}

int main()
{
    std::string s = "This is a command with several words";
    std::vector<std::string> words = splitCommand(s);
    for (const auto &word : words)
    {
        std::cout << word << std::endl;
    }
    return 0;
}