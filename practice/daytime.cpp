#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <assert.h>
using namespace std;
int main(int argc, char *argv[]) 
{
    char *host = argv[1];
    struct hostent *hostinfo=gethostbyname(host);
    struct servent *servinfo = getservbyname("daytime", "tcp");
    printf("daytime port is %d\n", ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port=servinfo->s_port;
    address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int res = connect(sockfd, (struct sockaddr *)&address, sizeof(address));

    char buffer[128];
    res=read(sockfd,buffer,sizeof(buffer));
    buffer[res] = '\0';

    puts(buffer);
    close(sockfd);
    return 0;
}