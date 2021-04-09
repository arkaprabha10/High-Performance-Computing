#include<iostream>

using namespace std;

int main()
{   
    int a=0,b=1;
    int n=1024;
    double h,ans=0;
    h=1.0/n;
    for(int i=0;i<n;i++)
    {
        float temp = (i+0.5)*h;
        ans+=4/(1+temp*temp);
    }
    // cout<<h<<" ";
    cout<<ans*h;
    return 0;
}