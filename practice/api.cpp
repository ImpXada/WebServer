#include "my_headers.h"
using namespace std;
static bool stop = false;
static void handle_term(int sig)
{
    stop = true;
}
int main(int argc,char * argv[])
{

    signal(SIGTERM, handle_term);

    if (argc <= 3)
    {
        cout << "usage: " << basename(argv[0]) << "ip_address port_number backlog" << endl;
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);    

    /**
     * struct sockaddr_in
     * {
     *      sa_family_t sin_family;
     *      u_int16_t sin_port;
     *      struct in_addr sin_addr;
     * }
     * struct in_addr
     * {
     *      u_int32_t s_addr;
     * }
     */

    struct sockaddr_in address;
    bzero(&address, sizeof(address));//前n个字节清零
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);//host to network short

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));

    ret = listen(sock, backlog);
    while (!stop)
    {
        sleep(1);
    }
    close(sock);
    return 0;
}
