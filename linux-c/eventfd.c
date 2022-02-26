#include <stdio.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <pthread.h>
#include <stdlib.h>

int _epollfd, _eventfd;

int init()
{
    _epollfd = epoll_create1(0);
    if (_epollfd == -1) abort();
    _eventfd = eventfd(0, EFD_NONBLOCK);
    struct epoll_event evnt = {0};
    evnt.data.fd = _eventfd;
    evnt.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _eventfd, &evnt) == -1)
        abort();
}

void *subprocess(void *arg)
{
    static const int EVENTS = 20;
    struct epoll_event evnts[EVENTS];
    while (1) {
        int count = epoll_wait(_epollfd, evnts, EVENTS, -1);
        printf("%d\n", count);
        if (count == -1)
        {
            if (errno != EINTR)
            {
                perror("epoll_wait");
                return NULL;
            }
        }

        int i;
        for (i = 0; i < count; ++i)
        {
            struct epoll_event *e = evnts + i;
            if (e->data.fd == _eventfd)
            {
                eventfd_t val;
                eventfd_read(_eventfd, &val);
                printf("DING: %lld\n", (long long)val);
                return NULL;
            }
        }
    }
}

int main() {
    pthread_t th;
    init();
    if (pthread_create(&th, NULL, subprocess, NULL) != 0) abort();
    sleep(2);
    printf("eventfd_write\n");
    eventfd_write(_eventfd, 1);
    sleep(2);
}
