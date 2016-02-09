#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int listenfd;
    int connfd;
    struct sockaddr_in servaddr;

    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(50001);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 10);
    connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

    int n;
    char recvline[1024];

    while((n=read(connfd, recvline, 1024)) > 0) {
        //recvline[n] = 0;
        printf("%s\n", recvline);
    }

    close(connfd);
    close(listenfd);

    return 1;
}
