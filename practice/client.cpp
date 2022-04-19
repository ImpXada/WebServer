#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
int main()
{
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(54321);
    
    int sockfd=socket(AF_INET, SOCK_STREAM, 0);

    if(!connect(sockfd, (struct sockaddr *)&address,sizeof(address)))
    {
        return 1;
    }
    else 
    {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = recv(sockfd, buf, 1024 - 1, 0);
        cout << buf << endl;
    }
    close(sockfd);
}