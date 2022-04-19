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
#define BUFFER_SIZE 1024
int main(int argc, char **argv)
{
    if(argc <=2)
    {
        return 1;	
    }
    const char *ip=argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock>=0);
    int recvbuf = 2000;
    int len=sizeof(recvbuf);
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,len);
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t *)&len);
    cout<<"recvbuf"<<recvbuf<<endl;

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);
    if(connfd<0)
    {
        cout << "error" << endl;
    }
    else 
    {
        char buffer[BUFFER_SIZE];

        memset(buffer, '\0', BUFFER_SIZE);
        while(recv(connfd, buffer,BUFFER_SIZE-1,0)){}
        cout << buffer << endl;

        close(connfd);
    }
    close(sock);
    return 0;
}