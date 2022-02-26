#include <sys/timerfd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

void print_elapsed_time(void);

int main(void)
{
  int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
  if (timerfd == -1)
  {
    handle_error("timerfd_create");
  }

  struct itimerspec new_value = {};
  new_value.it_value.tv_sec  = 10; // 10s
  new_value.it_value.tv_nsec = 0;

  new_value.it_interval.tv_sec  = 0; // one shot
  new_value.it_interval.tv_nsec = 0;

  if (timerfd_settime(timerfd, 0, &new_value, NULL) == -1)
  {
    handle_error("timerfd_settime");
  }

  print_elapsed_time();
  printf("timer started\n");


  uint64_t exp = 0;
  while (1)
  {
    int ret = read(timerfd, &exp, sizeof(uint64_t));

    if (ret == sizeof(uint64_t)) // 第一次定时到期
    {
      printf("ret: %d\n", ret);
      printf("expired times: %llu\n", exp);
      break;
    }

    // struct itimerspec curr;
    // if (timerfd_gettime(timerfd, &curr) == -1)
    // {
    //     handle_error("timerfd_gettime");
    // }
    // printf("remained time: %lds\n", curr.it_value.tv_sec);

    print_elapsed_time();
  }

  return 0;

}



void print_elapsed_time(void)
{
  static struct timeval start = {};
  static int first_call = 1;

  if (first_call == 1)
  {
    first_call = 0;
    if (gettimeofday(&start, NULL) == -1)
    {
      handle_error("gettimeofday");
    }
  }

  struct timeval current = {};
  if (gettimeofday(&current, NULL) == -1)
  {
    handle_error("gettimeofday");
  }

  static int old_secs = 0, old_usecs = 0;

  int secs  = current.tv_sec - start.tv_sec;
  int usecs = current.tv_usec - start.tv_usec;
  if (usecs < 0)
  {
    --secs;
    usecs += 1000000;
  }

  usecs = (usecs + 500)/1000; // 四舍五入

  if (secs != old_secs || usecs != old_usecs)
  {
    printf("%d.%03d\n", secs, usecs);
    old_secs = secs;
    old_usecs = usecs;
  }
}
