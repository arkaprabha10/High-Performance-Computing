#include<iostream>

using namespace std;

void func1()
{
    for(int i=0;i<1;i++);
    return;
}
void func2()
{
    for(int i=0;i<2;i++);
    return;
}

int main()
{
    for(int i=0;i<1000;i++)
    {
        func1();
        func2();

    }
    return 0;
}