#include <sys/timerfd.h>
#include <iostream>
#include <time.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
using namespace std;

const int MAXNUM = 20;

int main(int argc, char *argv[]) {
    struct itimerspec new_value;
    struct timespec now;
    uint64_t exp;
    ssize_t s;

    int ret = clock_gettime(CLOCK_REALTIME, &now);//获取时钟时间
    assert(ret != -1);


    new_value.it_value.tv_sec = 1; //第一次到期的时间
    new_value.it_value.tv_nsec = now.tv_nsec; 

    new_value.it_interval.tv_sec = 1;      //之后每次到期的时间间隔
    new_value.it_interval.tv_nsec = 0;

    int timefd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK); // 构建了一个定时器
    assert(timefd != -1);

    ret = timerfd_settime(timefd, 0, &new_value, NULL);//启动定时器
    assert(ret != -1);

    cout << "timer started" << endl; // 定时器开启啦！


    int epollfd = epoll_create(1);  //创建epoll实例对象

    struct epoll_event ev;
    struct epoll_event events[MAXNUM];
    ev.data.fd = timefd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, timefd, &ev); //添加到epoll事件集合

    for (; ;) 
    {
        //cout << "epoll wait" << endl;
        int num = epoll_wait(epollfd, events, MAXNUM, 0);
        assert(num >= 0);

        for (int i = 0; i < num; i++) 
        {
            if (events[i].events & EPOLLIN) 
            {
                //....处理其他事件
                if (events[i].data.fd == timefd) 
                {
                    s = read(events[i].data.fd, &exp, sizeof(uint64_t)); //需要读出uint64_t大小, 不然会发生错误
                    assert(s == sizeof(uint64_t));
                    cout << "here is timer" << endl;
                }
            }
        }
    }

    close(timefd);
    close(epollfd);

    return 0;
}

