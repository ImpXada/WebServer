#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <iostream>
using namespace std;   
const int MAX_EVENT_NUMBER = 1024;
const int BUFFER_SIZE = 1024;

struct fds
{
    int epollfd;
    int sockfd;
};

int setnonblocking(int fd)
{
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd, int fd , bool oneshot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;
    if(oneshot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void reset_oneshot(int epollfd,int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void *worker(void * arg)
{
    int sockfd=((fds*)arg)->sockfd;
    int epollfd = ((fds *)arg)->epollfd;
    cout << "start new worker to receive data on fd" << sockfd << endl;
    char buf[BUFFER_SIZE];
    memset(buf,'\0',BUFFER_SIZE);
    while(1)
    {
        int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
        if(ret==0)
        {
            close(sockfd);
            cout<<"foreiner closed the connection"<<endl;
            break;
        }
        else if (ret <0)
        {
            if (errno==EAGAIN)
            {
                reset_oneshot(epollfd, sockfd);
                cout << "read latere" << endl;
                break;
            }
        }
        else 
        {
            cout << "get connect " << buf << endl;
            sleep(5);
        }
    }
    cout<<"end worker"<<endl;
}

int main(int argc, char *argv[])
{
    if(argc <2)
    {
        return 1;
    }
    const char *ip=argv[1];
    int port = atoi(argv[2]);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    int listenfd= socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, false);

    while (true) 
    {
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if(ret<0)
        {
            cout << "epoll failure" << endl;
            break;
        }
        for (int i = 0; i < ret;i++)
        {
            int sockfd = events[i].data.fd;
            if(sockfd==listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                addfd(epollfd, connfd, true);

            }
            else if (events[i].events&EPOLLIN)
            {
                pthread_t thread;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd = epollfd;
                fds_for_new_worker.sockfd = sockfd;

                pthread_create(&thread, NULL, worker, (void *)&fds_for_new_worker);
            }
            else 
            {
                cout << "something else happened" << endl;
            }
        }
    }
    close(listenfd);
    return 0;
}