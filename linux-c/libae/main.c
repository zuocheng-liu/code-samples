#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ae.h"
#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000



void setnonblocking(int sock) {
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0) {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    
    ssize_t n;
    char line[MAXLINE];

    AE_NOTUSED(el);
    AE_NOTUSED(privdata);
    AE_NOTUSED(mask);

    socklen_t clilen;
    struct sockaddr_in clientaddr;
    int connfd = accept(fd, (struct sockaddr *)&clientaddr, &clilen);
    
    if(connfd < 0) {
        perror("connfd<0");
        exit(1);
    }
    setnonblocking(connfd);

    char *str = inet_ntoa(clientaddr.sin_addr);
    printf("connect from ");

    if ( (n = read(connfd, line, MAXLINE)) < 0) {
        if (errno == ECONNRESET) {
            close(connfd);
            return;
        } else {
            printf("readline error");
        }
    } else if (n == 0) {
        close(connfd);
        return;
    }
    printf("%s", line);
    write(connfd, line, n);
    close(connfd);

    return;
}


int main() {
    
    int listenfd, connfd;
    aeEventLoop *el; 
    socklen_t clilen;
    
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    el = aeCreateEventLoop();
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //把socket设置为非阻塞方式
    setnonblocking(listenfd);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr)); //htons(SERV_PORT);
    serveraddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
    printf("life event \r\n");
    /* create the file event */
    if (aeCreateFileEvent(el, listenfd, AE_READABLE,acceptTcpHandler,NULL) == AE_ERR) {
        printf( "Unrecoverable error creating l_socket file event.");
    }
    aeMain(el);
    aeDeleteEventLoop(el);
    return 0;
}
