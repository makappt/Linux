#include <iostream>
#include "operate.h"
using namespace std;

int main()
{
    operate opt(20);
    opt.add(10);
    opt.sub(20);
    opt.mul(10);
    opt.div(20);
    return 0;
}