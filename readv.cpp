#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

static const char *status_line[2] = {"200 OK", "500 Internal Server Error"};
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        return 1;
    }
    const char *ip=argv[1];
    int port=atoi(argv[2]);
    const char * file_name=argv[3];

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret=bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    res = listen(sock, 5);
    
}