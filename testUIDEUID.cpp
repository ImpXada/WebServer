#include <unistd.h>
#include <iostream>
#include <stdio.h>
using namespace std;
int main()
{
    uid_t uid=getuid();
    uid_t euid=geteuid();
    cout<<"uid: "<<uid<<" euid: "<<euid<<endl;
}