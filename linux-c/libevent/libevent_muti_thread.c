#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <event.h>

struct event_base *workerbase;
int fd[2];

static void 
thread_libevent_process(int fd, short which, void *arg)
{
    int ret;
    char buf[128];

    ret = read(fd, buf, 128);
    if (ret > 0) {
        buf[ret] = '\0';
        printf("thread receive message : %s\n", buf);
    }

    sleep(1);

    write(fd, buf, ret);
    return;
}

static void * worker_thread(void *arg) {
    int fd = *(int *)arg;
    struct event ev;
    event_set(&ev, fd, EV_READ | EV_PERSIST, thread_libevent_process, NULL);
    event_base_set(workerbase, &ev);
    event_add(&ev, 0);
    event_base_loop(workerbase, 0);
    return NULL;
}

static void main_event_process(int fd, short which, void *arg) {
    int ret;
    char buf[128];

    ret = read(fd, buf, 128);
    if (ret > 0) {
        buf[ret]='\0';
        printf("main thread receive echo message : %s\n", buf);
    }

    return;
}

static void timeout_cb(int fdd, short event, void *arg) {
    struct timeval tv;
    struct event *timeout = arg;

    write(fd[0], "Hello world!", sizeof("Hello world!") - 1);

    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    event_add(timeout, &tv);
}

int main (int argc, char *argv[]) {
    int ret;

    pthread_t tid;

    struct event ev;
    struct event evtimeout;
    struct timeval tv;

    struct event_base *base;

    base = event_init();
    if (base == NULL) {
        perror("event_init( base )");
        return 1;
    }

    workerbase = event_init();
    if (workerbase == NULL) {
        perror("event_init( workerbase )");
        return 1;
    }

    ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fd);
    if (ret == -1) {
        perror("socketpair() : ");
        return 1;
    }

    event_set(&ev, fd[0], EV_READ | EV_PERSIST, main_event_process, NULL);
    event_base_set(base, &ev);
    event_add(&ev, 0);

    evtimer_set(&evtimeout, timeout_cb, &evtimeout);
    event_base_set(base, &evtimeout);
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    event_add(&evtimeout, &tv);

    ret = pthread_create(&tid, NULL, worker_thread, (void *)&fd[1]);
    if (ret != 0) {
        perror("pthread_create()");
        return 1;
    }


    event_base_loop(base, 0);
    return 0;
}
