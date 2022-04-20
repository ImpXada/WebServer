#ifndef LIST_TIMER_H
#define LIST_TIMER_H
#include <time.h>

#include <netinet/in.h>

#define BUFFER_SIZE 64

class util_timer;

struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer *timer;
};

class util_timer
{
public:
    util_timer() : prev(nullptr), next(nullptr) {}
    // private:
    time_t expire;
    void (*cb_func)(client_data *);
    client_data *user_data;
    util_timer *prev;
    util_timer *next;
};

class sort_timer_lst
{
public:
    sort_timer_lst() : head(NULL), tail(NULL) {}
    ~sort_timer_lst()
    {
        util_timer *tmp = head;
        while (tmp)
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }

private:
private:
    util_timer *head;
    util_timer *tail;
}
#endif