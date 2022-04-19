#include "my_headers.h"
using namespace std;
int main(int argc,char * argv[])
{
    if(argc<=2)
    {
        return 1;
    }
    const char *ip=argv[1];
    int port=atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sockfd=socket(PF_INET,SOCK_STREAM,0);
    int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret!=-1);

    sleep(20);
    struct sockaddr_in client;
    socklen_t client_addrlength=sizeof(client);
    int connfd = accept(sockfd, (struct sockaddr *)&client, &client_addrlength);
    if(connfd)
    {
        char remote[INET_ADDRSTRLEN];
        close(connfd);
    }
    close(sockfd);

    return 0;
}