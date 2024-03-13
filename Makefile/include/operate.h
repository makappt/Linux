#ifndef OPERATE_H
#define OPERATE_H

#pragma once
#include <iostream>
using namespace std;
class operate
{
public:
    operate(int n) : num(n) {}
    void add(int a);
    void sub(int a);
    void mul(int a);
    void div(int a);
    ~operate() {}
    int num;
};

#endif