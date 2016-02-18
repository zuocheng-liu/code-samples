#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>

int main(){
   int listen_fd,accept_fd,flag;
   struct sockaddr_in my_addr,remote_addr;
   if((listen_fd = socket(AF_INET,SOCK_STREAM,0))==-1){
       perror("create socket error");
       exit(1);
    }
    if(setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,(char*)&flag,sizeof(flag))==-1){
       perror("setsockopt error");
    }
    int flags = fcntl(listen_fd,F_GETFL,0);
    fcntl(listen_fd,F_SETFL,flags|O_NONBLOCK);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(3389);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    if(bind(listen_fd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_in))==-1){
        perror("bind error");
        exit(1);
    }
    if(listen(listen_fd,1)==-1){
        perror("listen error");
        exit(1);
    }
    
    int pid = -1;
    int addr_len = sizeof(struct sockaddr_in);
    int max_process_num = 3;
    int child_id;
    int i;
    int child_process_status;
    for(i=0;i<max_process_num;i++){
         if(pid !=0){
             pid = fork();
         }else{
             child_id = i;
         }
    }
    if(pid == 0){
         int accept_handles = 0;
         struct epoll_event ev,events[20];
         int epfd = epoll_create(256);
         int ev_s = 0;
         ev.data.fd = listen_fd;
         ev.events = EPOLLIN|EPOLLET;
         epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,&ev);
         for(;;){
             ev_s = epoll_wait(epfd,events,20,500);
             int i=0;
             for(i=0;i<ev_s;i++){
                  if(events[i].data.fd == listen_fd){
                     int max_process_accept = 3;
                     if(accept_handles < max_process_accept){
                           accept_handles++;
                           int addr_len = sizeof(struct sockaddr_in);
                           accept_fd = accept(listen_fd,(struct sockaddr *)&remote_addr,&addr_len);
                           int flags = fcntl(accept_fd,F_GETFL,0);
                           fcntl(accept_fd,F_SETFL,flags|O_NONBLOCK);
                           ev.data.fd = accept_fd;
                           ev.events = EPOLLIN|EPOLLET;
                           epoll_ctl(epfd,EPOLL_CTL_ADD,accept_fd,&ev);
                           printf("Child:%d,ProcessID:%d,EPOLLIN,fd:%d,accept:%d\n",child_id,getpid(),listen_fd,accept_fd);
                      }
                    }
                   else if(events[i].events&EPOLLIN){
                       char in_buf[1024];
                       memset(in_buf,0,1024);
                       int recv_num = recv(events[i].data.fd,&in_buf,1024,0);
                       if(recv_num==0){
                           close(events[i].data.fd);
                           accept_handles--;
                           printf("Child:%d,ProcessID:%d,EPOLLIN,fd:%d,closed\n",child_id,getpid(),events[i].data.fd);
                       }
                       else{
                           printf("Child:%d,ProcessID:%d,EPOLLIN,fd:%d,recv:%s\n",child_id,getpid(),events[i].data.fd,in_buf);
                       }
                    }
                 }
             }
          }
          else{
              wait(&child_process_status);
          }
          return 0;
 }