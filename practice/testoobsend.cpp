#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#define BUFFER_SIZE 512
int main(int argc,char *argv[])
{
    if(argc<=2){
        return 1;
    }
    const char *ip=argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port=htons(port);


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    int sendbuf = 50;
    int len=sizeof(sendbuf);
    setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,&sendbuf,len);
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t *)&len);
    cout << "buffer size: " << sendbuf << endl;
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) ==-1)
    {
        cout << "failed" << endl;
    }
    else
    {
        char buffer[BUFFER_SIZE];
        memset(buffer,'a',BUFFER_SIZE);
        send(sockfd, buffer, BUFFER_SIZE, 0);
    }
    close(sockfd);
    return 0;
}
