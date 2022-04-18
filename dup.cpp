#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    const char *ip="127.0.0.1";
    int port = 54321;

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    int ret=bind(sock, (struct sockaddr *)&address, sizeof(address));

    ret = listen(sock, 5);

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int connfd = accept(sock, (struct sockaddr *)&client, &client_len);
    if(connfd)
    {
        close(STDOUT_FILENO);
        dup(connfd);
        printf("abcd\n");
        close(connfd);
    }
    close(sock);
}