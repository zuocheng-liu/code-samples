#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <memory>
#include <thread>
#include <exception>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAXBUF 1024 
#define MAXEPOLLSIZE 10000


class Reactor {
public:
    enum EventType {
        EVENT_TYPE_NET_IO = 0,
        EVENT_TYPE_IO = 1,
        EVENT_TYPE_TIMER = 2
    };

    void registerEvent(std::shared_ptr<ReactorEvent> event) {
    }

    void loop() {
    }

private:
    std::vector<std::shared_ptr<ReactorEvent> > events;
    Reactor *impl = nullptr;
};

class EpollReactor {
public:
    EpollReactor() {
        kdpfd = epoll_create(MAXEPOLLSIZE); 
    }

private:
    int32_t kdpfd = 0;
};


void* pthread_handle_message(void* para);

static int count111 = 0;
static time_t oldtime = 0, nowtime = 0;

//------------------------------------------------------------
int main(int argc, char **argv)  { 
    int listener, new_fd, nfds, n, ret; 
    struct epoll_event ev; 
    int kdpfd, curfds; 
    socklen_t len; 
    struct sockaddr_in my_addr, their_addr; 
    unsigned int myport, lisnum; 
    struct epoll_event events[MAXEPOLLSIZE]; 
    struct rlimit rt;

    if (argc>1) 
        myport = atoi(argv[1]); 
    else 
        myport = 8006;

    if (argc>2) 
        lisnum = atoi(argv[2]); 
    else 
        lisnum = 10;

    /* 设置每个进程允许打开的最大文件数 */ 
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE; 
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) { 
        perror("setrlimit"); 
        exit(1); 
    } 
    else printf("设置系统资源参数成功！/n");

    /* 开启 socket 监听 */ 
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("socket"); 
        exit(1); 
    } else 
        printf("socket 创建成功！/n");

    /*设置socket属性，端口可以重用*/ 
    int opt=SO_REUSEADDR; 
    setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    /*设置socket为非阻塞模式*/ 
    setnonblocking(listener);

    bzero(&my_addr, sizeof(my_addr)); 
    my_addr.sin_family = PF_INET; 
    my_addr.sin_port = htons(myport); 
    if (argc>3) 
        my_addr.sin_addr.s_addr = inet_addr(argv[3]); 
    else 
        my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind (listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) { 
            perror("bind"); 
            exit(1); 
    } else 
        printf("IP 地址和端口绑定成功/n");

    if (listen(listener, lisnum) == -1) { 
        perror("listen"); 
        exit(1); 
    } else 
        printf("开启服务成功！/n");

    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */ 
    kdpfd = epoll_create(MAXEPOLLSIZE); 
    len = sizeof(struct sockaddr_in); 
    ev.events = EPOLLIN | EPOLLET; 
    ev.data.fd = listener; 
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0) { 
        fprintf(stderr, "epoll set insertion error: fd=%d/n", listener); 
        return -1; 
    } else 
        printf("监听 socket 加入 epoll 成功！/n"); 
    curfds = 1; 
    while (1) { 
        /* 等待有事件发生 */ 
        nfds = epoll_wait(kdpfd, events, curfds, -1); 
        if (nfds == -1) { 
            perror("epoll_wait"); 
            continue; 
        } 
        /* 处理所有事件 */ 
        for (n = 0; n < nfds; ++n) { 
            if (events[n].data.fd == listener) { 
                new_fd = accept(listener, (struct sockaddr *) &their_addr, &len); 
                if (new_fd < 0) { 
                    perror("accept"); 
                    continue; 
                } else {
                    //printf("有连接来自于： %s:%d， 分配的 socket 为:%d/n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);
                }
                setnonblocking(new_fd); 
                ev.events = EPOLLIN | EPOLLET; 
                ev.data.fd = new_fd; 
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0) { 
                    fprintf(stderr, "把 socket '%d' 加入 epoll 失败！%s/n", 
                        new_fd, strerror(errno)); 
                    return -1; 
                } 
                curfds++; 
            } else { 
                pthread_attr_t attr;
                pthread_t threadId;
                

                /*初始化属性值，均设为默认值*/ 
                pthread_attr_init(&attr); 
                pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); 
                /* 设置线程为分离属性*/ 
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
                if(pthread_create(&threadId,&attr,pthread_handle_message,(void*)&(events[n].data.fd)))
                { 
                    perror("pthread_creat error!"); 
                    exit(-1); 
                } 
            } 
        } 
    } 
    close(listener); 
    return 0; 
}


void* pthread_handle_message(void* para)
{
    char recvBuf[1024] = {0}; 
    int ret = 999;
    int rs = 1;
    int socketfd = *(int *)para;

    while(rs)
    {
        ret = recv(socketfd,recvBuf,1024,0);// 接受客户端消息

        if(ret < 0)
        {
            //由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可//读在这里就当作是该次事件已处理过。

            if(errno == EAGAIN)
            {
                printf("EAGAIN\n");
                break;
            }
            else{
                printf("recv error! errno:%d\n", errno);
        
                close(socketfd);
                break;
            }
        }
        else if(ret == 0)
        {
            // 这里表示对端的socket已正常关闭. 
            rs = 0;
        }
        if(ret == sizeof(recvBuf))
            rs = 1; // 需要再次读取
        else
            rs = 0;
    }

    if(ret>0){
        count111 ++;
        struct tm *today;
        time_t ltime;
        time( &nowtime );

        if(nowtime != oldtime){
            printf("%d\n", count111);
            oldtime = nowtime;
            count111 = 0;
        }


        char buf[1000] = {0};
        sprintf(buf,"HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n%s","Hello world!\n");
        send(socketfd,buf, strlen(buf),0);
    }
    close(socketfd);
}
