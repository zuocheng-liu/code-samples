#include <stdio.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <time.h>
#include <sys/timerfd.h>
 
static void epollAddEvent(int epollFd, int addFd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = addFd;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, addFd, &ev);
	return;
}
 
static int calcProcessTime(int *pStartMs)
{
	struct timespec endTime;
	clock_gettime(CLOCK_REALTIME, &endTime);
	if(pStartMs){
		return endTime.tv_sec * 1000 + endTime.tv_nsec / 1000000 - *pStartMs;
	}
	return endTime.tv_sec * 1000 + endTime.tv_nsec / 1000000;
}
 
 
void* thread_work(void* user)
{
	int epollFd = *(int*)user;
	struct epoll_event events[1] = {0};
	int cnt = 0;
	while(1)
	{
		int ms = 0;
		ms = calcProcessTime(NULL);
		int fireEvents = epoll_wait(epollFd, events, 1, -1);
		ms = calcProcessTime(&ms);
		if(fireEvents > 0){
			printf("time out: %d ms\n", ms);
			uint64_t exp;
			ssize_t size = read(events[0].data.fd, &exp, sizeof(uint64_t));
			if(size != sizeof(uint64_t)) {
				printf("read error!\n");
			}
		}
		else{
			printf("fireEvents = %d", fireEvents);
		}
	}
}
 
int timer_update(int timerFd, int ms)
{
	struct itimerspec its;
 
	its.it_interval.tv_sec = 2; // 2 seconds
	its.it_interval.tv_nsec = 0;
	its.it_value.tv_sec = ms / 1000;
	its.it_value.tv_nsec = (ms % 1000) * 1000 * 1000;
	if (timerfd_settime(timerFd, 0, &its, NULL) < 0)
		return -1;
	printf("timer update: %d\n", ms);
	return 0;
}
 
void main()
{
	/* create epoll */
	int epollFd = -1;
	epollFd = epoll_create(1);
 
	/* create timer */
	int timeFd = -1;
	timeFd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
	
	/* add timer fd to epoll monitor event */
	epollAddEvent(epollFd, timeFd, EPOLLIN);
 
	/* create thread to monitor */
	pthread_t threadId = -1;
	pthread_create(&threadId, NULL, &thread_work, (void*)&epollFd);
 
	timer_update(timeFd, 1000);
	while(1){
		usleep(1000000);
	}
 
	return;
}
