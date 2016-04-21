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
#include "anet.h"
#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000

char errmsg[255];

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
    int cport, cfd, connfd;
    int n;
    char client_ip[128];
    char line[MAXLINE];

    AE_NOTUSED(el);
    AE_NOTUSED(mask);
    AE_NOTUSED(privdata);
setvbuf(stdout,NULL,_IONBF,0);
    connfd = anetAccept(errmsg, fd, client_ip, &cport);
    if (connfd == AE_ERR) {
        fprintf(stderr,"Accepting client connection: %s", errmsg);
        return;
    }
    fprintf(stdout, "Accepted %s:%d", client_ip, cport);

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
    fprintf(stdout ,"%s", line);
    write(connfd, line, n);
    close(connfd);
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
    fprintf(stderr, "CreateFileEvent \r\n");
    /* create the file event */
    if (aeCreateFileEvent(el, listenfd, AE_READABLE,acceptTcpHandler,NULL) == AE_ERR) {
        fprintf(stderr ,"Unrecoverable error creating l_socket file event.");
    }
    printf("Event Loop\r\n");
    aeMain(el);
    printf("delete Event Loop\r\n");
    aeDeleteEventLoop(el);
    return 0;
}
